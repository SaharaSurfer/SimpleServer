#include "header/session.h"

#include <iostream>
#include <string>
#include <utility>
#include <boost/asio.hpp>

#include "header/bookstore_database.h"

void Session::start() {
  try {
    send_welcome_message();

    while (socket_.is_open()) {
      std::vector<std::string> params = read_request();
      
      if (!params.empty()) {
        std::string type = params.front();
        params.erase(params.begin());

        request_queue_.push(Request(shared_from_this(), type, params));
      }
    }
  } catch (const std::exception& e) {
    std::cerr << "Session ended with error: " << e.what() << std::endl;
  }
}

void Session::stop() {
  boost::system::error_code ec;
  socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
  socket_.close();
}

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

std::vector<std::string> Session::read_request() {
  try {
    boost::system::error_code ec;
    boost::asio::streambuf buf;

    read_until(socket_, buf, "\r\n", ec);

    if (ec) {
      throw boost::system::system_error(ec);
    }

    std::istream is(&buf);
    std::string request;
    std::getline(is, request);

    std::vector<std::string> params = break_by_spaces(request);
    
    return params;

  } catch (const std::exception& e) {
    std::cerr << "Error in Session::read_request(): " << e.what() << std::endl;
    return {};
  }
}

void Session::process_request(const std::string& type,
                              const std::vector<std::string>& params) {
  std::string response = "";

  if (type == "<STOP>") {
    stop();
  } else if (type == "<FORWARD>") {
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

  } else if (type == "<DETAILS>") {
    if (params.size() == 1) {
      std::string isbn = params[1];

      response = db_.get_book_details(isbn);

      if (response.empty()) {
        response = "Unable to find book with ISBN " + isbn + ".";
      }
    } else {
      response = "Invalid DETAILS request.";
    }

  } else if (type == "<ADD_TO_CART>") {
    if (params.size() != 2) {
      response = "Invalid ADD_TO_CART request";
    } else {
      int quantity = std::stoi(params[1]);
      std::string isbn = params[2];

      if (user_id_ == -1) {
      response = "First you have to log in or register!";
      } else {
        if (db_.add_book_to_cart(user_id_, quantity, isbn)) {
          response = "Book added to cart.";
        } else {
          response = "Failed to add book to cart.";
        }
      }
    }
  } else if (type == "<CREATE_ORDER>") {
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

  } else if (type == "<REGISTER>") {
    if (params.size() == 5) {
      std::string registration_data = join_strings(params);

      user_id_ = db_.register_user(registration_data);
      if (user_id_ != -1) {
        response = "User registered with ID: " + std::to_string(user_id_);
      } else {
        response = "Failed to register user.";
      }
    } else {
      response = "Invalid REGISTER request.";
    }
  } else if (type == "<LOGIN>") {
    if (user_id_ != -1) {
      response = "You're already in the system.";
    } else {
      if (params.size() == 2) {
        std::string login_data = join_strings(params);

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
    response = "Invalid request: " + params[0];
  }

  boost::asio::write(socket_, boost::asio::buffer(response + "\r\n\r\n"));
}

std::vector<std::string> Session::break_by_spaces(const std::string& data) {
  std::vector<std::string> words;
  std::size_t start = 0;
  std::size_t end = data.find(' ');

  while (end != std::string::npos) {
    words.push_back(data.substr(start, end - start));
    start = end + 1;
    end = data.find(' ', start);
  }

  // Push the last word onto the vector
  std::string last_word = data.substr(start);

  // Delete /r symbol
  last_word.pop_back();

  words.push_back(last_word);
  return words;
}

std::string Session::join_strings(const std::vector<std::string>& data) {
  std::string result;

  for (const std::string& w : data) {
    if (!result.empty()) {
      result += " ";
    }
    result += w;
  }

  return result;
}
