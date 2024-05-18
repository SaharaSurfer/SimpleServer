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

boost::asio::ip::tcp::socket& Session::socket() {
  return socket_;
}

void Session::start() {
  try {
    bind_handlers();
    send_welcome_message();

    do_read();
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

void Session::do_read() {
  auto self(shared_from_this());
  boost::asio::async_read_until(socket_, buffer_, "\r\n",
      [this, self](const boost::system::error_code& ec, std::size_t) {
        if (!ec) {
          std::istream is(&buffer_);
          std::string request;
          std::getline(is, request);

          std::vector<std::string> params = self->break_by_spaces(request);
          std::string type = params.front();
          params.erase(params.begin());
          
          if (type == "<STOP>") {
            stop();
            std::cerr << "User disconected" << std::endl;
            return;
          }

          process_request(type, params);
          do_read();
        } else {
          if (ec == boost::asio::error::eof) {
            std::cerr << "The client terminated the connection." << std::endl;
          } else {
            std::cerr << "Error in Session::read_request(): " << ec.message() << std::endl;
          }
          stop();
        }
  });
}

void Session::process_request(const std::string& type,
                              const std::vector<std::string>& params) {
  std::string response = "";

  auto it = handlers_.find(type);
  if (it != handlers_.end()) {
    response = it->second->handle(params);
  } else {
    response = "Invalid request: " + type;
  }

  boost::asio::async_write(socket_, boost::asio::buffer(response + "\r\n\r\n"),
      [this](const boost::system::error_code& ec, std::size_t) {
        if (ec) {
          std::cerr << "Error in Session::process_request(): " << ec.message() << std::endl;
        }
      });
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
