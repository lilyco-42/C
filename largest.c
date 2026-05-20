#include <stdio.h>

int main() {
  int a, b, c, largest;

  printf("输入第一个值: ");
  scanf("%d", &a);

  printf("输入第二个值: ");
  scanf("%d", &b);

  printf("输入第三个值: ");
  scanf("%d", &c);

  if (a > b && a > c) {
    largest = a;
  } else if (b > c) {
    largest = b;
  } else {
    largest = c;
  }

  printf("最大值是: %d\n", largest);

  return 0;
}
