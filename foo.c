#include <stdio.h>

void loopFunc(int lower, int upper, const char *customMsg) {
  printf("Commencing loopFunc\n");
  printf("Custom msg: %s\n", customMsg);
  int index = lower;
  while (index < upper) {
    printf("%d\n", index);
    ++index;
  }
  printf("Finishing loopFunc\n");
}

int main(int argc, char *argv[]) {
  loopFunc(5, 100, "Hello, world!\n");
  return 0;
}
