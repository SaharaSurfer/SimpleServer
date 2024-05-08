#ifndef SIMPLESERVER_HEADER_REQUEST_H_
#define SIMPLESERVER_HEADER_REQUEST_H_

#include <vector>
#include <string>
#include <memory>

class Session;

struct Request {
  std::shared_ptr<Session> session;
  std::string type;
  std::vector<std::string> params;

  Request(std::shared_ptr<Session> s, const std::string& t, const std::vector<std::string>& p)
        : session(std::move(s)), type(std::move(t)), params(std::move(p)) {}
};

#endif // SIMPLESERVER_HEADER_REQUEST_H_
