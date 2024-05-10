#ifndef SIMPLESERVER_HEADER_LOGINHANDLER_H_
#define SIMPLESERVER_HEADER_LOGINHANDLER_H_

#include <vector>
#include <string>

#include "bookstore_database.h"
#include "request_handler_interaface.h"

class LoginHandler : public RequestHandlerInterface {
 private:
  int& user_id_;

  std::string join_strings(const std::vector<std::string>& data);

 public:
  LoginHandler(BookstoreDatabase& db, int& user_id)
      : RequestHandlerInterface(db), user_id_(user_id) {}

  std::string handle(const std::vector<std::string>& params);
};

#endif // SIMPLESERVER_HEADER_LOGINHANDLER_H_
