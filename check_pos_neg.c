#include <stdio.h>

int main() {
  int num;

  printf("输入一个数字: ");
  scanf("%d", &num);

  if (num > 0) {
    printf("这个数字是正数\n");
  } else if (num < 0) {
    printf("这个数字是负数\n");
  } else {
    printf("这个数字是零.\n");
  }

  return 0;
}
