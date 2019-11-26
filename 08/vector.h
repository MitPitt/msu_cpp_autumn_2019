#pragma once

#include <iterator>
#include <limits>
#include <utility>

template<typename T>
class Iterator : public std::iterator<std::random_access_iterator_tag, T> {
public:
    explicit Iterator(T* ptr) : ptr_(ptr) {}
    bool operator==(const Iterator<T>& other) const {
        return ptr_ == other.ptr_;
    }
    bool operator!=(const Iterator<T>& other) const {
        return !(*this == other);
    }
    T& operator*() const {
        return *ptr_;
    }
    Iterator& operator++() {
        ++ptr_;
        return *this;
    }
    Iterator& operator--() {
        --ptr_;
        return *this;
    }
    Iterator operator++(int) {
        auto tmp = ptr_;
        ++ptr_;
        return Iterator(tmp);
    }
    Iterator operator--(int) {
        auto tmp = ptr_;
        --ptr_;
        return Iterator(tmp);
    }
    Iterator operator+(size_t n) const {
        return Iterator(ptr_ + n);
    }
    Iterator operator-(size_t n) const {
        return Iterator(ptr_ - n);
    }
    Iterator& operator+=(size_t n) {
        ptr_ += n;
        return *this;
    }
    Iterator& operator-=(size_t n) {
        ptr_ -= n;
        return *this;
    }
    size_t operator-(const Iterator& other) const {
        if (ptr_ < other.ptr_) {
            throw std::invalid_argument("invalid iterators");
        }
        return ptr_ - other.ptr_;
    }
    bool operator>(const Iterator& other) const {
        return ptr_ > other.ptr_;
    }
    bool operator>=(const Iterator& other) const {
        return (*this == other) || (*this > other);
    }
    bool operator<(const Iterator& other) const {
        return !(*this >= other);
    }
    bool operator<=(const Iterator& other) const {
        return !(*this > other);
    }
    T& operator[](size_t n) {
        return *(ptr_ + n);
    }
private:
    T* ptr_;
};


template <class T>
class Allocator {
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using size_type = size_t;

    pointer allocate(size_type count) {
        return static_cast<pointer>(operator new(count * sizeof(T)));
    }
	
    void deallocate(pointer ptr, size_type count) {
        operator delete(ptr);
    }
	
    template <typename... Args>
    void construct(pointer ptr, Args&&... args) {
        new(ptr) T(std::forward <Args> (args)...);
    }
	
    void destroy(pointer ptr) {
        ptr->~T();
    }
	
    size_type max_size() const noexcept {
        return std::numeric_limits<size_type>::max();
    }
};


template <class T, class Alloc = Allocator<T>>
class Vector {
public:
    using iterator = Iterator<T>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using value_type = T;
    using reference = T&;
    using pointer = T*;
    Vector() : data_(nullptr), size_(0), capacity_(0) {}
    explicit Vector(size_t initial_size) : data_(alloc_.allocate(initial_size * 2)), \
            size_(initial_size), capacity_(initial_size * 2) {
        for(size_t i = 0; i < size_; i++)
            alloc_.construct(data_ + i);
    }
    void resize(size_t size) {
        if (size_ < size) {
            reserve(size * 2);
            for (size_t i = size_; i < size; i++) {
                alloc_.construct(data_ + i);
            }
            size_ = size;
        } else if(size_ > size) {
            for (size_t i = size; i < size_; i++) {
                alloc_.destroy(data_ + i);
            }
            size_ = size;
        }
    }
    void reserve(size_t new_cap) {
        if (capacity_ < new_cap) {
            pointer buf = alloc_.allocate(new_cap);
            for(size_t i = 0; i < size_; i++) {
                alloc_.construct(buf + i, std::move(*(data_ + i)));
                alloc_.destroy(data_ + i);
            }
            alloc_.deallocate(data_, capacity_);
            capacity_ = new_cap;
            data_ = buf;
        }
    }
    size_t capacity() const {
        return capacity_;
    }
    void pop_back() {
        if (size_ > 0) {
            alloc_.destroy(data_ + size_);
            --size_;
        }
    }
    void push_back(const T& new_el) {
        if (size_ >= capacity_) {
            size_t new_cap = (capacity_ > 0) ? (capacity_ * 2) : 2;
            reserve(new_cap);
        }
        alloc_.construct(data_ + size_, (new_el));
        size_++;
    }
    void push_back(T&& new_el) {
        if (size_ >= capacity_) {
            size_t new_cap = (capacity_ > 0) ? (capacity_ * 2) : 2;
            reserve(new_cap);
        }
        alloc_.construct(data_ + size_, std::move(new_el));
        size_++;
    }
    iterator begin() {
        return iterator(data_);
    }
    iterator end() {
        return iterator(data_ + size_);
    }
    reverse_iterator rbegin() {
        return reverse_iterator(this->end());
    }
    reverse_iterator rend() {
        return reverse_iterator(this->begin());
    }
    reference operator[](int i) {
        if (i < size_) {
            return data_[i];
        } else {
            throw std::out_of_range("out of range");
        }
    }
    size_t size() const {
        return size_;
    }
    bool empty() {
        return (size_ == 0);
    }
    void clear() {
        for(size_t i = 0; i < size_; ++i) {
            alloc_.destroy(data_ + i);
        }
        size_ = 0;
        capacity_ = 0;
    }
    ~Vector() {
        clear();
        alloc_.deallocate(data_, capacity_);
    }
private:
    Alloc alloc_;
    T* data_;
    size_t size_;
    size_t capacity_;
};
