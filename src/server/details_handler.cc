#include "header/details_handler.h"

#include <vector>
#include <string>

std::string DetailsHandler::handle(const std::vector<std::string>& params) {
  std::string response;
  
  if (params.size() == 2) {
    std::string isbn = params[0];

    response = db_.get_book_details(isbn);

    if (response.empty()) {
      response = "Unable to find book with ISBN " + isbn + ".";
    }
  } else {
    response = "Invalid DETAILS request.";
  }

  return response;
}
