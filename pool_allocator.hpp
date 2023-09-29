#include <algorithm>
#include <iostream>
#include <list>
#include <memory>
#include <unordered_map>

#define LOG_NAME_ON (false)
#define LOG_ON (false)

#define SHOW_FUNC                                      \
    if (LOG_NAME_ON)                                   \
    {                                                  \
        std::cout << __PRETTY_FUNCTION__ << std::endl; \
    }

template <typename T>
struct pool_allocator
{

    using value_type = T;

    using propagate_on_container_copy_assignment = std::true_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap = std::true_type;

    pool_allocator select_on_container_copy_construction() const
    {
        return pool_allocator();
    }

    template <class U>
    struct rebind
    {
        typedef pool_allocator<U> other;
    };

    using pool_item_t = std::shared_ptr<T>;
    using pool_item_weak_t = std::weak_ptr<T>;
    using pool_t = std::list<pool_item_t>;

    using alloc_map_t = std::unordered_map<std::size_t, pool_t>;
    using dealloc_map_t = std::unordered_map<T *, pool_item_weak_t>;

    static alloc_map_t alloc_map;
    static dealloc_map_t dealloc_map;

    void show_state(const pool_allocator<T> &allocator)
    {
        if (!LOG_ON)
        {
            return;
        }
        if (!allocator.alloc_map.empty())
        {
            std::cout << "-----  alloc_map  -----" << std::endl;
            for (const auto &pool : allocator.alloc_map)
            {
                if (!pool.second.empty())
                {
                    std::cout << "[" << pool.first << "][" << pool.second.size() << "]" << std::endl;
                    for (const auto &pool_item : pool.second)
                    {
                        std::cout << "{ " << pool_item.get() << " }" << std::endl;
                    }
                }
            }
            std::cout << std::endl;
        }
        if (!allocator.dealloc_map.empty())
        {
            std::cout << "----- dealloc_map -----" << std::endl;
            for (const auto &pool : allocator.dealloc_map)
            {
                std::cout << "{ " << pool.first << " }" << std::endl;
            }
            std::cout << std::endl;
        }
    }

    pool_allocator() = default;
    pool_allocator(const pool_allocator<T> &allocator) = default;
    pool_allocator(pool_allocator<T> &&allocator) = default;
    pool_allocator &operator=(const pool_allocator<T> &allocator) = default;
    pool_allocator &operator=(pool_allocator<T> &&allocator) = default;

    template <class U>
    pool_allocator([[maybe_unused]]const pool_allocator<U> &allocator)
    {
        dealloc_map = allocator.dealloc_map;
        alloc_map = allocator.alloc_map;
    }

    ~pool_allocator() noexcept
    {
        SHOW_FUNC;
        dealloc_map.clear();
        alloc_map.clear();
        show_state(*this);
    }

    T *allocate([[maybe_unused]] std::size_t n)
    {
        SHOW_FUNC;
        auto find_list = alloc_map.find(n);
        if (find_list == alloc_map.end())
        {
            pool_item_t new_pool_item{static_cast<T *>(::operator new(sizeof(T) * n))};
            if (!new_pool_item)
            {
                std::bad_alloc();
            }
            pool_t new_pool{new_pool_item};
            alloc_map.emplace(n, std::move(new_pool));
        }

        find_list = alloc_map.find(n);
        if (dealloc_map.find(find_list->second.front().get()) != dealloc_map.end())
        {
            pool_item_t new_pool_item{static_cast<T *>(::operator new(sizeof(T) * n))};
            if (!new_pool_item)
            {
                std::bad_alloc();
            }
            find_list->second.emplace_back(std::move(new_pool_item));
            dealloc_map.emplace(find_list->second.back().get(), find_list->second.back());
            show_state(*this);
            return find_list->second.back().get();
        }
        else
        {
            find_list->second.emplace_back(find_list->second.front());
            find_list->second.pop_front();
            dealloc_map.emplace(find_list->second.back().get(), find_list->second.back());
            show_state(*this);
            return find_list->second.back().get();
        }
    }

    void deallocate(T *p, [[maybe_unused]] std::size_t n)
    {
        SHOW_FUNC;
        auto item = dealloc_map.find(p);
        if (item != dealloc_map.end())
        {
            dealloc_map.erase(item);
            show_state(*this);
            return;
        }
        std::bad_alloc();
    }
};

template <typename T>
typename pool_allocator<T>::alloc_map_t pool_allocator<T>::alloc_map;

template <typename T>
typename pool_allocator<T>::dealloc_map_t pool_allocator<T>::dealloc_map;

template <class T, class U>
constexpr bool operator==([[maybe_unused]] const pool_allocator<T> &a1, [[maybe_unused]] const pool_allocator<U> &a2) noexcept
{
    return false;
}

template <class T, class U>
constexpr bool operator!=([[maybe_unused]] const pool_allocator<T> &a1, [[maybe_unused]] const pool_allocator<U> &a2) noexcept
{
    return true;
}