#include <stdio.h>

int fetch_and_add(int *a, int b);

int add_and_fetch(int *a, int b) {
  return fetch_and_add(a, b) + b;
}

int main() {
  int a = 3;
  int c = 3;
  printf("%d\n", fetch_and_add(&a, 4));
  printf("%d\n", add_and_fetch(&c, 4));
}
