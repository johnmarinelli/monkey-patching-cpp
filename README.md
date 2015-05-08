# monkey-patching-cpp

ON OSX 10.9.5, gcc 4.9

Vtable for toPatch
toPatch::_ZTV7toPatch: 5u entries
0     (int (*)(...))0
8     (int (*)(...))(& _ZTI7toPatch)
16    (int (*)(...))toPatch::broken
24    (int (*)(...))toPatch::brokenInt
32    (int (*)(...))toPatch::brokenWithArgs

Class toPatch
   size=8 align=8
   base size=8 base align=8
toPatch (0x0x104308f00) 0 nearly-empty
    vptr=((& toPatch::_ZTV7toPatch) + 16u)
    
    

