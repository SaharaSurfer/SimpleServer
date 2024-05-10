#ifndef SIMPLESERVER_HEADER_REGISTERHANDLER_H_
#define SIMPLESERVER_HEADER_REGISTERHANDLER_H_

#include <vector>
#include <string>

#include "bookstore_database.h"
#include "request_handler_interaface.h"

class RegisterHandler : public RequestHandlerInterface {
 private:
  int& user_id_;
  
  std::string join_strings(const std::vector<std::string>& data);

 public:
  RegisterHandler(BookstoreDatabase& db, int& user_id)
      : RequestHandlerInterface(db), user_id_(user_id) {}

  std::string handle(const std::vector<std::string>& params);
};

#endif // SIMPLESERVER_HEADER_REGISTERHANDLER_H_
