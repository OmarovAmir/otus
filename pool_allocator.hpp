#include <algorithm>
#include <iostream>
#include <list>
#include <memory>
#include <unordered_map>

#define SHOW_FUNC (std::cout << __PRETTY_FUNCTION__ << std::endl)

template <typename T>
class pool_allocator
{
    using pool_item = std::shared_ptr<T>;
    using pool = std::list<pool_item>;
    using pool_map = std::unordered_multimap<std::size_t, pool>;

    pool_map free_pool;
    pool_map used_pool;

    void show_state(const pool_allocator<T> &allocator)
    {
        auto show_pool = [](const auto &pool_map)
        {
            std::cout << "size: " << pool_map.size() << std::endl;
            for (const auto &pool : pool_map)
            {
                std::cout << "[" << pool.first << "].size: " << pool.second.size() << std::endl;
                if (!pool.second.empty())
                {
                    std::cout << "[" << pool.first << "] = { ";
                    auto pool_item = pool.second.cbegin();
                    while (pool_item != pool.second.cend())
                    {
                        if (pool_item != pool.second.cbegin())
                        {
                            std::cout << ", ";
                        }
                        std::cout << pool_item->get();
                        ++pool_item;
                    }
                    std::cout << " };" << std::endl;
                }
            }
        };
        std::cout << "----- free_pool -----" << std::endl;
        show_pool(allocator.free_pool);
        std::cout << "----- used_pool -----" << std::endl;
        show_pool(allocator.used_pool);
    }

  public:
    using value_type = T;

    using propagate_on_container_copy_assignment = std::true_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap = std::true_type;

    template <class U>
    struct rebind
    {
        using other = pool_allocator<U>;
    };

    pool_allocator() noexcept
    {
        free_pool.emplace(1, pool{});
        used_pool.emplace(1, pool{});
        // SHOW_FUNC;
        // show_state(*this);
    };
    pool_allocator([[maybe_unused]] const pool_allocator<T> &allocator) noexcept
    {
        free_pool = allocator.free_pool;
        used_pool = allocator.used_pool;
        // SHOW_FUNC;
        // show_state(*this);
        // show_state(allocator);
    };
    ~pool_allocator() noexcept
    {
        /*
             1. memoryClean для всех pool_item в free_pool и used_pool
             2. release для всех pool_item
        */
        auto remove_pool = [](auto &pool_map)
        {
            for (auto &pool : pool_map)
            {
                if (!pool.second.empty())
                {
                    for (auto pool_item : pool.second)
                    {
                        pool_item.reset();
                    }
                    pool.second.clear();
                }
            }
        };
        remove_pool(free_pool);
        remove_pool(used_pool);
        // SHOW_FUNC;
        // show_state(*this);
    }

    T *allocate([[maybe_unused]] std::size_t n)
    {
        /*
             Поиск pool размера n в free_pool
             Если pool не нашёлся
                  Создаем pool
                  Добавляем его в free_pool[n]
             Если в pool нет свободного pool_item
                  Выделяем память static_cast<T*>(::operator new(n*sizeof(T)))
                  Если не выделилась
                       std::bad_alloc();
                  Создаём новый pool_item
             Переносим pool_item в used_pool[n]->second
             Возвращаем указатель на данные в pool_item через get()
        */

        pool_item result;
        auto find_free_pool = free_pool.find(n);
        auto find_used_pool = used_pool.find(n);
        if (find_free_pool != free_pool.end())
        {
            if (!find_free_pool->second.empty())
            {
                result = std::move(find_free_pool->second.back());
                find_used_pool->second.push_back(result);
            }
            else
            {
                pool_item new_item = pool_item(static_cast<T *>(::operator new[](sizeof(T) * n)));
                result = new_item;
                find_used_pool->second.push_back(result);
            }
        }
        else
        {
            pool_item new_item = pool_item(static_cast<T *>(::operator new[](sizeof(T) * n)));
            free_pool.emplace(n, pool{});
            result = new_item;
            used_pool.emplace(n, pool{std::move(new_item)});
        }
        // SHOW_FUNC;
        // std::cout << n << std::endl;
        // show_state(*this);
        return result.get();
    }

    void deallocate([[maybe_unused]] T *p, [[maybe_unused]] std::size_t n)
    {
        pool_item remove_item;
        auto find_free_pool = free_pool.find(n);
        auto find_used_pool = used_pool.find(n);
        if (find_used_pool != used_pool.end())
        {
            if (!find_used_pool->second.empty())
            {
                for (auto pool_item : find_used_pool->second)
                {
                    if (pool_item.get() == p)
                    {
                        remove_item = pool_item;
                        find_used_pool->second.remove(pool_item);
                        break;
                    }
                }
                find_free_pool->second.push_back(std::move(remove_item));
            }
            else
            {
                ::operator delete[](p, sizeof(T) * n);
            }
        }
        else
        {
            ::operator delete[](p, sizeof(T) * n);
        }
        // SHOW_FUNC;
        // std::cout << n << std::endl;
        // show_state(*this);
    }
};

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