//
// Created by Jacob Cadena on 12/15/22.
//

#include "document.h"

Document::Document() : id_{0}, num_words_{0}, weight_{0} {}

Document::Document(size_t id,
                   std::string title,
                   std::string text,
                   std::string publisher,
                   std::string date_published,
                   std::string file_location,
                   std::string url,
                   size_t num_words)
    : id_(id),
      title_(std::move(title)),
      text_(std::move(text)),
      publisher_(std::move(publisher)),
      date_published_(std::move(date_published)),
      file_location_(std::move(file_location)),
      url_(std::move(url)),
      num_words_(num_words),
      weight_{0} {}

size_t Document::GetId() const {
  return id_;
}

void Document::SetId(size_t id) {
  id_ = id;

}
const std::string &Document::GetTitle() const {
  return title_;

}
void Document::SetTitle(const std::string &title) {
  title_ = title;
}

const std::string &Document::GetText() const {
  return text_;

}
void Document::SetText(const std::string &text) {
  text_ = text;
}

const std::string &Document::GetPublisher() const {
  return publisher_;
}

void Document::SetPublisher(const std::string &publisher) {
  publisher_ = publisher;
}

const std::string &Document::GetDatePublished() const {
  return date_published_;
}

void Document::SetDatePublished(const std::string &date_published) {
  date_published_ = date_published;
}

const std::string &Document::GetFileLocation() const {
  return file_location_;
}

void Document::SetFileLocation(const std::string &file_location) {
  file_location_ = file_location;
}

const std::string &Document::GetUrl() const {
  return url_;
}

void Document::SetUrl(const std::string &url) {
  url_ = url;
}

size_t Document::GetNumWords() const {
  return num_words_;
}

void Document::SetNumWords(size_t num_words) {
  num_words_ = num_words;
}

double Document::GetWeight() const {
  return weight_;
}

void Document::SetWeight(double weight) {
  weight_ = weight;
}

bool operator<(const Document &lhs, const Document &rhs) {
  return lhs.id_ < rhs.id_;
}

bool operator==(const Document &lhs, const Document &rhs) {
  return lhs.id_ == rhs.id_;
}

std::ostream &operator<<(std::ostream &out, Document &doc) {
  using std::endl;

  out << doc.title_ << endl;
  out << doc.publisher_ << endl;

  tm* ctime = new tm;
  char date_time[80];
  strptime(doc.date_published_.c_str(), "%FT%T%z", &*ctime);
  strftime (date_time, 80, "%D, %r", ctime);
  out << date_time << endl;
  delete ctime;

  out << doc.file_location_ << endl;
  out << doc.url_ << endl;
  out << doc.weight_ << endl;

  return out;
}

