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
  std::string query = "SELECT title, author_name, price "
                      "FROM book_view LIMIT $1 OFFSET $2";

  try {
    pqxx::result res = transaction_.exec_params(query, num_books, start_index);

    if (res.empty()) {
      return {};
    }

    std::vector<std::string> summaries;
    summaries.reserve(num_books);
    for (const auto& row : res) {
      std::string summary = std::format("TITLE: {}\nAUTHOR: {}\nPRICE: {} rubles\n",
                                      row["title"].as<std::string>(),
                                      row["author_name"].as<std::string>(),
                                      row["price"].as<int>());
      summaries.emplace_back(summary);
    }

    return summaries;

  } catch (const std::exception& e) {
    last_error = e.what();
    return {};
  }
}

std::string BookstoreDatabase::get_book_details(const std::string& isbn) {
  std::string query = "SELECT * FROM book_view "
                      "WHERE isbn = $1";
  
  try {
    pqxx::result res = transaction_.exec_params(query, isbn);

    if (res.empty()) {
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

    return book_details;
  
  } catch (const std::exception& e) {
    last_error = e.what();
    return "";
  }
}

bool BookstoreDatabase::add_book_to_cart(int user_id, int quantity,
                                         const std::string& isbn) {
  std::string query = "INSERT INTO cart_book (customer_id, isbn, quantity) "
                      "VALUES ($1, $2, $3)";
  
  try {
    pqxx::result res = transaction_.exec_params(query, user_id, isbn, quantity);
    transaction_.commit();

    return true;
    
  } catch (const std::exception& e) {
    last_error = e.what();
    return false;
  }
}

bool BookstoreDatabase::change_cart_book_quantity(int user_id, int new_quantity,
                                                  const std::string& isbn) {
  std::string query = "SELECT is_success FROM update_cart_book_quantity($1, $2, $3)";
  
  try {
    pqxx::result res = transaction_.exec_params(query, new_quantity, user_id, isbn);

    if (res.empty() || !res[0]["is_success"].as<bool>()) {
      return false;
    }

    transaction_.commit();
    return true;

  } catch (const std::exception& e) {
    last_error = e.what();
    return false;
  }
}

int BookstoreDatabase::create_order(int user_id) {
  std::string query = "SELECT new_order_id FROM create_order($1)";

  try {
    pqxx::result res = transaction_.exec_params(query, user_id);

    if (res.empty()) {
      return -1;
    }

    transaction_.commit();
    return res[0]["new_order_id"].as<int>();

  } catch (const std::exception& e) {
    last_error = e.what();
    return -1;
  }
}

int BookstoreDatabase::register_user(const std::string& registration_data) {
  std::string query = "SELECT user_id FROM register_user($1, $2, $3, $4, $5)";

  std::string username;
  std::string password;
  std::string email;
  std::string address;
  std::string phone;

  try {
    std::istringstream iss(registration_data);
    iss >> username >> password >> email >> address >> phone;

    pqxx::result res = transaction_.exec_params(query, username, password,
                                                email, address, phone);

    if (res.size() != 1) {
      return -1;
    }

    transaction_.commit();
    return res[0]["user_id"].as<int>();

  } catch (const std::exception& e) {
    last_error = e.what();
    return -1;
  }
}

bool BookstoreDatabase::log_in_user(const std::string& log_in_data) {
  std::string query = "SELECT is_success FROM login_user($1, $2)";

  std::string username;
  std::string password;

  try {
    std::istringstream iss(log_in_data);
    iss >> username >> password;

    pqxx::result res = transaction_.exec_params(query, username, password);

    if (res.size() != 1) {
      return false;
    }

    return res[0]["is_success"].as<bool>();

  } catch (const std::exception& e) {
    last_error = e.what();
    return false;
  }
}
