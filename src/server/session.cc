#include "header/session.h"

#include <iostream>
#include <string>
#include <utility>
#include <boost/asio.hpp>

#include "header/bookstore_database.h"

void Session::receive_request() {
  try {
    boost::asio::read_until(socket_, data_, "\0");
    handle_request();
  } catch (const std::exception& e) {
    // Handle errors here...
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
