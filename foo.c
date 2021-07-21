#include <stdio.h>

void printMsg(const char *msg) {
  for (int i = 0; i < 1000; ++i) {
    printf("%d\n", i);
  }
  printf("%s", msg);
}

int main(int argc, char *argv[]) {
  printMsg("Hello, world!\n");
  return 0;
}
