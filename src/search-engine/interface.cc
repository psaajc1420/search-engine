//
// Created by Jacob Cadena on 12/6/22.
//

#include "interface.h"

int Interface::Menu() {
  using std::cout;
  using std::endl;
  cout << "1. Create Index" << endl;
  cout << "2. Read Persistence" << endl;
  cout << "3. Write Persistence" << endl;
  cout << "4. Query" << endl;
  cout << "5. Get Statistics " << endl;
  cout << "6. Clear Index" << endl;
  cout << "Enter choice or 0 to exit: ";
  int choice;
  std::cin >> choice;
  cout << endl;
  return choice;
}

void Interface::Run() {
  using std::cout;
  using std::endl;

  index_handler_ = new IndexHandler("docs.json");
  keys_ = {"words", "persons", "orgs"};

  for (const auto &key: keys_) {
    std::string filename = key + ".json";
    index_handler_->AddIndex(key, new Index(filename));
  }

  int choice;
  do {
    choice = Menu();
    switch (choice) {
      case 0: cout << "Goodbye!" << endl;
        break;
      case 1: CreateIndex();
        break;
      case 2: ReadPersistence();
        break;
      case 3: WritePersistence();
        break;
      case 4: Query();
        break;
      case 5: GetStats();
        break;
      case 6: ClearIndex();
        break;
      default: cout << "Invalid option...try again" << endl;
        break;
    }
  } while (choice != 0);
}

void Interface::CreateIndex() {
  std::cout << "Creating index..." << std::endl;
  Parser parser(index_handler_, filenames_[0]);
  timer_.Start();
  parser.Traverse(filenames_[1]);
  timer_.Stop();
  timing_info_.parse_time_ = timer_.ElapsedSeconds();
}

void Interface::ReadPersistence() {
  std::cout << "Reading Persistence..." << std::endl;
  timer_.Start();
  index_handler_->ReadIndexes();
  index_handler_->ReadDocuments();
  timer_.Stop();
  timing_info_.persistence_time_ = timer_.ElapsedSeconds();
}

void Interface::WritePersistence() {
  std::cout << "Writing Persistence..." << std::endl;
  index_handler_->WriteIndexes();
  index_handler_->WriteDocuments();
}

void Interface::Query() {
  QueryProcessor query_processor(index_handler_);
  query_processor.Run();
}

void Interface::GetStats() {
  using std::cout;
  using std::endl;

  cout << "Total number of documents parsed: ";
  cout << index_handler_->GetNumDocuments() << endl;
  for (const auto& key: keys_) {
    cout << "Total number of " << key << " parsed: ";
    cout << index_handler_->GetIndex(key)->Size() << endl;
  }
  cout << "Previous time to parse all documents: ";
  cout << timing_info_.parse_time_ << "(s)" << endl;
  cout << "Previous time to read persistence: ";
  cout << timing_info_.persistence_time_ << "(s)" << endl;
  cout << endl << endl;
}

void Interface::ClearIndex() {
  std::cout << "Clearing index..." << std::endl;
  index_handler_->ClearIndexes();
  std::cout << std::endl;
}



