#ifndef DICTIONARY_H_
#define DICTIONARY_H_

#include <cstddef>
#include <vector>
#include <utility>

/// <summary>Простой универсальный словарь (ключ -> значение) без использования std::map / std::unordered_map.</summary>
/// <remarks>Хранит пары в динамическом массиве. Ключи уникальны.</remarks>
template <typename K, typename V>
class Dictionary {
 public:
  Dictionary();
  ~Dictionary();

  Dictionary(const Dictionary& other);
  Dictionary& operator=(const Dictionary& other);
  Dictionary(Dictionary&& other) noexcept;
  Dictionary& operator=(Dictionary&& other) noexcept;

  std::size_t Size() const;
  bool IsEmpty() const;

  /// <summary>Добавляет пару (key,value). Если ключ уже существует — обновляет значение.</summary>
  void Add(const K& key, const V& value);

  /// <summary>Удаляет элемент по ключу. Возвращает true, если удалено.</summary>
  bool Remove(const K& key);

  /// <summary>Проверяет наличие ключа.</summary>
  bool Contains(const K& key) const;

  /// <summary>Возвращает указатель на значение по ключу или nullptr, если нет.</summary>
  V* Get(const K& key);
  const V* Get(const K& key) const;

  /// <summary>Возвращает все пары в виде вектора (копии).</summary>
  std::vector<std::pair<K,V>> ToVector() const;

  void Clear();

 private:
  static constexpr std::size_t kInitialCapacity = 4;
  std::pair<K,V>* data_;
  std::size_t size_;
  std::size_t capacity_;

  void EnsureCapacity(std::size_t min_capacity);
  std::size_t FindIndex(const K& key) const;
  void Swap(Dictionary& other) noexcept;
};

#endif // DICTIONARY_H_