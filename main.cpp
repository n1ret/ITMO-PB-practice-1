#include <cstring>
#include <fstream>
#include <iostream>

const int kCharsSize = 26;
const int kMaxWordSize = 33;

enum FoundArgsMask {
  kAll = 0b11,
  kWord = 0b1,
  kPath = 0b10,
};

bool IsIn(char* str, int str_size, char to_find) {
  for (int j = 0; j < str_size; j++)
    if (to_find == str[j]) {
      return true;
    }

  return false;
}

int main(int argc, char** argv) {
  int found_args = 0;
  char necessary_chars[kCharsSize];
  int chars_count = 0;
  char* file_path;

  for (int i = 1; i < argc; i++) {
    if (std::strcmp(argv[i], "--word") == 0) {
      if (found_args & FoundArgsMask::kWord) {
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
        if (IsIn(necessary_chars, kCharsSize, argv[i + 1][u])) continue;

        necessary_chars[chars_count++] = argv[i + 1][u];

        u++;
      }

      found_args |= FoundArgsMask::kWord;
    } else if (std::strcmp(argv[i], "--file") == 0) {
      if (found_args & FoundArgsMask::kPath) {
        std::cerr << "The file argument must not repeat\n";
        return -1;
      }

      if (i + 1 == argc) {
        std::cerr << "The file argument must have a value\n";
        return -1;
      }

      file_path = argv[i + 1];

      found_args |= FoundArgsMask::kPath;
    }
  }

  if (found_args & FoundArgsMask::kAll != FoundArgsMask::kAll) {
    std::cerr << "Args was not passed\n";
    return -1;
  }

  std::ifstream file(file_path);

  if (!file.good()) {
    std::cerr << "The file can't be read";
    return -1;
  }

  int words_count = 0;
  
  char word[kMaxWordSize];
  while (file>>word) {
    for (int i = 0; i < kMaxWordSize; i++) {
      if ('A' <= word[i] && word[i] <= 'Z') word[i] += 'a' - 'A';
    }

    int in_count = 0;
    for (int i = 0; i < chars_count; i++) {
      if (IsIn(word, kMaxWordSize, necessary_chars[i])) in_count++;
    }

    if (in_count == chars_count) {
      words_count++;
    }
  }

  file.close();

  std::cout << words_count << '\n';

  return 0;
}
