#ifndef SIMPLESERVER_HEADER_FORWARDHANDLER_H_
#define SIMPLESERVER_HEADER_FORWARDHANDLER_H_

#include <vector>
#include <string>

#include "bookstore_database.h"
#include "request_handler_interaface.h"

class ForwardHandler : public RequestHandlerInterface {
 private:
  int current_page_ = -1;

 public:
  ForwardHandler(BookstoreDatabase& db) : RequestHandlerInterface(db) {}
  
  std::string handle(const std::vector<std::string>& params);
};

#endif // SIMPLESERVER_HEADER_FORWARDHANDLER_H_
