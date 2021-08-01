#define CXXProgram 1
#define QtProgram 1
#define DtkProgram 1


#if CXXProgram
    #include "./include/publishfunc_cxx.h"
#endif

#if QtProgram
    #include "./include/publishfunc_qt.h"
#endif

#if DtkProgram
    #include "./include/publishfunc_dtk.h"
#endif
