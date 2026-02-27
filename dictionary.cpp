#include "dictionary.h"

#include <utility>
#include <new>
#include <string>

template <typename K, typename V>
Dictionary<K,V>::Dictionary()
  : data_(nullptr), size_(0), capacity_(0) {}

template <typename K, typename V>
Dictionary<K,V>::~Dictionary() {
  delete[] data_;
}

template <typename K, typename V>
Dictionary<K,V>::Dictionary(const Dictionary& other)
  : data_(nullptr), size_(0), capacity_(0) {
  EnsureCapacity(other.size_);
  for (std::size_t i = 0; i < other.size_; ++i) data_[i] = other.data_[i];
  size_ = other.size_;
}

template <typename K, typename V>
Dictionary<K,V>& Dictionary<K,V>::operator=(const Dictionary& other) {
  if (this != &other) {
    Dictionary tmp(other);
    Swap(tmp);
  }
  return *this;
}

template <typename K, typename V>
Dictionary<K,V>::Dictionary(Dictionary&& other) noexcept
  : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
  other.data_ = nullptr; other.size_ = 0; other.capacity_ = 0;
}

template <typename K, typename V>
Dictionary<K,V>& Dictionary<K,V>::operator=(Dictionary&& other) noexcept {
  if (this != &other) {
    delete[] data_;
    data_ = other.data_; size_ = other.size_; capacity_ = other.capacity_;
    other.data_ = nullptr; other.size_ = 0; other.capacity_ = 0;
  }
  return *this;
}

template <typename K, typename V>
std::size_t Dictionary<K,V>::Size() const { return size_; }

template <typename K, typename V>
bool Dictionary<K,V>::IsEmpty() const { return size_ == 0; }

template <typename K, typename V>
void Dictionary<K,V>::Add(const K& key, const V& value) {
  std::size_t idx = FindIndex(key);
  if (idx != static_cast<std::size_t>(-1)) {
    data_[idx].second = value; // обновление
    return;
  }
  EnsureCapacity(size_ + 1);
  data_[size_++] = std::make_pair(key, value);
}

template <typename K, typename V>
bool Dictionary<K,V>::Remove(const K& key) {
  std::size_t idx = FindIndex(key);
  if (idx == static_cast<std::size_t>(-1)) return false;
  for (std::size_t i = idx; i + 1 < size_; ++i) data_[i] = data_[i+1];
  --size_;
  return true;
}

template <typename K, typename V>
bool Dictionary<K,V>::Contains(const K& key) const {
  return FindIndex(key) != static_cast<std::size_t>(-1);
}

template <typename K, typename V>
V* Dictionary<K,V>::Get(const K& key) {
  std::size_t idx = FindIndex(key);
  if (idx == static_cast<std::size_t>(-1)) return nullptr;
  return &data_[idx].second;
}

template <typename K, typename V>
const V* Dictionary<K,V>::Get(const K& key) const {
  std::size_t idx = FindIndex(key);
  if (idx == static_cast<std::size_t>(-1)) return nullptr;
  return &data_[idx].second;
}

template <typename K, typename V>
std::vector<std::pair<K,V>> Dictionary<K,V>::ToVector() const {
  return std::vector<std::pair<K,V>>(data_, data_ + size_);
}

template <typename K, typename V>
void Dictionary<K,V>::Clear() {
  delete[] data_;
  data_ = nullptr; size_ = 0; capacity_ = 0;
}

template <typename K, typename V>
std::size_t Dictionary<K,V>::FindIndex(const K& key) const {
  for (std::size_t i = 0; i < size_; ++i) if (data_[i].first == key) return i;
  return static_cast<std::size_t>(-1);
}

template <typename K, typename V>
void Dictionary<K,V>::EnsureCapacity(std::size_t min_capacity) {
  if (capacity_ >= min_capacity) return;
  std::size_t new_capacity = capacity_ == 0 ? kInitialCapacity : capacity_ * 2;
  while (new_capacity < min_capacity) new_capacity *= 2;
  std::pair<K,V>* new_data = new std::pair<K,V>[new_capacity];
  for (std::size_t i = 0; i < size_; ++i) new_data[i] = data_[i];
  delete[] data_;
  data_ = new_data;
  capacity_ = new_capacity;
}

template <typename K, typename V>
void Dictionary<K,V>::Swap(Dictionary& other) noexcept {
  std::swap(data_, other.data_);
  std::swap(size_, other.size_);
  std::swap(capacity_, other.capacity_);
}

// --- явные инстанциации ---
// Добавьте сюда новые инстанциации, если будете использовать другие комбинации типов.
template class Dictionary<std::string, long long>;
template class Dictionary<std::string, int>;