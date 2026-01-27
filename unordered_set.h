#ifndef UNORDERED_SET_H_
#define UNORDERED_SET_H_

#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include <vector>

/// <summary>Класс, реализующий функционал неупорядоченного списка с уникальными элементами.</summary>
/// <typeparam name="T">Тип элементов, хранящихся в множестве.</typeparam>
/// <remarks>Доступ к элементам по индексу отсутствует. Дубликаты игнорируются при добавлении.</remarks>
template <typename T>
class UnorderedSet {
 public:
  /// <summary>Конструктор по умолчанию. Создает пустое множество.</summary>
  UnorderedSet() : data_(nullptr), size_(0), capacity_(0) {}
  
  /// <summary>Деструктор. Освобождает выделенную память.</summary>
  ~UnorderedSet() { delete[] data_; }
  
  /// <summary>Копирующий конструктор.</summary>
  /// <param name="other">Множество для копирования.</param>
  UnorderedSet(const UnorderedSet& other) 
      : data_(nullptr), size_(0), capacity_(0) {
    EnsureCapacity(other.size_);
    for (std::size_t i = 0; i < other.size_; ++i) {
      data_[i] = other.data_[i];
    }
    size_ = other.size_;
  }
  
  /// <summary>Оператор присваивания копированием.</summary>
  /// <param name="other">Множество для копирования.</param>
  /// <returns>Ссылка на текущий объект.</returns>
  UnorderedSet& operator=(const UnorderedSet& other) {
    if (this != &other) {
      UnorderedSet temp(other);
      Swap(temp);
    }
    return *this;
  }
  
  /// <summary>Перемещающий конструктор.</summary>
  /// <param name="other">Множество для перемещения.</param>
  UnorderedSet(UnorderedSet&& other) noexcept
      : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
  }
  
  /// <summary>Оператор присваивания перемещением.</summary>
  /// <param name="other">Множество для перемещения.</param>
  /// <returns>Ссылка на текущий объект.</returns>
  UnorderedSet& operator=(UnorderedSet&& other) noexcept {
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
  
  /// <summary>Возвращает количество элементов в множестве.</summary>
  /// <returns>Количество элементов.</returns>
  std::size_t Size() const { return size_; }
  
  /// <summary>Проверяет наличие элемента в множестве.</summary>
  /// <param name="value">Элемент для проверки.</param>
  /// <returns>true, если элемент найден, иначе false.</returns>
  bool Contains(const T& value) const {
    return Find(value) != kNotFound;
  }
  
  /// <summary>Добавляет элемент в множество.</summary>
  /// <param name="value">Элемент для добавления.</param>
  /// <remarks>Если элемент уже существует, добавление не происходит.</remarks>
  void Add(const T& value) {
    if (Contains(value)) return;
    EnsureCapacity(size_ + 1);
    data_[size_++] = value;
  }
  
  /// <summary>Удаляет элемент из множества.</summary>
  /// <param name="value">Элемент для удаления.</param>
  /// <returns>true, если элемент был удален, иначе false.</returns>
  bool Remove(const T& value) {
    std::size_t index = Find(value);
    if (index == kNotFound) return false;
    
    // Сдвигаем элементы для сохранения порядка (без swap)
    for (std::size_t i = index; i + 1 < size_; ++i) {
      data_[i] = data_[i + 1];
    }
    --size_;
    return true;
  }
  
  /// <summary>Объединяет текущее множество с другим.</summary>
  /// <param name="other">Множество для объединения.</param>
  /// <returns>Новое множество, содержащее все элементы из обоих множеств.</returns>
  UnorderedSet Union(const UnorderedSet& other) const {
    UnorderedSet result(*this);
    for (std::size_t i = 0; i < other.size_; ++i) {
      result.Add(other.data_[i]);
    }
    return result;
  }
  
  /// <summary>Удаляет из текущего множества элементы, присутствующие в другом множестве.</summary>
  /// <param name="other">Множество, элементы которого нужно удалить.</param>
  /// <returns>Новое множество, содержащее только элементы из текущего множества, отсутствующие в other.</returns>
  UnorderedSet Except(const UnorderedSet& other) const {
    UnorderedSet result;
    for (std::size_t i = 0; i < size_; ++i) {
      if (!other.Contains(data_[i])) {
        result.Add(data_[i]);
      }
    }
    return result;
  }
  
  /// <summary>Находит пересечение текущего множества с другим.</summary>
  /// <param name="other">Множество для пересечения.</param>
  /// <returns>Новое множество, содержащее только общие элементы.</returns>
  UnorderedSet Intersect(const UnorderedSet& other) const {
    UnorderedSet result;
    for (std::size_t i = 0; i < size_; ++i) {
      if (other.Contains(data_[i])) {
        result.Add(data_[i]);
      }
    }
    return result;
  }
  
  /// <summary>Преобразует множество в вектор для удобства вывода.</summary>
  /// <returns>Вектор, содержащий все элементы множества.</returns>
  std::vector<T> ToVector() const {
    return std::vector<T>(data_, data_ + size_);
  }
  
  /// <summary>Очищает множество, удаляя все элементы.</summary>
  void Clear() {
    delete[] data_;
    data_ = nullptr;
    size_ = 0;
    capacity_ = 0;
  }
  
  /// <summary>Проверяет, пусто ли множество.</summary>
  /// <returns>true, если множество пусто, иначе false.</returns>
  bool IsEmpty() const { return size_ == 0; }

 private:
  static constexpr std::size_t kNotFound = static_cast<std::size_t>(-1);
  static constexpr std::size_t kInitialCapacity = 4;
  
  T* data_;
  std::size_t size_;
  std::size_t capacity_;
  
  /// <summary>Обменивает содержимое текущего объекта с другим.</summary>
  /// <param name="other">Объект для обмена.</param>
  void Swap(UnorderedSet& other) noexcept {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
  }
  
  /// <summary>Находит индекс элемента в массиве.</summary>
  /// <param name="value">Элемент для поиска.</param>
  /// <returns>Индекс элемента или kNotFound, если элемент не найден.</returns>
  std::size_t Find(const T& value) const {
    for (std::size_t i = 0; i < size_; ++i) {
      if (data_[i] == value) {
        return i;
      }
    }
    return kNotFound;
  }
  
  /// <summary>Обеспечивает минимальную емкость массива.</summary>
  /// <param name="min_capacity">Минимальная требуемая емкость.</param>
  void EnsureCapacity(std::size_t min_capacity) {
    if (capacity_ >= min_capacity) return;
    
    std::size_t new_capacity = capacity_ == 0 ? kInitialCapacity : capacity_ * 2;
    while (new_capacity < min_capacity) new_capacity *= 2;
    
    T* new_data = new T[new_capacity];
    for (std::size_t i = 0; i < size_; ++i) {
      new_data[i] = data_[i];
    }
    
    delete[] data_;
    data_ = new_data;
    capacity_ = new_capacity;
  }
};

#endif  // UNORDERED_SET_H_