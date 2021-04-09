#if !defined(DoutH)
#define DoutH

#if !defined(_DEBUG_QUIET)
#define dout cerr << __FILE__ << "(" << __LINE__ << ") "
#else
#define dout if(0)cout
#endif

#endif
