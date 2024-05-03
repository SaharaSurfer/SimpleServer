#ifndef SIMPLESERVER_HEADER_BOOKSTOREDATABASE_H_
#define SIMPLESERVER_HEADER_BOOKSTOREDATABASE_H_

#include <string>
#include <vector>

#include <pqxx/pqxx>

class BookstoreDatabase {
 private:
  pqxx::connection connection_;
  pqxx::work transaction_;
  std::string last_error;
  
 public:
  BookstoreDatabase(const std::string& connection_data)
      : connection_(connection_data), transaction_(connection_) {
    if (!connection_.is_open()) {
      throw std::runtime_error("Failed to connect to the database.");
    }
  };

  std::vector<std::string> get_summaries(int num_books, int start_index);
  std::string get_book_details(const std::string& isbn);
};

#endif // SIMPLESERVER_HEADER_BOOKSTOREDATABASE_H_
