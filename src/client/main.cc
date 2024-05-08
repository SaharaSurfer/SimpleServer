#include "header/client.h"

int main() {
  Client client;
  client.connect_to_server("localhost", "8080");
  client.run();
}
