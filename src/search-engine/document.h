//
// Created by Jacob Cadena on 12/15/22.
//

#ifndef SEARCH_ENGINE_DOCUMENT_H_
#define SEARCH_ENGINE_DOCUMENT_H_

#include <cstdio>
#include <ctime>
#include <iostream>
#include <string>

class Document {
 public:
  Document();
  Document(size_t id,
           std::string title,
           std::string text,
           std::string publisher,
           std::string date_published,
           std::string file_location,
           std::string url,
           size_t num_words);

  [[nodiscard]] size_t GetId() const;
  void SetId(size_t id);
  [[nodiscard]] const std::string &GetTitle() const;
  void SetTitle(const std::string &title);
  [[nodiscard]] const std::string &GetText() const;
  void SetText(const std::string &text);
  [[nodiscard]] const std::string &GetPublisher() const;
  void SetPublisher(const std::string &publisher);
  [[nodiscard]] const std::string &GetDatePublished() const;
  void SetDatePublished(const std::string &date_published);
  [[nodiscard]] const std::string &GetFileLocation() const;
  void SetFileLocation(const std::string &file_location);
  [[nodiscard]] const std::string &GetUrl() const;
  void SetUrl(const std::string &url);
  [[nodiscard]] size_t GetNumWords() const;
  void SetNumWords(size_t num_words);
  [[nodiscard]] double GetWeight() const;
  void SetWeight(double weight);

  friend bool operator<(const Document&, const Document&);
  friend bool operator==(const Document&, const Document&);
  friend std::ostream& operator<<(std::ostream&, Document&);
 private:
  size_t id_;
  std::string title_;
  std::string text_;
  std::string publisher_;
  std::string date_published_;
  std::string file_location_;
  std::string url_;
  size_t num_words_;
  double weight_;
};

#endif //SEARCH_ENGINE_DOCUMENT_H_
