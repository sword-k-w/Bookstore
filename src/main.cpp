#include "front_end.h"

int main() {
#ifdef debug
  freopen("data.txt", "r", stdin);
#endif
  FrontEnd a;
  a.run();
  return 0;
}