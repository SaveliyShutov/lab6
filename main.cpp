#include "dictionary.h"
#include "unordered_set.h"
#include "utils.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

/// <summary>Класс для анализа прочитанных книг читателями.</summary>
/// <remarks>
/// Использует класс UnorderedSet для хранения книг и выполнения операций над
/// множествами. Определяет три категории книг: прочитанные всеми, прочитанные
/// некоторыми, не прочитанные никем.
/// </remarks>
class BookAnalyzer {
public:
  /// <summary>Читает данные из файла и заполняет внутренние
  /// структуры.</summary> <param name="filename">Имя файла с данными.</param>
  /// <returns>true, если данные успешно прочитаны, иначе false.</returns>
  bool ReadData(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
      std::cerr << "Ошибка: не удалось открыть файл " << filename << std::endl;
      return false;
    }

    std::string line;
    bool reading_books = true;

    while (std::getline(file, line)) {
      std::string trimmed = Trim(line);

      if (trimmed.empty()) {
        reading_books = false;
        continue;
      }

      if (reading_books) {
        all_books_.Add(trimmed);
      } else {
        UnorderedSet<std::string> reader_books;
        std::vector<std::string> books = Split(trimmed, ';');

        for (const auto &book : books) {
          reader_books.Add(book);
          all_books_.Add(book);
        }

        readers_books_.push_back(std::move(reader_books));
      }
    }

    file.close();
    return true;
  }

  /// <summary>Выполняет анализ прочитанных книг.</summary>
  /// <remarks>
  /// Определяет три категории книг:
  /// 1. Книги, прочитанные всеми читателями
  /// 2. Книги, прочитанные некоторыми читателями (но не всеми)
  /// 3. Книги, которые никто не прочитал
  /// </remarks>
  void Analyze() {
    if (readers_books_.empty()) {
      std::cout << "Нет данных о читателях" << std::endl;
      return;
    }

    books_read_by_all_ = readers_books_[0];
    for (std::size_t i = 1; i < readers_books_.size(); ++i) {
      books_read_by_all_ = books_read_by_all_.Intersect(readers_books_[i]);
    }

    books_read_by_someone_ = readers_books_[0];
    for (std::size_t i = 1; i < readers_books_.size(); ++i) {
      books_read_by_someone_ = books_read_by_someone_.Union(readers_books_[i]);
    }

    books_read_by_some_ = books_read_by_someone_.Except(books_read_by_all_);

    books_read_by_none_ = all_books_.Except(books_read_by_someone_);
  }

  /// <summary>Выводит результаты анализа в консоль.</summary>
  void PrintResults() const {
    std::cout << "Всего книг в каталоге: " << all_books_.Size() << std::endl;
    std::cout << "Количество читателей: " << readers_books_.size() << "\n"
              << std::endl;

    PrintSet("Книги, прочитанные ВСЕМИ читателями:", books_read_by_all_);
    PrintSet("Книги, прочитанные НЕКОТОРЫМИ читателями (но не всеми):",
             books_read_by_some_);
    PrintSet("Книги, которые НИКТО не прочитал:", books_read_by_none_);
  }

  /// <summary>Сохраняет результаты анализа в файл.</summary>
  /// <param name="filename">Имя файла для сохранения результатов.</param>
  void SaveResults(const std::string &filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
      std::cerr << "Ошибка: не удалось создать файл " << filename << std::endl;
      return;
    }

    file << "Всего книг в каталоге: " << all_books_.Size() << "\n";
    file << "Количество читателей: " << readers_books_.size() << "\n\n";

    SaveSetToFile(file,
                  "Книги, прочитанные ВСЕМИ читателями:", books_read_by_all_);
    SaveSetToFile(file,
                  "Книги, прочитанные НЕКОТОРЫМИ читателями (но не всеми):",
                  books_read_by_some_);
    SaveSetToFile(file,
                  "Книги, которые НИКТО не прочитал:", books_read_by_none_);

    file.close();
    std::cout << "\nРезультаты сохранены в файл: " << filename << std::endl;
  }

private:
  UnorderedSet<std::string> all_books_;
  std::vector<UnorderedSet<std::string>> readers_books_;
  UnorderedSet<std::string> books_read_by_all_;
  UnorderedSet<std::string> books_read_by_some_;
  UnorderedSet<std::string> books_read_by_none_;
  UnorderedSet<std::string> books_read_by_someone_;

  /// <summary>Выводит множество книг с заголовком в консоль.</summary>
  /// <param name="title">Заголовок для вывода.</param>
  /// <param name="set">Множество книг для вывода.</param>
  void PrintSet(const std::string &title,
                const UnorderedSet<std::string> &set) const {
    std::cout << title << std::endl;
    std::cout << "Количество: " << set.Size() << std::endl;

    auto books = set.ToVector();
    if (books.empty()) {
      std::cout << "  (нет книг)\n" << std::endl;
    } else {
      for (const auto &book : books) {
        std::cout << "  • " << book << std::endl;
      }
      std::cout << std::endl;
    }
  }

  /// <summary>Сохраняет множество книг с заголовком в файл.</summary>
  /// <param name="file">Файловый поток для записи.</param>
  /// <param name="title">Заголовок для сохранения.</param>
  /// <param name="set">Множество книг для сохранения.</param>
  void SaveSetToFile(std::ofstream &file, const std::string &title,
                     const UnorderedSet<std::string> &set) const {
    file << title << "\n";
    file << "Количество: " << set.Size() << "\n";

    auto books = set.ToVector();
    if (books.empty()) {
      file << "  (нет книг)\n\n";
    } else {
      for (const auto &book : books) {
        file << "  • " << book << "\n";
      }
      file << "\n";
    }
  }
};

/// <summary>Структура для хранения информации о спортсмене.</summary>
/// <remarks>Используется в задаче многоборья.</remarks>
struct Athlete {
  std::string surname;
  std::string name;
  long long sum;
  std::size_t input_index;
};

/// <summary>Читает входные данные многоборья из файла, сортирует спортсменов и
/// сохраняет результаты.</summary> <param name="infile">Имя входного файла
/// (например, input2.txt).</param> <param name="outfile">Имя выходного файла
/// (например, output2.txt).</param> <returns>true, если успешно, иначе
/// false.</returns>
bool RunCompetition(const std::string &infile, const std::string &outfile) {
  std::ifstream in(infile);
  if (!in.is_open()) {
    std::cerr << "Не удалось открыть файл " << infile << std::endl;
    return false;
  }

  int N = 0, M = 0;
  if (!(in >> N)) {
    std::cerr << "Ошибка чтения N из " << infile << std::endl;
    return false;
  }
  if (!(in >> M)) {
    std::cerr << "Ошибка чтения M из " << infile << std::endl;
    return false;
  }
  if (N < 0 || N >= 1000) {
    std::cerr << "N вне допустимого диапазона (0..999)\n";
    return false;
  }
  if (M < 0) {
    std::cerr << "M некорректно\n";
    return false;
  }

  std::vector<Athlete> athletes;
  athletes.reserve(static_cast<std::size_t>(N));

  for (int i = 0; i < N; ++i) {
    std::string surname, name;
    if (!(in >> surname >> name)) {
      std::cerr << "Ошибка чтения Фамилии/Имени спортсмена на строке "
                << (i + 1) << std::endl;
      return false;
    }
    long long sum = 0;
    for (int j = 0; j < M; ++j) {
      long long x;
      if (!(in >> x)) {
        std::cerr << "Ошибка чтения баллов у " << surname << " " << name
                  << std::endl;
        return false;
      }
      sum += x;
    }
    athletes.push_back({surname, name, sum, static_cast<std::size_t>(i)});
  }

  // Сортировка по убыванию суммы, стабильная (сохранение порядка при равных
  // суммах).
  std::stable_sort(
      athletes.begin(), athletes.end(),
      [](const Athlete &a, const Athlete &b) { return a.sum > b.sum; });

  // Присвоение плотных мест: 1,1,2,...
  std::vector<int> rank(athletes.size());
  long long prev_sum = std::numeric_limits<long long>::min();
  int dense_rank = 0;
  for (std::size_t i = 0; i < athletes.size(); ++i) {
    if (athletes[i].sum != prev_sum) {
      ++dense_rank;
      prev_sum = athletes[i].sum;
    }
    rank[i] = dense_rank;
  }

  // Вывод в консоль
  std::cout << "\nРезультаты многоборья (из " << N << " спортсменов, " << M
            << " видов):\n";
  for (std::size_t i = 0; i < athletes.size(); ++i) {
    std::cout << athletes[i].surname << ' ' << athletes[i].name << ' '
              << athletes[i].sum << ' ' << rank[i] << '\n';
  }

  // Сохранение в outfile
  std::ofstream out(outfile);
  if (out.is_open()) {
    for (std::size_t i = 0; i < athletes.size(); ++i) {
      out << athletes[i].surname << ' ' << athletes[i].name << ' '
          << athletes[i].sum << ' ' << rank[i] << '\n';
    }
    out.close();
    std::cout << "\nРезультаты многоборья сохранены в файл: " << outfile
              << std::endl;
  } else {
    std::cerr << "Не удалось создать файл " << outfile
              << " для записи результатов\n";
  }

  return true;
}

/// <summary>Главная функция программы.</summary>
/// <returns>Код завершения программы: 0 - успешно, другие значения -
/// ошибка.</returns> <remarks> Выполняет два независимых сценария:
/// 1. Анализ прочитанных книг (читает input.txt, сохраняет output.txt)
/// 2. Задача многоборья (читает input2.txt, сохраняет output2.txt)
/// </remarks>
int main() {
  // 1) Анализ книг: input.txt -> output.txt
  BookAnalyzer analyzer;
  if (!analyzer.ReadData("input.txt")) {
    std::cerr << "Завершение: ошибка при чтении input.txt\n";
    // не прерываем — всё ещё хотим попытаться выполнить задачу многоборья
  } else {
    analyzer.Analyze();
    analyzer.PrintResults();
    analyzer.SaveResults("output.txt");
  }

  // 2) Многоборье: input2.txt -> output2.txt
  if (!RunCompetition("input2.txt", "output2.txt")) {
    std::cerr
        << "Ошибка при выполнении задачи многоборья. Проверьте input2.txt\n";
    return 1;
  }

  // Демонстрация работы UnorderedSet (пример из старого main)
  UnorderedSet<int> set1;
  UnorderedSet<int> set2;

  std::cout << "\nДобавление элементов в set1:" << std::endl;
  set1.Add(1);
  set1.Add(2);
  set1.Add(3);
  set1.Add(2);
  std::cout << "set1 содержит: ";
  for (int num : set1.ToVector()) {
    std::cout << num << " ";
  }
  std::cout << "\nРазмер set1: " << set1.Size() << std::endl;

  std::cout << "\nДобавление элементов в set2:" << std::endl;
  set2.Add(3);
  set2.Add(4);
  set2.Add(5);
  std::cout << "set2 содержит: ";
  for (int num : set2.ToVector()) {
    std::cout << num << " ";
  }
  std::cout << "\nРазмер set2: " << set2.Size() << std::endl;

  std::cout << "\nПроверка наличия элементов:" << std::endl;
  std::cout << "set1.Contains(2): " << (set1.Contains(2) ? "true" : "false")
            << std::endl;
  std::cout << "set1.Contains(5): " << (set1.Contains(5) ? "true" : "false")
            << std::endl;

  std::cout << "\nОперации над множествами:" << std::endl;

  auto union_set = set1.Union(set2);
  std::cout << "Объединение (Union): ";
  for (int num : union_set.ToVector()) {
    std::cout << num << " ";
  }
  std::cout << std::endl;

  auto intersect_set = set1.Intersect(set2);
  std::cout << "Пересечение (Intersect): ";
  for (int num : intersect_set.ToVector()) {
    std::cout << num << " ";
  }
  std::cout << std::endl;

  auto except_set = set1.Except(set2);
  std::cout << "Разность (Except): ";
  for (int num : except_set.ToVector()) {
    std::cout << num << " ";
  }
  std::cout << std::endl;

  std::cout << "\nУдаление элемента 3 из set1:" << std::endl;
  set1.Remove(3);
  std::cout << "set1 после удаления: ";
  for (int num : set1.ToVector()) {
    std::cout << num << " ";
  }
  std::cout << "\nРазмер set1: " << set1.Size() << std::endl;

  return 0;
}