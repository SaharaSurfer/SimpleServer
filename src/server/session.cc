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


}
