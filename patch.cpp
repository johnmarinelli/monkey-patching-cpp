/**
 * ========================
 * Monkey patching with C++
 * ========================
 * for shits and giggles
 * ------------------------
 * compiler-specific since it depends on the vtable implementation
 * tested on 
 * Apple LLVM version 6.0 (clang-600.0.57) (based on LLVM 3.5svn)
 * Target: x86_64-apple-darwin13.4.0
 */

#include <iostream>
#include <stdio.h>
#include <stdint.h>

class toPatch {
public:
  virtual void broken() {
    std::cout << "I'm broken\n";
  }
  
  virtual int brokenInt() {
    return 100;
  }

  virtual int brokenWithArgs(int a, int b) {
    return a + b;
  }
  ~toPatch() {}
};

void patch(toPatch* patchee) {
  std::cout << "I'm fixed\n";
}

int patchInt(toPatch* patchee) {
  return 300;
}

int patchWithArgs(toPatch* patchee, int a, int b) {
  return a * b;
}

template <typename T>
void** getVTable(T ptr) {
  return *(void***)ptr;
}

template <typename RetType, typename Class, typename ... Args>
void* getOffset(RetType(Class::*fn)(Args...)) {
  return *(void**)&fn;
}

template <typename RetType, typename Class, typename ... Args>
void setPatch(void** vTable, void* offset, RetType(*fn)(Class*, Args...)) {
  vTable[((uintptr_t)offset) / sizeof(void*)] = (void*)fn;
}

int main(int argc, char* argv[]) {
  auto tp = new toPatch;

  auto vTable = getVTable(tp);

  void (toPatch::*fnptr)() = &toPatch::broken;
  int (toPatch::*intFnptr)() = &toPatch::brokenInt;
  int (toPatch::*withArgsFnptr)(int, int) = &toPatch::brokenWithArgs;

  auto offset = getOffset(fnptr);
  auto offsetInt = getOffset(intFnptr);
  auto offsetWithArgs = getOffset(withArgsFnptr);

  setPatch(vTable, offset, &patch);
  setPatch(vTable, offsetInt, &patchInt);
  setPatch(vTable, offsetWithArgs, &patchWithArgs);

  tp->broken();
  std::cout << tp->brokenInt() << '\n';
  std::cout << tp->brokenWithArgs(3, 4) << '\n';

  return 0;
}
