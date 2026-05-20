#include <stdio.h>
int main() {
  int a, b;
  printf("输入两个数: ");
  scanf("%d %d", &a, &b);
  a = a + b;
  b = a - b;
  a = a - b;
  printf("交换后: a = %d and b = %d\n", a, b);
  return 0;
}
