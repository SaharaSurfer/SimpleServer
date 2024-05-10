#ifndef SIMPLESERVER_HEADER_DETAILSHANDLER_H_
#define SIMPLESERVER_HEADER_DETAILSHANDLER_H_

#include <vector>
#include <string>

#include "bookstore_database.h"
#include "request_handler_interaface.h"

class DetailsHandler : public RequestHandlerInterface {
 public:
  DetailsHandler(BookstoreDatabase& db) : RequestHandlerInterface(db) {}

  std::string handle(const std::vector<std::string>& params);
};

#endif // SIMPLESERVER_HEADER_DETAILSHANDLER_H_
