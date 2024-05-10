#include "header/create_order_handler.h"

#include <vector>
#include <string>

std::string CreateOrderHandler::handle(const std::vector<std::string>& params) {
  std::string response;
  
  if (user_id_ == -1) {
    response = "First you have to log in or register!";
  } else {
    int order_id = db_.create_order(user_id_);
    if (order_id != -1) {
      response = "Order created with ID: " + std::to_string(order_id);
    } else {
      response = "Failed to create order.";
    }
  }
  
  return response;
}
