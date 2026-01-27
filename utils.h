#ifndef UTILS_H_
#define UTILS_H_

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

/// <summary>Удаляет пробельные символы с начала и конца строки.</summary>
/// <param name="s">Входная строка.</param>
/// <returns>Обрезанная строка без начальных и конечных пробелов.</returns>
inline std::string Trim(const std::string &s) {
  std::size_t start = 0;
  // Пропускаем пробельные символы в начале
  while (start < s.size() &&
         std::isspace(static_cast<unsigned char>(s[start]))) {
    ++start;
  }

  std::size_t end = s.size();
  // Пропускаем пробельные символы в конце
  while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1]))) {
    --end;
  }

  // Возвращаем подстроку без пробелов
  return s.substr(start, end - start);
}

/// <summary>Разделяет строку на подстроки по указанному разделителю.</summary>
/// <param name="s">Входная строка для разделения.</param>
/// <param name="delimiter">Разделитель для использования.</param>
/// <returns>Вектор строк, полученных разделением входной строки.</returns>
inline std::vector<std::string> Split(const std::string &s, char delimiter) {
  std::vector<std::string> result;
  std::istringstream iss(s);
  std::string token;

  // Читаем строку по разделителю
  while (std::getline(iss, token, delimiter)) {
    // Убираем пробелы вокруг каждой подстроки
    std::string trimmed = Trim(token);
    // Добавляем только непустые строки
    if (!trimmed.empty()) {
      result.push_back(trimmed);
    }
  }

  return result;
}

#endif // UTILS_H_