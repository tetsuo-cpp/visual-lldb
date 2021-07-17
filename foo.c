#include <stdio.h>
#include <unistd.h>

void printMsg(const char *msg) { printf("%s", msg); }

int main(int argc, char *argv[]) {
  printMsg("Hello, world!\n");
  return 0;
}
