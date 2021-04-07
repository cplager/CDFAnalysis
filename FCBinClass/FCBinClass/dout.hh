#if !defined(DoutHH)
#define DoutHH

#if !defined(_DEBUG_QUIET)
#define dout std::cerr << __FILE__ << "(" << setw(5) << __LINE__ << ") "
#else
#define dout if(0) std::cout << " "
#endif

#endif // defined DoutHH
