#ifndef SIMPLESERVER_HEADER_REQUEST_H_
#define SIMPLESERVER_HEADER_REQUEST_H_

#include <string>
#include <memory>

#include "header/session.h"

struct Request {
  std::shared_ptr<Session> session;
  std::string data;

  Request(std::shared_ptr<Session> s, std::string d)
        : session(std::move(s)), data(std::move(d)) {}
};

#endif // SIMPLESERVER_HEADER_REQUEST_H_
