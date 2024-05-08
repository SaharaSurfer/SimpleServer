#ifndef SIMPLESERVER_HEADER_BOOKSTOREDATABASE_H_
#define SIMPLESERVER_HEADER_BOOKSTOREDATABASE_H_

#include <string>
#include <vector>
#include <memory>
#include <mutex>

#include <pqxx/pqxx>

class BookstoreDatabase {
 private:
  std::unique_ptr<pqxx::connection> connection_ = nullptr;
  mutable std::mutex connection_mutex_;

  pqxx::work begin_transaction();
  
 public:
  BookstoreDatabase(const std::string& connection_data);

  std::vector<std::string> get_summaries(int num_books, int start_index);
  std::string get_book_details(const std::string& isbn);
  bool add_book_to_cart(int user_id, int quantity, const std::string& isbn);
  bool change_cart_book_quantity(int user_id, int new_quantity, const std::string& isbn);
  int create_order(int user_id);
  int register_user(const std::string& registration_data);
  int log_in_user(const std::string& log_in_data);
};

#endif // SIMPLESERVER_HEADER_BOOKSTOREDATABASE_H_
