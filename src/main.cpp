#include "run.h"

int main() {
#ifdef debug
  freopen("data.txt", "r", stdin);
  freopen("out.txt", "w", stdout);
#endif
  Run a;
  a.run();
  return 0;
}

// 数据点分析
// 1.1 & 4.1 仅含 su/logout/quit/exit
// 2.1 仅含 su/logout/register/passwd/useradd/delete/quit/exit
// 1.1 不含除 \r 和 \n 外的不可见字符