#include <memory>

template<typename T, typename Allocator = std::allocator<T>>
struct my_list
{
    my_list() = default;
    my_list(const my_list<T, Allocator>& list) = default;
    my_list(my_list<T, Allocator>&& list) = default;
    my_list& operator=(const my_list<T, Allocator>& list) = default;
    my_list& operator=(my_list<T, Allocator>&& list) = default;
    ~my_list() noexcept {};

    void push_back(const T& val)
    {
        typename Allocator::template rebind<Node>::other nodeAlloc;
        std::shared_ptr<Node> newNode = nodeAlloc.allocate(1);
    }

private:
    struct Node
    {
        std::shared_ptr<Node> next;
        std::shared_ptr<Node> previous;
        T val;
    };
    std::shared_ptr<Node> head = nullptr;
    std::shared_ptr<Node> tail = nullptr;
    std::size_t size = 0;
    Allocator alloc;
};
