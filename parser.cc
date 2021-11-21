//
// Created by Jacob Cadena on 11/20/21.
//

#include "parser.h"


void Parser::OpenStream(const std::string &filename) {
  ifs.open(filename);

  rapidjson::IStreamWrapper isw(ifs);
  rapidjson::Document document;
  document.ParseStream(isw);

  std::string text = document["text"].GetString();
  Parse(text);

  ifs.close();
}

void Parser::Parse(std::string &text) {
  if (!text.empty()) {
    char *input = text.data();
    char delimit[] = " \t\r\n\v\f \",.;:~`''!?@#$%^&*()_+*-/\\={}[]|1234567890";
    char *token = strtok(input, delimit);

    while (token != nullptr) {
      char c;
      int i = 0;
      while (token[i]) {
        c = token[i];
        if (std::isalpha(c)) c = tolower(c);
        token[i] = c;
        i++;
      }

      if (stop_words.find(token) == stop_words.end()) {
        word_articles_map.insert(std::make_pair(token, std::vector<std::string>()));
      }
      token = strtok(nullptr, delimit);
    }
  }
}

void Parser::Traverse(const std::string& directory_path) {
  using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

  int counter = 1;
  for (auto const &dir_entry: recursive_directory_iterator{directory_path}) {
    if (counter % 10000 == 0) {
      std::cout << "Files searched: " <<  counter << std::endl;
    }
    OpenStream(dir_entry.path());
    counter++;
  }
}
void Parser::ReadStopWords(const std::string &directory) {
  std::ifstream infile("stop_words_english_unix.txt");

  std::string line;
  while (getline(infile, line)) {
    stop_words.insert(line);
  }

  infile.close();
}

