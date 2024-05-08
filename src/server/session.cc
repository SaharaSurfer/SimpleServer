#include "header/session.h"

#include <iostream>
#include <string>
#include <utility>
#include <boost/asio.hpp>

#include "header/bookstore_database.h"

void Session::send_welcome_message() {
  try {
    std::vector<std::string> welcome_summaries = db_.get_summaries(5, 1);

    std::string welcome_string;
    std::string divider(80, '-');
    for (const std::string& row : welcome_summaries) {
      welcome_string += row + divider + '\n';
    }

    boost::asio::write(socket_, boost::asio::buffer(welcome_string + "\r\n\r\n"));

  } catch (const std::exception& e) {
    std::cerr << "Error in Session::send_welcome_message(): " << e.what() << std::endl;
  }
}

void Session::read_request() {
  try {
    boost::system::error_code ec;

    // Clear input buffer
    input_buffer_.consume(input_buffer_.size());

    read_until(socket_, input_buffer_, "\r\n", ec);

    if (ec) {
      throw boost::system::system_error(ec);
    }

  } catch (const std::exception& e) {
    std::cerr << "Error in Session::read_request(): " << e.what() << std::endl;
  }
}

void Session::process_request() {
  std::istream is(&input_buffer_);
  std::string request;
  is >> request;

  std::string response = "";

  if (request == "<STOP>") {
    stop();
  } else if (request == "<FORWARD>") {
    current_page_++;
    std::vector<std::string> summaries = db_.get_summaries(5, current_page_ * 5 + 1);

    if (summaries.empty()) {
      response = "Error finding books.";
    } else {
      std::string divider(80, '-');
      for (const std::string& summary : summaries) {
        response += summary + divider + '\n';
      }
    }

  } else if (request == "<DETAILS>") {
    std::string isbn;

    if (std::getline(is, isbn)) {
      for (std::string::iterator it = isbn.begin(); it != isbn.end(); ) {
        // Remove spaces and newline characters
        if (*it == ' ' || *it == '\n' || *it == '\r') {
            it = isbn.erase(it);
        } else {
            ++it;
        }
      }

      response = db_.get_book_details(isbn);

      if (response.empty()) {
        response = "Unable to find book with ISBN " + isbn + ".";
      }
    } else {
      response = "Invalid DETAILS request.";
    }

  } else if (request == "<ADD_TO_CART>") {
    int quantity;
    std::string isbn;
    is >> quantity >> isbn;

    for (std::string::iterator it = isbn.begin(); it != isbn.end(); ) {
      // Remove spaces and newline characters
      if (*it == ' ' || *it == '\n' || *it == '\r') {
        it = isbn.erase(it);
      } else {
        ++it;
      }
    }
    
    if (user_id_ == -1) {
      response = "First you have to log in or register!";
    } else {
      if (db_.add_book_to_cart(user_id_, quantity, isbn)) {
        response = "Book added to cart.";
      } else {
        response = "Failed to add book to cart.";
      }
    }

  } else if (request == "<CREATE_ORDER>") {
    if (user_id_ == -1) {
      response = "First you have to log in or register!";
    } else {
      int order_id = db_.create_order(user_id_);
      if (order_id != -1) {
        response = "Order created with ID: " + std::to_string(order_id);
      } else {
        response = "Failed to create order.";
      }
    }

  } else if (request == "<REGISTER>") {
    std::string registration_data;

    if (std::getline(is, registration_data)) {
      user_id_ = db_.register_user(registration_data);
      if (user_id_ != -1) {
        response = "User registered with ID: " + std::to_string(user_id_);
      } else {
        response = "Failed to register user.";
      }
    } else {
      response = "Invalid REGISTER request.";
    }
  } else if (request == "<LOGIN>") {
    if (user_id_ != -1) {
      request = "You're already in the system.";
    } else {
      std::string login_data;

      if (std::getline(is, login_data)) {
        user_id_ = db_.log_in_user(login_data);
        if (user_id_ != -1) {
          response = "Login successful.";
        } else {
          response = "Login failed. Incorrect username or password.";
        }
      } else {
        response = "Invalid LOGIN request.";
      }
    }

  } else {
    response = "Invalid request: " + request;
  }

  boost::asio::write(socket_, boost::asio::buffer(response + "\r\n\r\n"));
}
