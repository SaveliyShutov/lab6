#include "unordered_set.h"

#include <cstring>
#include <new>
#include <string>
#include <utility>

template <typename T>
UnorderedSet<T>::UnorderedSet() : data_(nullptr), size_(0), capacity_(0) {}

template <typename T> UnorderedSet<T>::~UnorderedSet() { delete[] data_; }

template <typename T>
UnorderedSet<T>::UnorderedSet(const UnorderedSet &other)
    : data_(nullptr), size_(0), capacity_(0) {
  EnsureCapacity(other.size_);
  for (std::size_t i = 0; i < other.size_; ++i) {
    data_[i] = other.data_[i];
  }
  size_ = other.size_;
}

template <typename T>
UnorderedSet<T> &UnorderedSet<T>::operator=(const UnorderedSet &other) {
  if (this != &other) {
    UnorderedSet temp(other);
    Swap(temp);
  }
  return *this;
}

template <typename T>
UnorderedSet<T>::UnorderedSet(UnorderedSet &&other) noexcept
    : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
  other.data_ = nullptr;
  other.size_ = 0;
  other.capacity_ = 0;
}

template <typename T>
UnorderedSet<T> &UnorderedSet<T>::operator=(UnorderedSet &&other) noexcept {
  if (this != &other) {
    delete[] data_;
    data_ = other.data_;
    size_ = other.size_;
    capacity_ = other.capacity_;
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
  }
  return *this;
}

template <typename T> std::size_t UnorderedSet<T>::Size() const {
  return size_;
}

template <typename T> bool UnorderedSet<T>::Contains(const T &value) const {
  return Find(value) != kNotFound;
}

template <typename T> void UnorderedSet<T>::Add(const T &value) {
  if (Contains(value))
    return;
  EnsureCapacity(size_ + 1);
  data_[size_++] = value;
}

template <typename T> bool UnorderedSet<T>::Remove(const T &value) {
  std::size_t index = Find(value);
  if (index == kNotFound)
    return false;
  for (std::size_t i = index; i + 1 < size_; ++i) {
    data_[i] = data_[i + 1];
  }
  --size_;
  return true;
}

template <typename T>
UnorderedSet<T> UnorderedSet<T>::Union(const UnorderedSet &other) const {
  UnorderedSet result(*this);
  for (std::size_t i = 0; i < other.size_; ++i) {
    result.Add(other.data_[i]);
  }
  return result;
}

template <typename T>
UnorderedSet<T> UnorderedSet<T>::Except(const UnorderedSet &other) const {
  UnorderedSet result;
  for (std::size_t i = 0; i < size_; ++i) {
    if (!other.Contains(data_[i])) {
      result.Add(data_[i]);
    }
  }
  return result;
}

template <typename T>
UnorderedSet<T> UnorderedSet<T>::Intersect(const UnorderedSet &other) const {
  UnorderedSet result;
  for (std::size_t i = 0; i < size_; ++i) {
    if (other.Contains(data_[i])) {
      result.Add(data_[i]);
    }
  }
  return result;
}

template <typename T> std::vector<T> UnorderedSet<T>::ToVector() const {
  return std::vector<T>(data_, data_ + size_);
}

template <typename T> void UnorderedSet<T>::Clear() {
  delete[] data_;
  data_ = nullptr;
  size_ = 0;
  capacity_ = 0;
}

template <typename T> bool UnorderedSet<T>::IsEmpty() const {
  return size_ == 0;
}

template <typename T> void UnorderedSet<T>::Swap(UnorderedSet &other) noexcept {
  std::swap(data_, other.data_);
  std::swap(size_, other.size_);
  std::swap(capacity_, other.capacity_);
}

template <typename T> std::size_t UnorderedSet<T>::Find(const T &value) const {
  for (std::size_t i = 0; i < size_; ++i) {
    if (data_[i] == value) {
      return i;
    }
  }
  return kNotFound;
}

template <typename T>
void UnorderedSet<T>::EnsureCapacity(std::size_t min_capacity) {
  if (capacity_ >= min_capacity)
    return;
  std::size_t new_capacity = capacity_ == 0 ? kInitialCapacity : capacity_ * 2;
  while (new_capacity < min_capacity)
    new_capacity *= 2;
  T *new_data = new T[new_capacity];
  for (std::size_t i = 0; i < size_; ++i) {
    new_data[i] = data_[i];
  }
  delete[] data_;
  data_ = new_data;
  capacity_ = new_capacity;
}

template class UnorderedSet<int>;
template class UnorderedSet<std::string>;