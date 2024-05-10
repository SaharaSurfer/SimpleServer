#include "header/login_handler.h"

#include <vector>
#include <string>

std::string LoginHandler::handle(const std::vector<std::string>& params) {
  std::string response;
  
  if (params.size() != 2) {
    response = "Invalid <LOGIN> request parameters";
  } else {
    if (user_id_ != -1) {
      response = "You're already in the system.";
    } else {
      std::string login_data = join_strings(params);

      user_id_ = db_.log_in_user(login_data);
      if (user_id_ != -1) {
        response = "Login successful.";
      } else {
        response = "Login failed. Incorrect username or password.";
      }
    }
  }

  return response;
}

std::string LoginHandler::join_strings(const std::vector<std::string>& data) {
  std::string result;

  for (const std::string& w : data) {
    if (!result.empty()) {
      result += " ";
    }
    result += w;
  }

  return result;
}
