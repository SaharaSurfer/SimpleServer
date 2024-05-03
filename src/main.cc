#include <iostream>
#include <string>

#include "header/bookstore_database.h"

int main() {
  std::string connection_data = std::getenv("DATABASE_URL");

  if (connection_data.empty()) {
    std::cerr << "DATABASE_URL environment variable not set" << std::endl;
    return 1;
  }

  try {
    BookstoreDatabase based(connection_data);
  
    std::string test_1 = based.get_book_details("978-1-40-885565-2");
    std::cout << test_1 << std::endl;
    
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
  
  return 0;
}
