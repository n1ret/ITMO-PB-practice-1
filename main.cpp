#include <cstring>
#include <fstream>
#include <iostream>

const int CHARS_SIZE = 26;
const int WORD_SIZE = 33;
const int FILE_PATH_SIZE = 1000;

bool is_in(char* str, int str_size, char to_find) {
  for (int j = 0; j < str_size; j++)
    if (to_find == str[j]) {
      return true;
    }

  return false;
}

int main(int argc, char** argv) {
  int found_args = 0;
  char necessary_chars[CHARS_SIZE];
  int chars_count = 0;
  char file_path[FILE_PATH_SIZE];

  for (int i = 1; i < argc; i++) {
    if (std::strcmp(argv[i], "--word") == 0) {
      if (found_args & 0b00000001) {
        std::cerr << "The word argument must not repeat\n";
        return -1;
      }

      if (i + 1 == argc) {
        std::cerr << "The word argument must have a value\n";
        return -1;
      }

      int u = 0;
      while (argv[i + 1][u] != 0) {
        if ('A' <= argv[i + 1][u] && argv[i + 1][u] <= 'Z')
          argv[i + 1][u] += 'a' - 'A';
        if (argv[i + 1][u] < 'a' || 'z' < argv[i + 1][u]) {
          std::cerr << "The symbol \"" << argv[i + 1][u]
                    << "\" must not be in the word\n";
          return -1;
        }
        if (is_in(necessary_chars, CHARS_SIZE, argv[i + 1][u])) continue;

        necessary_chars[chars_count++] = argv[i + 1][u];

        u++;
      }

      found_args |= 0b00000001;
    } else if (std::strcmp(argv[i], "--file") == 0) {
      if (found_args & 0b00000010) {
        std::cerr << "The file argument must not repeat\n";
        return -1;
      }

      if (i + 1 == argc) {
        std::cerr << "The file argument must have a value\n";
        return -1;
      }

      std::strncpy(file_path, argv[i + 1], FILE_PATH_SIZE - 1);

      found_args |= 0b00000010;
    }
  }

  if (found_args & 0b00000011 != 0b00000011) {
    std::cerr << "Args was not passed\n";
    return -1;
  }

  std::ifstream file;

  file.open(file_path);

  if (!file.is_open()) {
    std::cerr << "The file can't be opened";
    return -1;
  }

  char word[WORD_SIZE];

  int words_count = 0;
  while (file >> word) {
    for (int i = 0; i < WORD_SIZE; i++) {
      if ('A' <= word[i] && word[i] <= 'Z') word[i] += 'a' - 'A';
    }

    int in_count = 0;
    for (int i = 0; i < chars_count; i++) {
      if (is_in(word, WORD_SIZE, necessary_chars[i])) in_count++;
    }

    if (in_count == chars_count) {
      words_count++;
    }
  }

  file.close();

  std::cout << words_count << '\n';

  return 0;
}
