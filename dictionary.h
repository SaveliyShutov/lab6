#ifndef DICTIONARY_H_
#define DICTIONARY_H_

#include <cstddef>
#include <stdexcept>

/// <summary>Простейший словарь ключ-значение на основе двух массивов.</summary>
/// <typeparam name="K">Тип ключа.</typeparam>
/// <typeparam name="V">Тип значения.</typeparam>
template <typename K, typename V>
class Dictionary {
 public:
  /// <summary>Создаёт пустой словарь.</summary>
  Dictionary() : keys_(nullptr), values_(nullptr), size_(0), capacity_(0) {}

  /// <summary>Копирующий конструктор.</summary>
  /// <param name="other">Словарь для копирования.</param>
  Dictionary(const Dictionary& other) : keys_(nullptr), values_(nullptr), size_(0), capacity_(0) {
    EnsureCapacity(other.size_);
    for (std::size_t i = 0; i < other.size_; ++i) {
      keys_[i] = other.keys_[i];
      values_[i] = other.values_[i];
    }
    size_ = other.size_;
  }

  /// <summary>Оператор присваивания копированием.</summary>
  /// <param name="other">Словарь для копирования.</param>
  /// <returns>Ссылка на текущий объект.</returns>
  Dictionary& operator=(const Dictionary& other) {
    if (this != &other) {
      delete[] keys_; delete[] values_;
      keys_ = values_ = nullptr;
      size_ = capacity_ = 0;
      EnsureCapacity(other.size_);
      for (std::size_t i = 0; i < other.size_; ++i) {
        keys_[i] = other.keys_[i];
        values_[i] = other.values_[i];
      }
      size_ = other.size_;
    }
    return *this;
  }

  /// <summary>Перемещающий конструктор.</summary>
  /// <param name="other">Словарь для перемещения.</param>
  Dictionary(Dictionary&& other) noexcept
    : keys_(other.keys_), values_(other.values_), size_(other.size_), capacity_(other.capacity_) {
    other.keys_ = other.values_ = nullptr;
    other.size_ = other.capacity_ = 0;
  }

  /// <summary>Оператор присваивания перемещением.</summary>
  /// <param name="other">Словарь для перемещения.</param>
  /// <returns>Ссылка на текущий объект.</returns>
  Dictionary& operator=(Dictionary&& other) noexcept {
    if (this != &other) {
      delete[] keys_; delete[] values_;
      keys_ = other.keys_;
      values_ = other.values_;
      size_ = other.size_;
      capacity_ = other.capacity_;
      other.keys_ = other.values_ = nullptr;
      other.size_ = other.capacity_ = 0;
    }
    return *this;
  }

  /// <summary>Деструктор.</summary>
  ~Dictionary() { delete[] keys_; delete[] values_; }

  /// <summary>Проверяет наличие ключа в словаре.</summary>
  /// <param name="k">Ключ для поиска.</param>
  /// <returns>true если ключ найден, иначе false.</returns>
  bool ContainsKey(const K& k) const { return IndexOf(k) != npos; }

  /// <summary>Добавляет пару ключ-значение в словарь.</summary>
  /// <param name="k">Ключ для добавления.</param>
  /// <param name="v">Значение для добавления.</param>
  /// <remarks>Если ключ уже существует, ничего не происходит.</remarks>
  void Add(const K& k, const V& v) {
    if (ContainsKey(k)) return;
    EnsureCapacity(size_ + 1);
    keys_[size_] = k;
    values_[size_] = v;
    ++size_;
  }

  /// <summary>Удаляет ключ и соответствующее значение из словаря.</summary>
  /// <param name="k">Ключ для удаления.</param>
  /// <returns>true если ключ был удалён, иначе false.</returns>
  bool Remove(const K& k) {
    std::size_t idx = IndexOf(k);
    if (idx == npos) return false;
    for (std::size_t i = idx; i + 1 < size_; ++i) {
      keys_[i] = keys_[i + 1];
      values_[i] = values_[i + 1];
    }
    --size_;
    return true;
  }

  /// <summary>Получает значение по ключу.</summary>
  /// <param name="k">Ключ для поиска.</param>
  /// <returns>Значение, соответствующее ключу.</returns>
  /// <exception cref="std::out_of_range">Выбрасывается, если ключ не найден.</exception>
  V Get(const K& k) const {
    std::size_t idx = IndexOf(k);
    if (idx == npos) throw std::out_of_range("Key not found");
    return values_[idx];
  }

  /// <summary>Возвращает количество пар ключ-значение в словаре.</summary>
  /// <returns>Количество пар.</returns>
  std::size_t Size() const { return size_; }

 private:
  K* keys_;
  V* values_;
  std::size_t size_;
  std::size_t capacity_;
  static constexpr std::size_t npos = static_cast<std::size_t>(-1);
  
  /// <summary>Увеличивает вместимость внутренних массивов при необходимости.</summary>
  /// <param name="mincap">Минимальная требуемая вместимость.</param>
  void EnsureCapacity(std::size_t mincap) {
    if (capacity_ >= mincap) return;
    std::size_t newcap = capacity_ == 0 ? 4 : capacity_ * 2;
    while (newcap < mincap) newcap *= 2;
    K* nkeys = new K[newcap];
    V* nvals = new V[newcap];
    for (std::size_t i = 0; i < size_; ++i) {
      nkeys[i] = keys_[i];
      nvals[i] = values_[i];
    }
    delete[] keys_; delete[] values_;
    keys_ = nkeys;
    values_ = nvals;
    capacity_ = newcap;
  }
  
  /// <summary>Находит индекс ключа в массиве.</summary>
  /// <param name="k">Ключ для поиска.</param>
  /// <returns>Индекс ключа или npos, если ключ не найден.</returns>
  std::size_t IndexOf(const K& k) const {
    for (std::size_t i = 0; i < size_; ++i)
      if (keys_[i] == k) return i;
    return npos;
  }
};

#endif