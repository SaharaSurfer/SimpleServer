#ifndef SIMPLESERVER_HEADER_ADDTOCARTHANDLER_H_
#define SIMPLESERVER_HEADER_ADDTOCARTHANDLER_H_

#include <vector>
#include <string>

#include "bookstore_database.h"
#include "request_handler_interaface.h"

class AddToCartHandler : public RequestHandlerInterface {
 private:
  int& user_id_;

 public:
  AddToCartHandler(BookstoreDatabase& db, int& user_id)
      : RequestHandlerInterface(db), user_id_(user_id) {}

  std::string handle(const std::vector<std::string>& params);
};

#endif // SIMPLESERVER_HEADER_ADDTOCARTHANDLER_H_
