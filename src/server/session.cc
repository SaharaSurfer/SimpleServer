#include "header/session.h"

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <utility>
#include <boost/asio.hpp>

#include "handlers_includer.h"
#include "header/bookstore_database.h"

void Session::start() {
  try {
    bind_handlers();
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
    std::string welcome_string = handlers_["<FORWARD>"]->handle({});

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
  } else {
    auto it = handlers_.find(type);
    if (it != handlers_.end()) {
      response = it->second->handle(params);
    } else {
      response = "Invalid request: " + type;
    }
  }

  boost::asio::write(socket_, boost::asio::buffer(response + "\r\n\r\n"));
}

void Session::bind_handlers() {
  handlers_["<FORWARD>"] = std::make_unique<ForwardHandler>(db_);
  handlers_["<DETAILS>"] = std::make_unique<DetailsHandler>(db_);
  handlers_["<ADD_TO_CART>"] = std::make_unique<AddToCartHandler>(db_, user_id_);
  handlers_["<CREATE_ORDER>"] = std::make_unique<CreateOrderHandler>(db_, user_id_);
  handlers_["<REGISTER>"] = std::make_unique<RegisterHandler>(db_, user_id_);
  handlers_["<LOGIN>"] = std::make_unique<LoginHandler>(db_, user_id_);
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
