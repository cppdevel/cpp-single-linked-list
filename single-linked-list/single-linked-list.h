#pragma once

#include <cassert>
#include <cstddef>
#include <string>
#include <utility>

template <typename Type>
class SingleLinkedList {
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };

    template <typename ValueType>
    class BasicIterator {
        friend class SingleLinkedList;

        explicit BasicIterator(Node* node) {
            node_ = node;
        }

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;

        BasicIterator() = default;

        BasicIterator(const BasicIterator<Type>& other) noexcept {
            node_ = other.node_;
        }

        BasicIterator& operator=(const BasicIterator& rhs) = default;

        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return node_ != rhs.node_;
        }

        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return node_ != rhs.node_;
        }

        BasicIterator& operator++() noexcept {
            node_ = node_->next_node;
            return *this;
        }

        BasicIterator operator++(int) noexcept {
            auto old_value(*this);
            ++(*this);
            return old_value;
        }

        [[nodiscard]] reference operator*() const noexcept {
            return node_->value;
        }

        [[nodiscard]] pointer operator->() const noexcept {
            if (node_) {
                return &node_->value;
            }
            else {
                return nullptr;
            }
        }
    private:
        Node* node_ = nullptr;
    };

public:
    SingleLinkedList() {}

    template <typename TypeInitialization>
    void Initialization(TypeInitialization begin, TypeInitialization end) {
        Node* node = &head_;
        for (TypeInitialization i = begin; i != end; ++i) {
            ++size_;
            node->next_node = new Node(*i, nullptr);
            node = node->next_node;
        }
    }

    SingleLinkedList(std::initializer_list<Type> values) {
        SingleLinkedList collateral_list;
        collateral_list.Initialization(values.begin(), values.end());
        swap(collateral_list);
    }

    SingleLinkedList(const SingleLinkedList& other) {
        SingleLinkedList collateral_list;
        collateral_list.Initialization(other.begin(), other.end());
        swap(collateral_list);
    }

    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        if (this != &rhs) {
            SingleLinkedList collateral_list(rhs);
            swap(collateral_list);
        }
        return *this;
    }

    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;

    [[nodiscard]] Iterator begin() noexcept {
        return Iterator{ head_.next_node };
    }

    [[nodiscard]] Iterator end() noexcept {
        return Iterator{ nullptr };
    }

    [[nodiscard]] ConstIterator begin() const noexcept {
        return ConstIterator{ head_.next_node };
    }

    [[nodiscard]] ConstIterator end() const noexcept {
        return ConstIterator{ nullptr };
    }

    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return ConstIterator{ head_.next_node };
    }

    [[nodiscard]] ConstIterator cend() const noexcept {
        return ConstIterator{ nullptr };
    }

    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    [[nodiscard]] bool IsEmpty() const noexcept {
        if (!size_) {
            return true;
        }
        else {
            return false;
        }
    }

    void PushFront(const Type& value) {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }

    void Clear() noexcept {
        while (head_.next_node) {
            Node* new_head = head_.next_node->next_node;
            delete head_.next_node;
            head_.next_node = new_head;
        }
        size_ = 0;
    }

    void swap(SingleLinkedList& other) noexcept {
        std::swap(head_.next_node, other.head_.next_node);
        std::swap(size_, other.size_);
    }

    [[nodiscard]] Iterator before_begin() noexcept {
        return Iterator{&head_};
    }

    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return ConstIterator{const_cast<Node*>(& head_)};
    }

    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return cbefore_begin;
    }

    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        pos.node_->next_node = new Node(value, pos.node_->next_node);
        ++size_;
        return Iterator{pos.node_->next_node};
    }

    void PopFront() noexcept {
        assert(IsEmpty() == 0);
        --size_;
        Node* collateral_node_ptr = head_.next_node->next_node;
        delete head_.next_node;
        head_.next_node = collateral_node_ptr;
    }

    Iterator EraseAfter(ConstIterator pos) noexcept {
        assert(IsEmpty() == 0);
        --size_;
        Node* collateral_node_ptr = pos.node_->next_node->next_node;
        delete pos.node_->next_node;
        pos.node_->next_node = collateral_node_ptr;
        return Iterator{pos.node_->next_node};
    }

    ~SingleLinkedList() {
        Clear();
    }

private:
    Node head_;
    size_t size_ = 0;
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) {
    return lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return rhs < lhs;
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(rhs < lhs);
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs < rhs);
}
