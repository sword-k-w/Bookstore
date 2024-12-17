#include "front_end.h"

int main() {
#ifdef debug
  freopen("data.txt", "r", stdin);
  freopen("out.txt", "w", stdout);
#endif
  FrontEnd a;
  a.run();
  return 0;
}

// 数据点分析
// 1.1 & 4.1 仅含 su/logout/quit/exit
// 1.1 不含除 \r 和 \n 外的不可见字符