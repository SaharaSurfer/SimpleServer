#include "header/forward_handler.h"

#include <vector>
#include <string>

std::string ForwardHandler::handle(const std::vector<std::string>& params) {
  current_page_++;
  std::vector<std::string> summaries = db_.get_summaries(5, current_page_ * 5 + 1);

  std::string response;

  if (summaries.empty()) {
    response = "Error finding books.";
  } else {
    std::string divider(80, '-');
    for (const std::string& summary : summaries) {
      response += summary + divider + '\n';
    }
  }

  return response;
}
