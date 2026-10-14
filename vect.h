#ifndef VECT_H
#define VECT_H

#include <memory>
#include <utility>

template <typename T>
class vect {
private:
    using allocator_type = std::allocator<T>;
    using allocator_traits = std::allocator_traits<allocator_type>;

    allocator_type allocator_;
    T* data_ = nullptr;
    int size_ = 0;
    int capacity_ = 0;

    static void validate_size(int requested_size);
    void check_index(int index) const;
    void check_capacity() const;
    int next_capacity() const noexcept;
    void destroy_elements() noexcept;
    void release_storage() noexcept;

public:
    static constexpr int MAX_SIZE = 100;

    vect() noexcept = default;
    explicit vect(int initial_size);
    vect(int initial_size, const T& value);

    vect(const vect& other);
    vect(vect&& other) noexcept;

    vect& operator=(const vect& other);
    vect& operator=(vect&& other) noexcept;

    ~vect();

    int get_size() const noexcept;
    int get_capacity() const noexcept;
    bool is_empty() const noexcept;

    T& operator[](int index);
    const T& operator[](int index) const;

    template <typename... Args>
    T& emplace_back(Args&&... args);

    void push_back(const T& value);
    void push_back(T&& value);
    void insert(int index, T value);

    void pop_front();
    void pop_back();
    void clear() noexcept;

    void swap(vect& other) noexcept;
};
#include "vect.cpp"
#endif
