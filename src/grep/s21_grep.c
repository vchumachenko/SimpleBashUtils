#include "s21_grep.h"

int main(int argc, char *argv[]) {
  char patterns[SIZE][SIZE] = {
      0};  // Объявление и инициализация двумерного массива patterns
  int opt_ind = 0;  // Инициализация переменной opt_ind
  flags option = {0};  // Инициализация структуры option
  grep_values value = {0};  // Инициализация структуры value

  int file_location = 0;  // Инициализация переменной file_location
  opt_ind = parser(
      argc, argv, &option, &value,
      patterns);  // Вызов функции parser для определения значения opt_ind

  if (argc >= 3 && option.error_flag != 1 && opt_ind != argc) {
    file_location = find_pattern(
        opt_ind, argv, patterns);  // Вызов функции find_pattern для определения
                                   // значения file_location
    while (file_location < argc) {
      if (argv[file_location + 1] != NULL) {
        value.count_files = value.count_files + 1;
      }
      value.path = argv[file_location];  // Присвоение переменной path значения
                                         // argv[file_location]
      grep(value, option, patterns);  // Вызов функции grep с передачей
                                      // аргументов value, option и patterns
      file_location++;  // Увеличение значения file_location на 1
    }
  }
}

int find_pattern(int opt_ind, char *argv[], char patterns[SIZE][SIZE]) {
  int file_location = 0;  // Инициализация переменной file_location
  if (*patterns[0] == 0) {
    file_location =
        opt_ind +
        1;  // Присвоение переменной file_location значения opt_ind + 1
    strcpy(patterns[0],
           argv[optind]);  // Копирование значения argv[optind] в patterns[0]
  } else {
    file_location =
        opt_ind;  // Присвоение переменной file_location значения opt_ind
  }
  return file_location;  // Возврат значения file_location
}

int parser(int argc, char *argv[], flags *flag,
           grep_values *value,  // Функция parser принимает аргументы командной
                                // строки, флаги и шаблоны поиска
           char patterns[SIZE][SIZE]) {
  int opt = 0;
  opterr = 0;  // Отключаем вывод сообщений об ошибках

  while (
      (opt = getopt_long(argc, argv, "e:ivclnhsf:o", NULL, NULL)) !=
      -1) {  // Цикл обработки аргументов командной строки с помощью getopt_long
    switch (opt) {
      case 'e':  // Если задан флаг -e, то добавляем шаблон поиска в массив
        while (*patterns[value->count_pattern] != 0) {
          value->count_pattern = value->count_pattern + 1;
        }
        strcpy(patterns[value->count_pattern], optarg);
        value->count_pattern = value->count_pattern + 1;
        flag->e_flag = 1;
        break;
      case 'i':
        flag->i_flag = 1;
        break;
      case 'v':
        flag->v_flag = 1;
        break;
      case 'c':
        flag->c_flag = 1;
        break;
      case 'l':
        flag->l_flag = 1;
        break;
      case 'n':
        flag->n_flag = 1;
        break;
      case 'h':
        flag->h_flag = 1;
        break;
      case 's':
        flag->s_flag = 1;
        break;
      case 'f':
        while (*patterns[value->count_pattern] != 0) {
          value->count_pattern = value->count_pattern + 1;
        }
        f_flag(optarg, patterns,
               value);  //  Вызываем функцию f_flag для обработки файла
        flag->f_flag = 1;  // Устанавливаем флаг -f
        break;
      case 'o':  // Если задан флаг -o, то устанавливаем соответствующий флаг
        flag->o_flag = 1;
        break;
      case '?':  // Если введен некорректный аргумент, то устанавливаем флаг
                 // ошибки
        flag->error_flag = 1;
        break;
    }
  }
  return optind;  // Возвращаем индекс первого необработанного аргумента
}

void grep(grep_values value, flags flag, char pattern[SIZE][SIZE]) {
  FILE *filename;
  char string[SIZE] = " ";
  int count_lines = 0;          // Счетчик строк в файле
  int count_matched_lines = 0;  // Счетчик совпадающих строк

  int comp_flag1 = REG_EXTENDED;  // Флаг для регулярного выражения

  int stop = 0;  // Флаг для прекращения поиска

  int result_regex = 243;  // Результат выполнения регулярного выражения
  regex_t preg;

  if (flag.v_flag == 1 || flag.c_flag == 1 || flag.l_flag == 1)
    flag.o_flag =
        0;  // Если установлены флаги -v, -c или -l, то флаг -o неактивен

  if (access(value.path, F_OK) == 0) {  // Проверка доступности файла
    filename = fopen(value.path, "r");

    if (value.count_pattern == 0 && flag.e_flag == 0 && flag.f_flag == 0)
      value.count_pattern =
          1;  // Если нет заданных паттернов и не установлены флаги -e или -f,
              // то устанавливается значение 1 для count_pattern
    if (flag.i_flag == 1)
      comp_flag1 = REG_ICASE;  // Если установлен флаг -i, то устанавливается
                               // флаг REG_ICASE для регистронезависимого поиска

    while ((fgets(string, SIZE, filename)) && (stop == 0)) {
      int findline = 0;  // Флаг для обозначения нахождения строки
      count_lines++;  // Увеличиваем счетчик строк
      int pattern_no_match = 0;  // Объявление переменной pattern_no_match

      if (strchr(string, '\n') == NULL)
        strcat(string,
               "\n");  // Добавляем символ новой строки, если отсутствует

      for (int i = 0; i < value.count_pattern; i++) {
        int match = 0;  // Флаг для обозначения совпадения

        regcomp(&preg, pattern[i],
                comp_flag1 | REG_NEWLINE);  // Компиляция регулярного выражения
        result_regex =
            regexec(&preg, string, 0, NULL,
                    0);  // Выполнение регулярного выражения на строке

        if (result_regex == 0 && flag.v_flag == 0)
          match = 1;  // Если регулярное выражение совпало и не установлен флаг
                      // -v, то устанавливаем флаг совпадения

        if (result_regex == REG_NOMATCH && flag.v_flag == 1) {
          pattern_no_match++;  // Увеличиваем счетчик несовпадающих паттернов
          if (pattern_no_match == value.count_pattern)
            match = 1;  // Если все паттерны не совпали, то устанавливаем флаг
                        // совпадения
        }
        if (flag.l_flag == 1 && match == 1 && flag.c_flag == 0) {
          printf("%s\n", value.path);  // Если установлен флаг -l и есть
                                       // совпадение, выводим путь к файлу
          match = 0;
          stop = 1;  // Устанавливаем флаг прекращения поиска
        }
        if (flag.c_flag == 1 && match == 1) {
          count_matched_lines++;  // Увеличиваем счетчик совпадающих строк
          match = 0;
        }
        if (findline == 0 && match == 1) {
          printing_strings(value, flag, count_lines, string);  // Выводим строку
          findline++;
        }
        if (flag.o_flag == 1) {
          flag_o(string, preg);  // Выводим подстроки, соответствующие
                                 // регулярному выражению
        }
        regfree(&preg);  // Освобождаем память от скомпилированного регулярного
                         // выражения
      }
    }
    if (flag.c_flag == 1) {
      flag_c_l(
          value, flag,
          count_matched_lines);  // Выводим результат подсчета совпадающих строк
    }
    fclose(filename);
  } else if (flag.s_flag == 0) {
    fprintf(stderr, "No such file or directory: %s\n",
            value.path);  // Выводим сообщение об ошибке отсутствия файла
  }
}

void f_flag(char *path, char pattern[SIZE][SIZE], grep_values *value) {
  FILE *filename;
  int lenght = 0;
  if (access(path, F_OK) == 0) {
    filename = fopen(path, "r");
    while (!feof(filename)) {
      fgets(pattern[value->count_pattern], SIZE, filename);
      lenght = strlen(pattern[value->count_pattern]);

      if (pattern[value->count_pattern][0] != '\n' &&
          pattern[value->count_pattern][lenght - 1] == '\n') {
        pattern[value->count_pattern][lenght - 1] = '\0';
      }
      value->count_pattern = value->count_pattern + 1;
    }
    fclose(filename);
  } else {
    printf("error file");
  }
}

void flag_c_l(grep_values value, flags flag, int count_matched_lines) {
  if (value.count_files >= 1 && flag.h_flag == 0) {
    printf("%s:", value.path);
  }
  if (flag.l_flag == 0) {
    printf("%d\n", count_matched_lines);
  } else if (flag.l_flag == 1) {
    if (count_matched_lines > 0) {
      printf("1\n");
      printf("%s\n", value.path);
    } else {
      printf("0\n");
    }
  }
}

void printing_strings(grep_values value, flags flag, int count_lines,
                      char *string) {
  if (value.count_files >= 1 && flag.h_flag == 0) printf("%s:", value.path);
  if (flag.n_flag == 1) printf("%d:", count_lines);
  if (flag.o_flag == 0) printf("%s", string);
}

void flag_o(char *string, regex_t preg) {
  regmatch_t pmatch[1];
  while (regexec(&preg, string, 1, pmatch, 0) == 0) {
    for (int j = 0; j < pmatch->rm_eo; j++) {
      if (j >= pmatch->rm_so) {
        printf("%c", string[j]);
      }
      string[j] = 127;
    }
    printf("\n");
  }
}
