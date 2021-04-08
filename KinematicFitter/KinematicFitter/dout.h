#if !defined(DoutH)
#define DoutH

#if !defined(_DOUT_QUIET)
#define ccout cout
#define dout cout << __FILE__ << "(" << __LINE__ << ") "
#else
#define ccout if(o)cout
#define dout if(0)cout
#endif

#endif
