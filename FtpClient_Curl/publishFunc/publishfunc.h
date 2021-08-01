#define CXXProgram 1
#define QtProgram 1
#define DtkProgram 1


#if CXXProgram
    #include "publishfunc_cxx.h"
#endif

#if QtProgram
    #include "publishfunc_qt.h"
#endif

#if DtkProgram
    #include "publishfunc_dtk.h"
#endif
