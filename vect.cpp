#ifndef VECT_TEMPLATE_IMPLEMENTATION
#define VECT_TEMPLATE_IMPLEMENTATION

#include "vect.h"
#include <stdexcept>
#include <type_traits>
#include <utility>

template <typename T>
void vect<T>::validate_size(int requested_size) {
    if (requested_size < 0) {
        throw std::invalid_argument("Размер вектора не может быть отрицательным.");
    }
    if (requested_size > MAX_SIZE) {
        throw std::length_error("Размер вектора превышает размера (100)");
    }
}

template <typename T>
void vect<T>::check_index(int index) const {
    if (index < 0 || index >= size_) {
        throw std::out_of_range("Индекс вектора выходит за пределы допустимого диапазона");
    }
}

template <typename T>
void vect<T>::check_capacity() const {
    if (size_ == MAX_SIZE) {
        throw std::length_error("Вектор не может содержать более 100 элементов.");
    }
}

template <typename T>
int vect<T>::next_capacity() const noexcept {
    if (capacity_ == 0) {
        return 1;
    }
    return capacity_ >= MAX_SIZE / 2 ? MAX_SIZE : capacity_ * 2;
}

template <typename T>
void vect<T>::destroy_elements() noexcept {
    while (size_ > 0) {
        --size_;
        allocator_traits::destroy(allocator_, data_ + size_);
    }
}

template <typename T>
void vect<T>::release_storage() noexcept {
    destroy_elements();
    if (data_ != nullptr) {
        allocator_traits::deallocate(allocator_, data_, capacity_);
        data_ = nullptr;
        capacity_ = 0;
    }
}

template <typename T>
vect<T>::vect(int initial_size) {
    validate_size(initial_size);
    if (initial_size == 0) {
        return;
    }

    data_ = allocator_traits::allocate(allocator_, initial_size);
    capacity_ = initial_size;
    try {
        while (size_ < initial_size) {
            allocator_traits::construct(allocator_, data_ + size_);
            ++size_;
        }
    } catch (...) {
        release_storage();
        throw;
    }
}

template <typename T>
vect<T>::vect(int initial_size, const T& value) {
    validate_size(initial_size);
    if (initial_size == 0) {
        return;
    }

    data_ = allocator_traits::allocate(allocator_, initial_size);
    capacity_ = initial_size;
    try {
        while (size_ < initial_size) {
            allocator_traits::construct(allocator_, data_ + size_, value);
            ++size_;
        }
    } catch (...) {
        release_storage();
        throw;
    }
}

template <typename T>
vect<T>::vect(const vect& other) {
    if (other.size_ == 0) {
        return;
    }
    data_ = allocator_traits::allocate(allocator_, other.size_);
    capacity_ = other.size_;
    try {
        while (size_ < other.size_) {
            allocator_traits::construct(allocator_, data_ + size_, other.data_[size_]);
            ++size_;
        }
    } catch (...) {
        release_storage();
        throw;
    }
}

template <typename T>
vect<T>::vect(vect&& other) noexcept
    : data_(std::exchange(other.data_, nullptr)),
      size_(std::exchange(other.size_, 0)),
      capacity_(std::exchange(other.capacity_, 0)) {}

template <typename T>
vect<T>& vect<T>::operator=(const vect& other) {
    if (this != &other) {
        vect copy(other);
        swap(copy);
    }
    return *this;
}

template <typename T>
vect<T>& vect<T>::operator=(vect&& other) noexcept {
    if (this != &other) {
        release_storage();
        data_ = std::exchange(other.data_, nullptr);
        size_ = std::exchange(other.size_, 0);
        capacity_ = std::exchange(other.capacity_, 0);
    }
    return *this;
}

template <typename T>
vect<T>::~vect() {
    release_storage();
}

template <typename T>
int vect<T>::get_size() const noexcept {
    return size_;
}

template <typename T>
int vect<T>::get_capacity() const noexcept {
    return capacity_;
}

template <typename T>
bool vect<T>::is_empty() const noexcept {
    return size_ == 0;
}

template <typename T>
T& vect<T>::operator[](int index) {
    check_index(index);
    return data_[index];
}

template <typename T>
const T& vect<T>::operator[](int index) const {
    check_index(index);
    return data_[index];
}

template <typename T>
template <typename... Args>
T& vect<T>::emplace_back(Args&&... args) {
    check_capacity();
    if (size_ < capacity_) {
        allocator_traits::construct(allocator_, data_ + size_, std::forward<Args>(args)...);
        return data_[size_++];
    }

    const int new_capacity = next_capacity();
    T* new_data = allocator_traits::allocate(allocator_, new_capacity);
    bool appended = false;
    int relocated = 0;
    try {
        allocator_traits::construct(allocator_, new_data + size_, std::forward<Args>(args)...);
        appended = true;
        for (; relocated < size_; ++relocated) {
            allocator_traits::construct(allocator_, new_data + relocated,
                                        std::move_if_noexcept(data_[relocated]));
        }
    } catch (...) {
        for (int i = 0; i < relocated; ++i) {
            allocator_traits::destroy(allocator_, new_data + i);
        }
        if (appended) {
            allocator_traits::destroy(allocator_, new_data + size_);
        }
        allocator_traits::deallocate(allocator_, new_data, new_capacity);
        throw;
    }

    const int old_size = size_;
    release_storage();
    data_ = new_data;
    size_ = old_size;
    capacity_ = new_capacity;
    return data_[size_++];
}

template <typename T>
void vect<T>::push_back(const T& value) {
    emplace_back(value);
}

template <typename T>
void vect<T>::push_back(T&& value) {
    emplace_back(std::move(value));
}

template <typename T>
void vect<T>::insert(int index, T value) {
    if (index < 0 || index > size_) {
        throw std::out_of_range("Индекс вставки выходит за пределы допустимого диапазона");
    }
    check_capacity();

    const int new_capacity = size_ < capacity_ ? capacity_ : next_capacity();
    T* new_data = allocator_traits::allocate(allocator_, new_capacity);
    bool inserted = false;
    int prefix = 0;
    int suffix = 0;
    try {
        allocator_traits::construct(allocator_, new_data + index, std::move(value));
        inserted = true;
        for (int i = 0; i < index; ++i) {
            allocator_traits::construct(allocator_, new_data + i,
                                        std::move_if_noexcept(data_[i]));
            ++prefix;
        }
        for (int i = index; i < size_; ++i) {
            allocator_traits::construct(allocator_, new_data + i + 1,
                                        std::move_if_noexcept(data_[i]));
            ++suffix;
        }
    } catch (...) {
        for (int i = 0; i < prefix; ++i) {
            allocator_traits::destroy(allocator_, new_data + i);
        }
        if (inserted) {
            allocator_traits::destroy(allocator_, new_data + index);
        }
        for (int i = 0; i < suffix; ++i) {
            allocator_traits::destroy(allocator_, new_data + index + 1 + i);
        }
        allocator_traits::deallocate(allocator_, new_data, new_capacity);
        throw;
    }

    const int new_size = size_ + 1;
    release_storage();
    data_ = new_data;
    size_ = new_size;
    capacity_ = new_capacity;
}

template <typename T>
void vect<T>::pop_front() {
    if (is_empty()) {
        throw std::underflow_error("Невозможно удалить элемент из пустого вектора");
    }
    if (size_ == 1) {
        pop_back();
        return;
    }

    if constexpr (std::is_move_assignable_v<T> || std::is_copy_assignable_v<T>) {
        for (int i = 1; i < size_; ++i) {
            data_[i - 1] = std::move_if_noexcept(data_[i]);
        }
        pop_back();
    } else {
        T* new_data = allocator_traits::allocate(allocator_, capacity_);
        int constructed = 0;
        try {
            for (int i = 1; i < size_; ++i) {
                allocator_traits::construct(allocator_, new_data + constructed,
                                            std::move_if_noexcept(data_[i]));
                ++constructed;
            }
        } catch (...) {
            for (int i = 0; i < constructed; ++i) {
                allocator_traits::destroy(allocator_, new_data + i);
            }
            allocator_traits::deallocate(allocator_, new_data, capacity_);
            throw;
        }
        const int old_capacity = capacity_;
        release_storage();
        data_ = new_data;
        size_ = constructed;
        capacity_ = old_capacity;
    }
}

template <typename T>
void vect<T>::pop_back() {
    if (is_empty()) {
        throw std::underflow_error("Невозможно удалить элемент из пустого вектора");
    }
    --size_;
    allocator_traits::destroy(allocator_, data_ + size_);
}

template <typename T>
void vect<T>::clear() noexcept {
    destroy_elements();
}

template <typename T>
void vect<T>::swap(vect& other) noexcept {
    using std::swap;
    swap(data_, other.data_);
    swap(size_, other.size_);
    swap(capacity_, other.capacity_);
}

#endif
