#include "header/server.h"

#include <iostream>
#include <string>
#include <utility>
#include <memory>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "header/session.h"

Server::Server(unsigned short port, BookstoreDatabase& db)
    : acceptor_(io_context_), db_(db), pool_(std::thread::hardware_concurrency()) {
  set_up_acceptor(port);
}

Server::~Server() {
  stop();
}

void Server::run() {
  begin_accept_connections();
  
  boost::asio::post(pool_, [this] { io_context_.run(); });
  pool_.join();
}

void Server::stop() {
  stop_ = true;
  acceptor_.close();
  io_context_.stop();
}

void Server::begin_accept_connections() {
  auto new_session = std::make_shared<Session>(io_context_, db_);
  acceptor_.async_accept(new_session->socket(),
                         [this, new_session](const boost::system::error_code& error) {
    if (!error) {
      new_session->start();
    } else {
      std::cerr << "Error accepting connection: " << error.message() << std::endl;
    }

    if (!stop_) {
      begin_accept_connections();
    }
  });
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
