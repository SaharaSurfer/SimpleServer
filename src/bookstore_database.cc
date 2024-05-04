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
      return "Book not found!";
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

int BookstoreDatabase::register_user(const std::string& registration_data) {
  std::string query = "SELECT user_id FROM register_user(&1, &2, &3, &4, &5)";

  std::string username;
  std::string password;
  std::string email;
  std::string address;
  std::string phone;

  try {
    std::istringstream iss(registration_data);
    iss >> username >> password >> email >> address >> phone;

    pqxx::result res = transaction_.exec_params(username, password, email,
                                                address, phone);

    if (res.size() != 1) {
      throw std::runtime_error("Failed to register user!");
    }

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

    pqxx::result res = transaction_.exec_params(username, password);

    if (res.size() != 1) {
      throw std::runtime_error("Failed to log in user!");
    }

    return res[0]["is_success"].as<bool>();

  } catch (const std::exception& e) {
    last_error = e.what();
  }

  return false;
}
