#ifndef UTILS_H_
#define UTILS_H_

#include <string>
#include <vector>

/// <summary>Удаляет пробельные символы с начала и конца строки.</summary>
/// <param name="str">Входная строка.</param>
/// <returns>Обрезанная строка без начальных и конечных пробелов.</returns>
std::string Trim(const std::string& str);

/// <summary>Разделяет строку на подстроки по указанному разделителю.</summary>
/// <param name="str">Входная строка для разделения.</param>
/// <param name="delimiter">Разделитель для использования.</param>
/// <returns>Вектор строк, полученных разделением входной строки.</returns>
std::vector<std::string> Split(const std::string& str, char delimiter);

#endif // UTILS_H_