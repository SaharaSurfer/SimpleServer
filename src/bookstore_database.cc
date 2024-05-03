#include "header/bookstore_database.h"

#include <string>

std::string BookstoreDatabase::get_book_details(std::string isbn) {
  std::string query = "SELECT b.isbn, b.title, g.genre_name, "
                      "       a.author_name, p.publisher_name, "
                      "       b.publication_date, b.price "
                      "FROM book b "
                      "JOIN genre g ON b.genre_id = g.genre_id "
                      "JOIN author a ON b.author_id = a.author_id "
                      "JOIN publisher p ON b.publisher_id = p.publisher_id "
                      "WHERE b.isbn = '" + isbn + "'";
  
  try {
    pqxx::result res = transaction_.exec(query);

    if (res.empty()) {
      return "Book not found!";
    }

    std::string book_details = "Description:\n"
        "ISBN: " + res[0]["isbn"].as<std::string>() + "\n"
        "TITLE: " + res[0]["title"].as<std::string>() + "\n"
        "GENRE: " + res[0]["genre_name"].as<std::string>() + "\n"
        "AUTHOR: " + res[0]["author_name"].as<std::string>() + "\n"
        "PUBLISHER: " + res[0]["publisher_name"].as<std::string>() + "\n"
        "PUBLICATION DATE: " + res[0]["publication_date"].as<std::string>() + "\n"
        "PRICE: " + res[0]["price"].as<std::string>() + "\n";

    return book_details;

  } catch (const std::exception& e) {
    last_error = e.what();
  }

  return "An error occured!";
}
