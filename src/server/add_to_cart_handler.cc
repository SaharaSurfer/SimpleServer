#include "header/add_to_cart_handler.h"

#include <vector>
#include <string>

std::string AddToCartHandler::handle(const std::vector<std::string>& params) {
  std::string response;
  
  if (params.size() != 2) {
    response = "Invalid ADD_TO_CART request";
  } else {
    int quantity = std::stoi(params[0]);
    std::string isbn = params[1];

    if (user_id_ == -1) {
      response = "First you have to log in or register!";
    } else {
      if (db_.add_book_to_cart(user_id_, quantity, isbn)) {
        response = "Book added to cart.";
      } else {
        response = "Failed to add book to cart.";
      }
    }
  }

  return response;
}
