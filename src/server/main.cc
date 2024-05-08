#include <iostream>
#include <string>

#include "header/server.h"

int main() {
  try {
    std::string connection_data = std::getenv("DATABASE_URL");
    BookstoreDatabase db(connection_data);

    Server server(8080, db);

  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
  
  return 0;
}
