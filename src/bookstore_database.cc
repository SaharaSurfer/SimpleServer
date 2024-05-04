#include "header/bookstore_database.h"

#include <string>
#include <sstream>
#include <vector>

std::vector<std::string> BookstoreDatabase::get_summaries(int num_books,
                                                          int start_index) {
  std::string query = "SELECT title, author_name, price "
                      "FROM book_view LIMIT $1 OFFSET $2";

  try {
    pqxx::result res = transaction_.exec_params(query, num_books, start_index);

    if (res.empty()) {
      throw std::runtime_error("No books found!");
    }

    std::vector<std::string> summaries;
    summaries.reserve(num_books);
    for (const auto& row : res) {
      std::string summary = "TITLE: " + row["title"].as<std::string>() + "\n"
                            "AUTHOR: " + row["author_name"].as<std::string>() + "\n"
                            "PRICE: " + std::to_string(row["price"].as<int>()) + " rubles\n";
      summaries.emplace_back(summary);
    }

    return summaries;

  } catch (const std::exception& e) {
    last_error = e.what();
  }

  return std::vector<std::string>();
}

std::string BookstoreDatabase::get_book_details(const std::string& isbn) {
  std::string query = "SELECT * FROM book_view "
                      "WHERE isbn = $1";
  
  try {
    pqxx::result res = transaction_.exec_params(query, isbn);

    if (res.empty()) {
      throw std::runtime_error("Book not found!");
    }

    std::string book_details = "DESCRIPTION:\n"
        "ISBN: " + res[0]["isbn"].as<std::string>() + "\n"
        "TITLE: " + res[0]["title"].as<std::string>() + "\n"
        "GENRE: " + res[0]["genre_name"].as<std::string>() + "\n"
        "AUTHOR: " + res[0]["author_name"].as<std::string>() + "\n"
        "PUBLISHER: " + res[0]["publisher_name"].as<std::string>() + "\n"
        "PUBLICATION DATE: " + res[0]["publication_date"].as<std::string>() + "\n"
        "PRICE: " + res[0]["price"].as<std::string>() + " rubles\n";

    return book_details;

  } catch (const std::exception& e) {
    last_error = e.what();
  }

  return "An error occured!";
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
  }

  return false;
}

bool BookstoreDatabase::change_cart_book_quantity(int user_id, int new_quantity,
                                                  const std::string& isbn) {
  std::string query = "SELECT is_success FROM update_cart_book_quantity($1, $2, $3)";
  
  try {
    pqxx::result res = transaction_.exec_params(query, new_quantity, user_id, isbn);

    if (res.empty() || !res[0]["is_success"].as<bool>()) {
      throw std::runtime_error("Failed update_quantity in cart_book table!");
    }

    transaction_.commit();
    return true;

  } catch (const std::exception& e) {
    last_error = e.what();
  }

  return false;
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
      throw std::runtime_error("Failed to register user!");
    }

    transaction_.commit();
    return res[0]["user_id"].as<int>();

  } catch (const std::exception& e) {
    last_error = e.what();
  }

  return -1;
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
      throw std::runtime_error("Failed to log in user!");
    }

    return res[0]["is_success"].as<bool>();

  } catch (const std::exception& e) {
    last_error = e.what();
  }

  return false;
}
