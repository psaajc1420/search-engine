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
  Parse(text, filename);

  ifs.close();
}

void Parser::Parse(std::string text, std::string filename) {

  std::unordered_set<std::string> seen_words;

  if (!text.empty()) {
    char *input = text.data();
    char delimit[] = " \t\r\n\v\f \",.;:~`''!?@#$%^&*()_+*-/\\={}[]|1234567890";
    char *token = strtok(input, delimit);

    while (token != nullptr) {
      RemoveCharacters(token);
      if (stop_words.find(token) == stop_words.end()) {
        auto it = word_articles_map.Find(token);
        if (it == word_articles_map.End()) {
          auto token_it = word_articles_map.Insert(token);
          (*token_it).emplace_back(filename);
        } else {
          (*it).emplace_back(filename);
        }
      }
      token = strtok(nullptr, delimit);
    }
  }
}

void Parser::Traverse(const std::string &directory_path) {
  using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

  int counter = 1;
  for (auto const &dir_entry: recursive_directory_iterator{directory_path}) {
    if (counter % 10000 == 0) {
      std::cout << "Files searched: " << counter << std::endl;
    }

    if (strcmp(dir_entry.path().extension().c_str(), ".json") == 0) {
      OpenStream(dir_entry.path());
      counter++;
    }
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

void Parser::RemoveCharacters(char* token) {
  unsigned int i = 0, j = 0;
  while (token[i]) {
    if (std::isalpha(token[i])) {
      token[j++] = static_cast<char>(tolower(token[i]));
    }
    i++;
  }
  token[j] = '\0';
}

