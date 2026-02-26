#ifndef UNORDERED_SET_H_
#define UNORDERED_SET_H_

#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <vector>

/// <summary>Класс, реализующий функционал неупорядоченного списка с уникальными
/// элементами.</summary> <typeparam name="T">Тип элементов, хранящихся в
/// множестве.</typeparam> <remarks>Доступ к элементам по индексу отсутствует.
/// Дубликаты игнорируются при добавлении.</remarks>
template <typename T> class UnorderedSet {
public:
  /// <summary>Конструктор по умолчанию. Создает пустое множество.</summary>
  UnorderedSet();

  /// <summary>Деструктор. Освобождает выделенную память.</summary>
  ~UnorderedSet();

  /// <summary>Копирующий конструктор.</summary>
  /// <param name="other">Множество для копирования.</param>
  UnorderedSet(const UnorderedSet &other);

  /// <summary>Оператор присваивания копированием.</summary>
  /// <param name="other">Множество для копирования.</param>
  /// <returns>Ссылка на текущий объект.</returns>
  UnorderedSet &operator=(const UnorderedSet &other);

  /// <summary>Перемещающий конструктор.</summary>
  /// <param name="other">Множество для перемещения.</param>
  UnorderedSet(UnorderedSet &&other) noexcept;

  /// <summary>Оператор присваивания перемещением.</summary>
  /// <param name="other">Множество для перемещения.</param>
  /// <returns>Ссылка на текущий объект.</returns>
  UnorderedSet &operator=(UnorderedSet &&other) noexcept;

  /// <summary>Возвращает количество элементов в множестве.</summary>
  /// <returns>Количество элементов.</returns>
  std::size_t Size() const;

  /// <summary>Проверяет наличие элемента в множестве.</summary>
  /// <param name="value">Элемент для проверки.</param>
  /// <returns>true, если элемент найден, иначе false.</returns>
  bool Contains(const T &value) const;

  /// <summary>Добавляет элемент в множество.</summary>
  /// <param name="value">Элемент для добавления.</param>
  /// <remarks>Если элемент уже существует, добавление не происходит.</remarks>
  void Add(const T &value);

  /// <summary>Удаляет элемент из множества.</summary>
  /// <param name="value">Элемент для удаления.</param>
  /// <returns>true, если элемент был удален, иначе false.</returns>
  bool Remove(const T &value);

  /// <summary>Объединяет текущее множество с другим.</summary>
  /// <param name="other">Множество для объединения.</param>
  /// <returns>Новое множество, содержащее все элементы из обоих
  /// множеств.</returns>
  UnorderedSet Union(const UnorderedSet &other) const;

  /// <summary>Удаляет из текущего множества элементы, присутствующие в другом
  /// множестве.</summary> <param name="other">Множество, элементы которого
  /// нужно удалить.</param> <returns>Новое множество, содержащее только
  /// элементы из текущего множества, отсутствующие в other.</returns>
  UnorderedSet Except(const UnorderedSet &other) const;

  /// <summary>Находит пересечение текущего множества с другим.</summary>
  /// <param name="other">Множество для пересечения.</param>
  /// <returns>Новое множество, содержащее только общие элементы.</returns>
  UnorderedSet Intersect(const UnorderedSet &other) const;

  /// <summary>Преобразует множество в вектор для удобства вывода.</summary>
  /// <returns>Вектор, содержащий все элементы множества.</returns>
  std::vector<T> ToVector() const;

  /// <summary>Очищает множество, удаляя все элементы.</summary>
  void Clear();

  /// <summary>Проверяет, пусто ли множество.</summary>
  /// <returns>true, если множество пусто, иначе false.</returns>
  bool IsEmpty() const;

private:
  static constexpr std::size_t kNotFound = static_cast<std::size_t>(-1);
  static constexpr std::size_t kInitialCapacity = 4;

  T *data_;
  std::size_t size_;
  std::size_t capacity_;

  /// <summary>Обменивает содержимое текущего объекта с другим.</summary>
  /// <param name="other">Объект для обмена.</param>
  void Swap(UnorderedSet &other) noexcept;

  /// <summary>Находит индекс элемента в массиве.</summary>
  /// <param name="value">Элемент для поиска.</param>
  /// <returns>Индекс элемента или kNotFound, если элемент не найден.</returns>
  std::size_t Find(const T &value) const;

  /// <summary>Обеспечивает минимальную емкость массива.</summary>
  /// <param name="min_capacity">Минимальная требуемая емкость.</param>
  void EnsureCapacity(std::size_t min_capacity);
};

#endif // UNORDERED_SET_H_