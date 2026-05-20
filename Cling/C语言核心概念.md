# C 语言核心概念速览

> 面向有编程经验者（Rust/Go/Python/ASM），只讲 C 特有的东西，不讲什么是循环变量。

---

## 一、程序骨架

```c
#include <stdio.h>   // 预处理：插入头文件
#include <stdlib.h>
#define PI 3.14       // 预处理：宏替换（编译前文本替换）

int main(int argc, char* argv[]) {
    printf("hello\n");
    return 0;
}
```

- C 程序从 `main` 开始执行
- `argc` = 命令行参数个数，`argv` = 参数字符串数组
- `return 0` 表示成功，非 0 表示错误

---

## 二、基本类型和大小

| 类型 | 通常大小 | 格式化 |
|------|----------|--------|
| `char` | 1 字节 | `%c` |
| `short` | 2 字节 | `%hd` |
| `int` | 4 字节 | `%d` |
| `long` | 4 或 8 字节 | `%ld` |
| `long long` | 8 字节 | `%lld` |
| `float` | 4 字节 | `%f` |
| `double` | 8 字节 | `%lf` |
| `void*` | 8 字节（64位） | `%p` |

**注意**：C 只保证 `sizeof(char) == 1`，其他是"至少这么大"，具体由 ABI 决定。用 `sizeof` 运算符确认。

```c
_Bool flag = 1;    // C99 起，或直接 #include <stdbool.h> 用 bool
```

---

## 三、指针（C 的灵魂）

### 基本操作

```c
int  x = 42;
int* p = &x;      // & 取地址 → p 指向 x
*p = 100;         // * 解引用 → 修改 x 为 100

printf("%p\n", p); // 打印地址
printf("%d\n", *p); // 打印 100
```

### `*` 的两种用法（初学者最晕的）

| 位置 | 含义 |
|------|------|
| `int* p;` | 声明：p 是一个指向 int 的指针 |
| `*p = 1;` | 表达式：解引用 p，访问它指向的值 |

### `&` vs Rust 的引用

```c
// C
int x = 1;
int* p = &x;     // & 就是"取地址"，没有生命周期检查

// Rust
let x = 1;
let p = &x;      // & 是借用，有生命周期和借用检查
```

**C 没有借用检查。** `&x` 之后你可以 free、返回、乱传——编译器不拦，崩溃自己扛。

### 指针运算

```c
int arr[4] = {10, 20, 30, 40};
int* p = arr;         // arr 退化为 &arr[0]

p++;                  // 前进 sizeof(int) 字节
printf("%d", *p);     // 20

// 下标就是指针运算的语法糖
arr[i] == *(arr + i)  // 永远等价
```

### NULL 指针

```c
int* p = NULL;   // 指向空，解引用 = 崩溃
if (p != NULL) { /* 安全使用 */ }
```

---

## 四、数组

### 声明

```c
int a[5] = {1,2,3,4,5};       // 栈上固定大小
int b[]  = {1,2,3};           // 自动推断长度：3
int c[10] = {0};              // 全部初始化为 0
int d[5];                     // 未初始化！值是垃圾（栈上残留）
```

### 数组和指针的关键差异

```c
int arr[5];
int* p = arr;

sizeof(arr);  // = 20（整个数组：5 * 4 字节）
sizeof(p);    // = 8（指针本身的大小）
```

数组名在大部分上下文中**退化为指针**，但 `sizeof` 和 `&arr` 保留数组语义。

### VLA（Variable Length Arrays, C99 起）

```c
int n = 10;
int arr[n];   // 运行时决定大小，栈上分配
```

注意 C++ 不支持 VLA，C11 起变成可选。

---

## 五、字符串——C 最坑的地方

### C 没有字符串类型

字符串 = `char` 数组 + 末尾 `'\0'`（null terminator）。

```c
char s1[] = "hello";     // 自动加 '\0'，实际大小 6 字节
char s2[6] = "hello";    // 显式分配
char s3[] = {'h','e','l','l','o','\0'}; // 和上面等价

s1[0] = 'H';             // 可以修改

char* s4 = "hello";      // 字符串字面量——不能修改！（只读区）
// s4[0] = 'H';          // 未定义行为！可能崩溃
```

### 常用操作（需要 `#include <string.h>`）

| 函数 | 作用 | 注 |
|------|------|----|
| `strlen(s)` | 返回长度（不含 `\0`），O(n) | 非恒等 Rust 的 `.len()` |
| `strcpy(dst, src)` | 复制 | **不检查目标大小！** 溢出就 UB |
| `strncpy(dst, src, n)` | 限制 n 个字符复制 | 可能不补 `\0` |
| `strcat(dst, src)` | 追加 | 同样不检查大小 |
| `strcmp(a, b)` | 比较，返回 0 相等 | 不是布尔值 |
| `sprintf(buf, fmt, ...)` | 格式化到字符串 | 同样不检查大小 |

```c
// 安全做法：snprintf
char buf[64];
snprintf(buf, sizeof(buf), "hello %s, age %d", name, age);
```

---

## 六、内存管理：malloc / free

### 栈 vs 堆

```c
void foo() {
    int a = 1;          // 栈：自动分配释放
    int* p = malloc(4); // 堆：手动分配
    *p = 42;
    free(p);            // 必须手动释放！
}                       // a 自动销毁，但 *p 不会
```

### malloc 三板斧

```c
int* p = malloc(10 * sizeof(int));  // 1. 分配
if (!p) exit(1);                    // 2. 检查（malloc 可能返回 NULL）
// ... 使用 ...
free(p);                            // 3. 释放
p = NULL;                           // 防野指针（好习惯）
```

### 常见内存错误（对应 Rust/Go 没有的烦恼）

| 错误 | 后果 | 对比 |
|------|------|------|
| 忘记 `free` | 内存泄漏 | Go 有 GC，Rust 有 drop |
| `free` 后继续用 | Use-after-free | Rust 所有权阻止 |
| `free` 两次 | Double-free 崩溃 | - |
| 越界写入 | 缓冲区溢出，安全漏洞 | Rust 边界检查 |
| 忘记 `\0` 结尾 | 字符串爆炸 | - |

### 必备工具

```bash
gcc -g -fsanitize=address program.c -o program  # AddressSanitizer
./program                                        # 自动检测内存错误
```

---

## 七、结构体

```c
#include <string.h>

struct Point {
    int x;
    int y;
};

// typedef 省去每次写 struct 关键字
typedef struct {
    char name[64];
    int  age;
    float score;
} Student;

int main() {
    // 初始化
    struct Point p1 = {10, 20};
    Student s1 = {"Alice", 20, 95.5};      // 不用写 struct

    // 访问
    s1.age = 21;
    strcpy(s1.name, "Bob");                 // 数组不能直接赋值

    // 传参（默认值传递！复制整个结构体）
    // 所以通常传指针
    void print_student(const Student* s) {
        printf("%s %d %.1f\n", s->name, s->age, s->score);
        // s->name 等价于 (*s).name
    }
}
```

### 结构体对齐

```c
struct S {
    char  c;   // offset 0
    int   i;   // 被对齐到 offset 4（不是 1）！中间有 3 字节 padding
    short s;   // offset 8
};             // total: 10 + 2 padding = 12 字节
```

---

## 八、函数

### 声明和定义

```c
// 声明（函数原型）：告诉编译器有这么个函数
int add(int a, int b);

// 定义
int add(int a, int b) {
    return a + b;
}
```

如果函数在调用之后才定义，必须先声明。这点和 Go/Rust 不同。

### 传参：只有值传递

```c
void set_to_100(int x) { x = 100; }              // 没用！x 是副本
void set_to_100(int* x) { *x = 100; }            // 传指针才行
```

### 函数指针

```c
int add(int a, int b) { return a + b; }

int (*fptr)(int, int) = &add;   // 函数指针
int result = fptr(3, 4);        // 调用（或用 (*fptr)(3, 4)）
```

---

## 九、预处理

```c
#include <stdio.h>     // 头文件插入
#include "myheader.h"  // 自定义头文件

#define MAX(a,b) ((a)>(b)?(a):(b))   // 带参宏（危险的文本替换）
#define BUFSZ 1024                    // 常量宏

#ifdef DEBUG
    printf("debug\n");
#endif

// 条件编译
#if defined(_WIN32)
    #include <windows.h>
#elif defined(__linux__)
    #include <unistd.h>
#endif
```

### 头文件守卫

```c
// myheader.h
#ifndef MYHEADER_H
#define MYHEADER_H

void foo(void);
int  bar(int x);

#endif
```

---

## 十、常用 I/O

```c
#include <stdio.h>

printf("整数=%d, 浮点=%.2f, 字符=%c, 字符串=%s, 地址=%p\n",
       42, 3.14, 'A', "hello", &x);

scanf("%d", &x);                // 读整数（注意 &！忘写就崩）
fgets(buf, sizeof(buf), stdin); // 安全的读一行（推荐）

// 文件
FILE* f = fopen("test.txt", "r");
if (!f) { perror("open failed"); return 1; }
char line[256];
while (fgets(line, sizeof(line), f)) {
    printf("%s", line);
}
fclose(f);
```

---

## 十一、编译命令

```bash
# 一步到位
gcc program.c -o program

# 多文件
gcc main.c helper.c -o program

# 调试和检查
gcc -Wall -Wextra -pedantic -g program.c -o program   # 全警告
gcc -fsanitize=address -g program.c -o program         # 内存错误检测
gcc -O2 program.c -o program                           # 优化

# 四步拆解
gcc -E program.c -o program.i   # ① 预处理（展开宏）
gcc -S program.i -o program.s   # ② 编译到汇编
gcc -c program.s -o program.o   # ③ 汇编到目标文件
gcc program.o -o program        # ④ 链接
```

---

## 十二、C 与 Rust/Go 核心差异速查

| 场景 | Rust | Go | C |
|------|------|----|---|
| 内存管理 | 所有权系统 | GC | 手动 malloc/free |
| 字符串 | `&str` / `String` | `string` | `char*` + `\0` |
| 空值 | `Option::None` | `nil` | `NULL`（只是 0） |
| 泛型 | 泛型/trait | interface{} | 宏或 void* |
| 容器 | std::collections | 内置 map/slice | 没有，自己造 |
| 错误处理 | Result/Option | 多返回值 | 返回 -1 或 NULL |
| 布尔 | `bool` 类型安全 | `bool` | `int`（0 假，非 0 真） |
| 模块 | `mod` / `use` | `package` / `import` | `#include` 文本插入 |
| 构造函数 | `fn new()` | `NewXxx()` | 没有，自己写 init 函数 |

---

## 十三、一句话 C 哲学

> **C 信任你。** 它不检查边界、不管理内存、不保护类型安全。  
> 给你的是裸地址和裸内存——和 ASM 只隔一层薄薄的抽象。  
> 写对了飞快，写错了就是 UB（未定义行为），什么情况都可能发生。
