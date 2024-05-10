#include "header/register_handler.h"

#include <vector>
#include <string>

std::string RegisterHandler::handle(const std::vector<std::string>& params) {
  std::string response;
  
  if (params.size() == 5) {
    if (user_id_ != -1) {
      response = "You're already in the system.";
    } else {
      std::string registration_data = join_strings(params);

      user_id_ = db_.register_user(registration_data);
      if (user_id_ != -1) {
        response = "User registered with ID: " + std::to_string(user_id_);
      } else {
        response = "Failed to register user.";
      }
    }
  } else {
    response = "Invalid REGISTER request.";
  }

  return response;
}

std::string RegisterHandler::join_strings(const std::vector<std::string>& data) {
  std::string result;

  for (const std::string& w : data) {
    if (!result.empty()) {
      result += " ";
    }
    result += w;
  }

  return result;
}

