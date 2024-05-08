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
}

void Server::accept_connection() {
  while (true) {
    ip::tcp::socket socket = acceptor_.accept(io_context_);
    std::make_shared<Session>(std::move(socket), database_)->start();
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
