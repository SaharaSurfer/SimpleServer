#include "header/session.h"

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

void Session::handle_request() {

}
