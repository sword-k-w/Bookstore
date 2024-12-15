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