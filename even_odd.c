#include <stdio.h>
int main() {
  int num;
  printf("输入一个数字: ");
  scanf("%d", &num);
  if (num % 2 == 0)
    printf("%d 是 偶数\n", num);
  else
    printf("%d 是 奇数\n", num);
  return 0;
}
