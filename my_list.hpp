#include <memory>

template <typename T, typename Allocator = std::allocator<T>>
struct my_list
{
  private:
    struct Node
    {
        Node* next;
        Node* previous;
        T val;
    };

    Node* head = nullptr;
    Node* tail = nullptr;
    std::size_t size = 0;

    using NodeAllocator = typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;
    NodeAllocator alloc;

  public:
    class Iterator
    {
      private:
        Node* node;

      public:
        explicit Iterator(Node* node)
            : node(node)
        {
        }

        T& operator*() const
        {
            return node->val;
        }

        Iterator& operator++()
        {
            node = node->next;
            return *this;
        }

        bool operator!=(const Iterator& other) const
        {
            return node != other.node;
        }
    };

    my_list() = default;
    my_list(const my_list<T, Allocator>& list) = default;
    my_list(my_list<T, Allocator>&& list) = default;
    my_list& operator=(const my_list<T, Allocator>& list) = default;
    my_list& operator=(my_list<T, Allocator>&& list) = default;
    ~my_list() noexcept {};

    Iterator begin() const
    {
        return Iterator(head);
    }

    Iterator end() const
    {
        return Iterator(nullptr);
    }

    void push_back(const T& val)
    {
        Node* newNode = std::allocator_traits<NodeAllocator>::allocate(alloc, 1);
        newNode->val = val;
        newNode->next = nullptr;
        newNode->previous = tail;
        if (0 == size)
        {
            head = newNode;
        }
        else
        {
            tail->next = newNode;
        }
        tail = newNode;
        ++size;
    }

    void clear()
    {
        auto current = head;

        while (current)
        {
            auto next = current->next;
            current->next = nullptr;
            current->previous = nullptr;
            std::allocator_traits<NodeAllocator>::destroy(alloc, current);
            std::allocator_traits<NodeAllocator>::deallocate(alloc, current, 1);
            current = next;
        }

        head = nullptr;
        tail = nullptr;
    }
};
