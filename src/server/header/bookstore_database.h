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
  bool add_book_to_cart(int user_id, int quantity, const std::string& isbn);
  bool change_cart_book_quantity(int user_id, int new_quantity,
                                 const std::string& isbn);
  int create_order(int user_id);

  int register_user(const std::string& registration_data);
  bool log_in_user(const std::string& log_in_data);
};

#endif // SIMPLESERVER_HEADER_BOOKSTOREDATABASE_H_
