#include <stdio.h>
int main() {
  float length, breadth, area;
  printf("输入长 \n");
  scanf(" %f", &length);
  printf("输入宽 : \n");
  scanf(" %f", &breadth);
  area = length * breadth;
  printf("面积: %.2f\n", area);
  return 0;
}
