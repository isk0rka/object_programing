/*1. Запрограммировать класс Вектор.

Предусмотреть "защиту от дурака" на введение размера вектора, ограниченного, например, значением 100.

Запрограммировать операции доступ по индексу, добавление элемента, удаление элемента из начала,
удаление элемента из конца.
Предусмотреть генерацию исключительных ситуаций:
1) попытка обратиться к элементу с номером меньше 0 или больше текущего размера вектора;
2) попытка добавить элемент с номером больше максимального размера;
3) попытка удалить элемент из пустого вектора. */


#pragma once

#include <memory>
#include <type_traits>
#include <utility>

template <typename T>
class vect
{
private:
    using allocator_type = std::allocator<T>;
    using allocator_traits = std::allocator_traits<allocator_type>;

    allocator_type allocator_;
    T* data_;
    int size_;

    void check_index(int index) const;
    void check_capacity() const;
    void ensure_storage();

public:
    static constexpr int MAX_SIZE = 100;

    vect();
    explicit vect(int initial_size);
    vect(int initial_size, const T& value);

    vect(const vect& other);
    vect(vect&& other) noexcept;

    vect& operator=(const vect& other);
    vect& operator=(vect&& other) noexcept;

    ~vect();

    int get_size() const noexcept;
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