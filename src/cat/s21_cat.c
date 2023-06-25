#include "s21_cat.h"
int parser(int argc, char *argv[], flags *flag);
void cat(char *path, flags flag);

void cat_stdin() {  // с командной строки
  char buffer[BUFFER_SIZE];

  while (fgets(buffer, BUFFER_SIZE, stdin) != NULL) {
    fputs(buffer, stdout);
  }
}

int main(int argc, char *argv[]) {
  int number_of_files;
  flags option = {0};
  number_of_files = parser(argc, argv, &option);

  if (option.error_flag == 1) {  // Проверка флага ошибки
    fprintf(stderr, "illegal option");  // Вывод сообщения об ошибке в stderr
  } else if (number_of_files ==
             argc) {  // Если не указаны файлы, читаем из стандартного ввода
    cat_stdin(option);  // Вызов функции cat_stdin с передачей опций
  } else {
    while (number_of_files < argc) {  // Чтение файлов
      char *path;
      path = argv[number_of_files];
      cat(path, option);  // Вызов функции cat с передачей пути и опций
      number_of_files++;
    }
  }
  return 0;
}

int parser(int argc, char *argv[], flags *flag) {
  const struct option long_options[] = {
      // Массив с опциями командной строки
      {"number-nonblank", no_argument, NULL, 'b'},
      {"number", no_argument, NULL, 'n'},
      {"squeeze-blank", no_argument, NULL, 's'},
      {NULL, 0, NULL, 0}};

  int opt = 0;

  while ((opt = getopt_long(argc, argv, "+beEnstTv?", long_options, NULL)) !=
         -1) {  // Обработка опций командной строки
    switch (opt) {
      case 'b':
        flag->b_flag = 1;  // Установка флага b_flag
        break;
      case 'e':
        flag->e_flag = flag->v_flag = 1;  // Установка флагов e_flag и v_flag
        break;
      case 'E':
        flag->e_flag = 1;  // Установка флага e_flag
        break;
      case 'n':
        flag->n_flag = 1;  // Установка флага n_flag
        break;
      case 's':
        flag->s_flag = 1;  // Установка флага s_flag
        break;
      case 't':
        flag->t_flag = flag->v_flag = 1;  // Установка флагов t_flag и v_flag
        break;
      case 'T':
        flag->t_flag = 1;  // Установка флага t_flag
        break;
      case 'v':
        flag->v_flag = 1;  // Установка флага v_flag
        break;
      case '?':
        flag->error_flag = 1;  // Установка флага error_flag
        break;
    }
  }
  return optind;  // Возврат индекса следующего аргумента
}

void cat(char *path, flags flag) {
  if (access(path, F_OK) == 0) {  // Проверка наличия файла
    FILE *filename;
    filename = fopen(path, "r");  // Открытие файла для чтения

    char ch, prev;

    int current_line = 1;
    int squeeze = 0;
    for (prev = '\n'; (ch = getc(filename)) != EOF; prev = ch) {
      if (flag.s_flag == 1) {  // Если установлен флаг squeeze-blank
        if (ch == '\n' && prev == '\n') {  // Если текущий символ и предыдущий
                                           // символ - символы новой строки
          if (squeeze == 1) {
            continue;
          }
          squeeze++;
        } else
          squeeze = 0;
      }

      if (flag.b_flag == 1) {  // Если установлен флаг number-nonblank
        if (prev == '\n' &&
            ch != '\n') {  // Если предыдущий символ - символ новой строки, а
                           // текущий символ - не символ новой строки
          printf("%*d\t", 6, current_line);  // Вывод номера текущей строки
          current_line++;
        }
      }

      if (flag.n_flag == 1 && flag.b_flag == 0 &&
          prev == '\n') {  // Если установлен флаг number и не установлен флаг
                           // number-nonblank
        printf("%*d\t", 6, current_line);  // Вывод номера текущей строки
        current_line++;
      }

      if (flag.e_flag == 1 && ch == '\n') {  // Если установлен флаг e и текущий
                                             // символ - символ новой строки
        putchar('$');  // Вывод символа '$'
      }

      if (flag.t_flag == 1 && ch == '\t') {  // Если установлен флаг t и текущий
                                             // символ - символ табуляции
        printf("^");  // Вывод символа '^'
        ch = 'I';     // Замена символа табуляции на 'I'
      }

      if (flag.v_flag == 1) {  // Если установлен флаг v
        if ((ch >= 0 && ch <= 31) && ch != '\t' &&
            ch != '\n') {  // Если текущий символ - управляющий символ
          printf("^");   // Вывод символа '^'
          ch = ch + 64;  // Преобразование символа
        } else if (ch == 127) {  // Если текущий символ - символ DEL (127)
          printf("^");   // Вывод символа '^'
          ch = ch - 64;  // Преобразование символа
        }
      }
      putchar(ch);  // Вывод текущего символа
    }
    fclose(filename);  // Закрытие файла
  } else {
    fprintf(stderr, "No such file or directory: %s\n",
            path);  // Вывод сообщения об ошибке в stderr
  }
}
