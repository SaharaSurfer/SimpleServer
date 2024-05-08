#include "header/bookstore_database.h"

#include <string>
#include <sstream>
#include <format>
#include <vector>
#include <memory>
#include <mutex>

#include <pqxx/pqxx>

BookstoreDatabase::BookstoreDatabase(const std::string& connection_data) {
  connection_ = std::make_unique<pqxx::connection>(connection_data);
}

pqxx::work BookstoreDatabase::begin_transaction() {
  std::lock_guard<std::mutex> lock(connection_mutex_);
  
  if (!connection_ || !connection_->is_open()) {
    throw std::runtime_error("Connection is not open.");
  }

  return pqxx::work(*connection_);
}

std::vector<std::string> BookstoreDatabase::get_summaries(int num_books,
                                                          int start_index) {
  try {
    pqxx::work transaction = begin_transaction();
    std::string query = "SELECT title, author_name, price, isbn "
                        "FROM book_view LIMIT $1 OFFSET $2";
    pqxx::result res = transaction.exec_params(query, num_books, start_index);

    if (res.empty()) {
      transaction.commit();
      return {};
    }

    std::vector<std::string> summaries;
    summaries.reserve(num_books);
    for (const auto& row : res) {
      std::string summary = std::format("TITLE: {}\nAUTHOR: {}\nPRICE: {} rubles\nISBN: {}\n",
                                        row["title"].as<std::string>(),
                                        row["author_name"].as<std::string>(),
                                        row["price"].as<int>(),
                                        row["isbn"].as<std::string>());
      summaries.emplace_back(summary);
    }
    
    transaction.commit();
    return summaries;

  } catch (const std::exception& e) {
    return {};
  }
}

std::string BookstoreDatabase::get_book_details(const std::string& isbn) {
  try {
    pqxx::work transaction = begin_transaction();
    std::string query = "SELECT * FROM book_view "
                        "WHERE isbn = $1";
    pqxx::result res = transaction.exec_params(query, isbn);

    if (res.empty()) {
      transaction.commit();
      return "";
    }

    std::string book_details = std::format(
        "DESCRIPTION:\n"
        "ISBN: {}\n"
        "GENRE: {}\n"
        "AUTHOR: {}\n"
        "TITLE: {}\n"
        "PUBLISHER: {}\n"
        "PUBLICATION DATE: {}\n"
        "PRICE: {} rubles\n",
        res[0]["isbn"].as<std::string>(),
        res[0]["title"].as<std::string>(),
        res[0]["genre_name"].as<std::string>(),
        res[0]["author_name"].as<std::string>(),
        res[0]["publisher_name"].as<std::string>(),
        res[0]["publication_date"].as<std::string>(),
        res[0]["price"].as<std::string>());

    transaction.commit();
    return book_details;
  
  } catch (const std::exception& e) {
    return "";
  }
}

bool BookstoreDatabase::add_book_to_cart(int user_id, int quantity,
                                         const std::string& isbn) {
  try {
    pqxx::work transaction = begin_transaction();
    std::string query = "INSERT INTO cart_book (customer_id, isbn, quantity) "
                        "VALUES ($1, $2, $3)";
    pqxx::result res = transaction.exec_params(query, user_id, isbn, quantity);
    transaction.commit();

    return true;
    
  } catch (const std::exception& e) {
    return false;
  }
}

bool BookstoreDatabase::change_cart_book_quantity(int user_id, int new_quantity,
                                                  const std::string& isbn) {  
  try {
    pqxx::work transaction = begin_transaction();
    std::string query = "SELECT update_cart_book_quantity($1, $2, $3)";
    pqxx::result res = transaction.exec_params(query, new_quantity, user_id, isbn);

    transaction.commit();
    return !res.empty() && res[0][0].as<bool>();

  } catch (const std::exception& e) {
    return false;
  }
}

int BookstoreDatabase::create_order(int user_id) {
  try {
    pqxx::work transaction = begin_transaction();
    std::string query = "SELECT create_order($1)";
    pqxx::result res = transaction.exec_params(query, user_id);

    transaction.commit();
    return res.empty() ? -1 : res[0][0].as<int>();

  } catch (const std::exception& e) {
    return -1;
  }
}

int BookstoreDatabase::register_user(const std::string& registration_data) {
  try {
    std::istringstream iss(registration_data);
    std::string username, password, email, address, phone;
    iss >> username >> password >> email >> address >> phone;

    pqxx::work transaction = begin_transaction();
    std::string query = "SELECT register_user($1, $2, $3, $4, $5)";
    pqxx::result res = transaction.exec_params(query, username, password,
                                                email, address, phone);

    transaction.commit();
    return res.size() != 1 ? -1 : res[0][0].as<int>();

  } catch (const std::exception& e) {
    return -1;
  }
}

int BookstoreDatabase::log_in_user(const std::string& log_in_data) {
  try {
    std::istringstream iss(log_in_data);
    std::string username, password;
    iss >> username >> password;

    pqxx::work transaction = begin_transaction();
    std::string query = "SELECT login_user($1, $2)";
    pqxx::result res = transaction.exec_params(query, username, password);

    transaction.commit();
    return res.size() != 1 ? -1 : res[0][0].as<int>();
  
  } catch (const std::exception& e) {
    return -1;
  }
}
