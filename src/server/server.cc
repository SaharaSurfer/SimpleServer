#include "header/server.h"

#include <iostream>
#include <string>
#include <utility>
#include <memory>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "header/session.h"

Server::Server(unsigned short port, BookstoreDatabase& db)
  : acceptor_(io_context_), db_(db) {
    
  set_up_acceptor(port);
  begin_accept_connections();
}

void Server::begin_accept_connections() {
  while (true) {
    try {
      boost::asio::ip::tcp::socket socket(io_context_);
      acceptor_.accept(socket);
      std::cout << "Connection established" << std::endl;

      std::make_shared<Session>(std::move(socket), db_)->start();

    } catch (const boost::system::system_error& e) {
      if (e.code() == boost::asio::error::bad_descriptor) {
        // This error occurs when the socket is closed
        std::cerr << "Socket closed. Continuing to accept connections." << std::endl;
        continue;
      } else {
        // Other errors
        std::cerr << "Error accepting connection: " << e.what() << std::endl;
      }
    }
  }
}

void Server::set_up_acceptor(unsigned short port) {
  // Create endpoint
  boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), port);

  // Open the acceptor with the option to reuse the address
  acceptor_.open(endpoint.protocol());
  acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
  acceptor_.bind(endpoint);
  acceptor_.listen();
}

void Server::worker_thread() {
  while (true) {
    Request request(nullptr, "", {});
    if (!request_queue_.pop(request)) {
      break;
    }

    request.session->process_request(request.type, request.params);
  }
}
