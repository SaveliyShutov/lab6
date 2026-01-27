# **Отчет по лабораторной работе №6**

## Содержание
1. [Общая структура проекта](#общая-структура-проекта)
2. [Задание 1: Реализация класса UnorderedSet](#задание-1-реализация-класса-unorderedset)
3. [Задание 2: Решение задачи анализа прочитанных книг](#задание-2-решение-задачи-анализа-прочитанных-книг)
4. [Тестирование и результаты](#тестирование-и-результаты)

---

## Общая структура проекта

### Основные классы и структуры

#### Шаблонный класс UnorderedSet
```cpp
template <typename T>
class UnorderedSet {
    // Поля:
    T* data_;                    // Динамический массив для хранения элементов
    std::size_t size_;           // Текущее количество элементов
    std::size_t capacity_;       // Максимальная вместимость массива
};
```

#### Класс BookAnalyzer
```cpp
class BookAnalyzer {
    // Поля:
    UnorderedSet<std::string> all_books_;        // Все существующие книги
    std::vector<UnorderedSet<std::string>> readers_books_; // Книги каждого читателя
    UnorderedSet<std::string> books_read_by_all_;          // Книги, прочитанные всеми
    UnorderedSet<std::string> books_read_by_some_;         // Книги, прочитанные некоторыми
    UnorderedSet<std::string> books_read_by_none_;         // Книги, которые никто не читал
};
```

#### Вспомогательные функции (utils.h)
```cpp
std::string Trim(const std::string& s);                    // Удаление пробелов
std::vector<std::string> Split(const std::string& s, char delimiter); // Разделение строки
void PrintSet(const UnorderedSet<std::string>& s, std::ostream& out); // Вывод множества
```

---

## Задание 1: Реализация класса UnorderedSet

### 1) Постановка задачи
**Цель:** Реализовать класс, который реализует функционал неупорядоченного списка с уникальными элементами. Класс должен использовать массив в качестве поля и поддерживать следующие операции:
- Добавление элементов (игнорирование дубликатов)
- Удаление элементов
- Проверка наличия элемента
- Объединение двух множеств (Union)
- Разность множеств (Except)
- Пересечение множеств (Intersect)

**Требования:**
- Нет доступа к элементам по индексу
- Только уникальные элементы
- Автоматическое управление памятью

### 2) Ход решения

#### Конструкторы и деструктор
**Конструктор по умолчанию:**
```cpp
UnorderedSet() : data_(nullptr), size_(0), capacity_(0) {}
```
Инициализирует пустое множество с нулевой емкостью.

**Копирующий конструктор:**
```cpp
UnorderedSet(const UnorderedSet& other) 
    : data_(nullptr), size_(0), capacity_(0) {
    EnsureCapacity(other.size_);
    for (std::size_t i = 0; i < other.size_; ++i) {
        data_[i] = other.data_[i];
    }
    size_ = other.size_;
}
```
Создает глубокую копию множества, выделяя новый массив.

**Перемещающий конструктор:**
```cpp
UnorderedSet(UnorderedSet&& other) noexcept
    : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}
```
"Крадет" ресурсы временного объекта для оптимизации.

**Деструктор:**
```cpp
~UnorderedSet() { delete[] data_; }
```
Освобождает выделенную память.

#### Основные методы

**Добавление элемента (Add):**
```cpp
void Add(const T& value) {
    if (Contains(value)) return;        // Игнорирование дубликатов
    EnsureCapacity(size_ + 1);          // Проверка емкости
    data_[size_++] = value;             // Добавление в конец
}
```
- Сложность: O(n) (поиск) + O(1) амортизированное (добавление)

**Удаление элемента (Remove):**
```cpp
bool Remove(const T& value) {
    std::size_t index = Find(value);
    if (index == kNotFound) return false;
    
    for (std::size_t i = index; i + 1 < size_; ++i) {
        data_[i] = data_[i + 1];        // Сдвиг элементов
    }
    --size_;
    return true;
}
```
- Сложность: O(n) (поиск + сдвиг)
- Сохраняет порядок элементов

**Проверка наличия (Contains):**
```cpp
bool Contains(const T& value) const {
    return Find(value) != kNotFound;
}
```
- Сложность: O(n) (линейный поиск)

#### Операции над множествами

**Объединение (Union):**
```cpp
UnorderedSet Union(const UnorderedSet& other) const {
    UnorderedSet result(*this);
    for (std::size_t i = 0; i < other.size_; ++i) {
        result.Add(other.data_[i]);     // Add игнорирует дубликаты
    }
    return result;
}
```
- Сложность: O(n*m) где n, m - размеры множеств

**Пересечение (Intersect):**
```cpp
UnorderedSet Intersect(const UnorderedSet& other) const {
    UnorderedSet result;
    for (std::size_t i = 0; i < size_; ++i) {
        if (other.Contains(data_[i])) {
            result.Add(data_[i]);
        }
    }
    return result;
}
```
- Сложность: O(n*m)

**Разность (Except):**
```cpp
UnorderedSet Except(const UnorderedSet& other) const {
    UnorderedSet result;
    for (std::size_t i = 0; i < size_; ++i) {
        if (!other.Contains(data_[i])) {
            result.Add(data_[i]);
        }
    }
    return result;
}
```
- Сложность: O(n*m)

### 3) Ключевые особенности реализации

**Управление памятью:**
```cpp
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
```
- Стратегия удвоения емкости
- Амортизированная сложность O(1) для добавления
- Начальная емкость: 4 элемента

**Поиск элемента:**
```cpp
std::size_t Find(const T& value) const {
    for (std::size_t i = 0; i < size_; ++i) {
        if (data_[i] == value) {
            return i;
        }
    }
    return kNotFound;
}
```
- Линейный поиск
- Использует оператор == типа T

---

## Задание 2: Решение задачи анализа прочитанных книг

### 1) Постановка задачи
**Цель:** Используя реализованный класс UnorderedSet, решить задачу анализа прочитанных книг:
- Определить книги, которые прочли все читатели
- Определить книги, которые прочли некоторые читатели (но не все)
- Определить книги, которые никто не прочитал

**Входные данные:**
- Текстовый файл input.txt со списком книг и читателей

**Выходные данные:**
- Консольный вывод результатов анализа
- Файл output.txt с сохраненными результатами

### 2) Ход решения

#### Структура входного файла
```
Название книги 1
Название книги 2
Название книги 3
<пустая строка>
Книга1;Книга2;Книга3    ← Читатель 1
Книга2;Книга4           ← Читатель 2
Книга1;Книга3;Книга5    ← Читатель 3
```

#### Класс BookAnalyzer

**Чтение данных (ReadData):**
```cpp
bool ReadData(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;
    
    std::string line;
    bool reading_books = true;
    
    while (std::getline(file, line)) {
        std::string trimmed = Trim(line);
        
        if (trimmed.empty()) {
            reading_books = false;
            continue;
        }
        
        if (reading_books) {
            all_books_.Add(trimmed);           // Добавление в общий каталог
        } else {
            UnorderedSet<std::string> reader_books;
            std::vector<std::string> books = Split(trimmed, ';');
            
            for (const auto& book : books) {
                reader_books.Add(book);        // Книги текущего читателя
                all_books_.Add(book);          // Добавление в общий каталог
            }
            
            readers_books_.push_back(std::move(reader_books));
        }
    }
    
    return true;
}
```

**Анализ данных (Analyze):**
```cpp
void Analyze() {
    if (readers_books_.empty()) return;
    
    // Книги, прочитанные всеми
    books_read_by_all_ = readers_books_[0];
    for (std::size_t i = 1; i < readers_books_.size(); ++i) {
        books_read_by_all_ = books_read_by_all_.Intersect(readers_books_[i]);
    }
    
    // Книги, прочитанные хотя бы одним
    UnorderedSet<std::string> books_read_by_someone_ = readers_books_[0];
    for (std::size_t i = 1; i < readers_books_.size(); ++i) {
        books_read_by_someone_ = books_read_by_someone_.Union(readers_books_[i]);
    }
    
    // Книги, прочитанные некоторыми (но не всеми)
    books_read_by_some_ = books_read_by_someone_.Except(books_read_by_all_);
    
    // Книги, которые никто не прочитал
    books_read_by_none_ = all_books_.Except(books_read_by_someone_);
}
```

**Математическая основа:**
```
A = все книги
R₁, R₂, ..., Rₙ = книги каждого читателя

Все читали: R₁ ∩ R₂ ∩ ... ∩ Rₙ
Хотя бы один читал: R₁ ∪ R₂ ∪ ... ∪ Rₙ
Некоторые читали: (R₁ ∪ R₂ ∪ ... ∪ Rₙ) \ (R₁ ∩ R₂ ∩ ... ∩ Rₙ)
Никто не читал: A \ (R₁ ∪ R₂ ∪ ... ∪ Rₙ)
```

### 3) Вывод результатов

**Консольный вывод (PrintResults):**
```cpp
void PrintResults() const {
    std::cout << "Всего книг в каталоге: " << all_books_.Size() << std::endl;
    std::cout << "Количество читателей: " << readers_books_.size() << "\n" << std::endl;
    
    PrintSet("Книги, прочитанные ВСЕМИ читателями:", books_read_by_all_);
    PrintSet("Книги, прочитанные НЕКОТОРЫМИ читателями (но не всеми):", books_read_by_some_);
    PrintSet("Книги, которые НИКТО не прочитал:", books_read_by_none_);
}
```

**Сохранение в файл (SaveResults):**
```cpp
void SaveResults(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) return;
    
    file << "Всего книг в каталоге: " << all_books_.Size() << "\n";
    file << "Количество читателей: " << readers_books_.size() << "\n\n";
    
    SaveSetToFile(file, "Книги, прочитанные ВСЕМИ читателями:", books_read_by_all_);
    SaveSetToFile(file, "Книги, прочитанные НЕКОТОРЫМИ читателями (но не всеми):", books_read_by_some_);
    SaveSetToFile(file, "Книги, которые НИКТО не прочитал:", books_read_by_none_);
    
    file.close();
}
```

### 4) Вспомогательные функции

**Обработка строк (utils.h):**
```cpp
inline std::string Trim(const std::string& s) {
    std::size_t start = 0;
    while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start]))) {
        ++start;
    }
    
    std::size_t end = s.size();
    while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1]))) {
        --end;
    }
    
    return s.substr(start, end - start);
}

inline std::vector<std::string> Split(const std::string& s, char delimiter) {
    std::vector<std::string> result;
    std::istringstream iss(s);
    std::string token;
    
    while (std::getline(iss, token, delimiter)) {
        std::string trimmed = Trim(token);
        if (!trimmed.empty()) {
            result.push_back(trimmed);
        }
    }
    
    return result;
}
```

---

## Тестирование и результаты

### Тест 1: Базовый сценарий
**Входные данные (input.txt):**
```
Гарри Поттер и философский камень
Властелин колец
1984
Мастер и Маргарита
Преступление и наказание

Гарри Поттер и философский камень;Властелин колец;1984
Гарри Поттер и философский камень;Мастер и Маргарита
Властелин колец;1984;Мастер и Маргарита
```

**Результаты:**
```
Всего книг в каталоге: 5
Количество читателей: 3

Книги, прочитанные ВСЕМИ читателями:
Количество: 0
  (нет книг)

Книги, прочитанные НЕКОТОРЫМИ читателями (но не всеми):
Количество: 4
  • Гарри Поттер и философский камень
  • Властелин колец
  • 1984
  • Мастер и Маргарита

Книги, которые НИКТО не прочитал:
Количество: 1
  • Преступление и наказание
```

### Тест 2: Все читатели прочитали общие книги
**Входные данные:**
```
Книга1
Книга2
Книга3

Книга1;Книга2
Книга1;Книга3
Книга2;Книга3
Книга1;Книга2;Книга3
```

**Результаты:**
```
Всего книг в каталоге: 3
Количество читателей: 4

Книги, прочитанные ВСЕМИ читателями:
Количество: 0
  (нет книг)

Книги, прочитанные НЕКОТОРЫМИ читателями (но не всеми):
Количество: 3
  • Книга1
  • Книга2
  • Книга3

Книги, которые НИКТО не прочитал:
Количество: 0
  (нет книг)
```

### Тест 3: Демонстрация работы UnorderedSet
**Код демонстрации:**
```cpp
UnorderedSet<int> set1;
set1.Add(1);
set1.Add(2);
set1.Add(3);
set1.Add(2);  // Дубликат - не добавится

UnorderedSet<int> set2;
set2.Add(3);
set2.Add(4);
set2.Add(5);

// Операции над множествами
auto union_set = set1.Union(set2);        // {1, 2, 3, 4, 5}
auto intersect_set = set1.Intersect(set2); // {3}
auto except_set = set1.Except(set2);      // {1, 2}
```

**Вывод программы:**
```
Добавление элементов в set1:
set1 содержит: 1 2 3 
Размер set1: 3

Добавление элементов в set2:
set2 содержит: 3 4 5 
Размер set2: 3

Проверка наличия элементов:
set1.Contains(2): true
set1.Contains(5): false

Операции над множествами:
Объединение (Union): 1 2 3 4 5 
Пересечение (Intersect): 3 
Разность (Except): 1 2 

Удаление элемента 3 из set1:
set1 после удаления: 1 2 
Размер set1: 2
```

### Обработка ошибок
Все методы класса BookAnalyzer используют проверку открытия файлов:
```cpp
std::ifstream file(filename);
if (!file.is_open()) {
    std::cerr << "Ошибка: не удалось открыть файл " << filename << std::endl;
    return false;
}
```

Класс UnorderedSet корректно обрабатывает граничные случаи:
- Добавление в пустое множество
- Удаление из пустого множества
- Удаление несуществующего элемента
- Операции с пустыми множествами

### Производительность
**Сложность операций:**
- Добавление: O(n) (поиск) + O(1) амортизированное
- Удаление: O(n)
- Поиск: O(n)
- Операции множеств: O(n*m)

**Оптимизации:**
- Стратегия удвоения емкости массива
- Использование перемещения вместо копирования
- Константные ссылки для параметров

---