#ifndef SIMPLESERVER_HEADER_REQUESTHANDLERINTERFACE_H_
#define SIMPLESERVER_HEADER_REQUESTHANDLERINTERFACE_H_

#include <vector>
#include <string>

#include "bookstore_database.h"

class RequestHandlerInterface {
 protected:
  BookstoreDatabase& db_;

 public:
  RequestHandlerInterface(BookstoreDatabase& db) : db_(db) {}

  virtual std::string handle(const std::vector<std::string>& params) = 0;
  virtual ~RequestHandlerInterface() = default;
};

#endif // SIMPLESERVER_HEADER_REQUESTHANDLERINTERFACE_H_
