// -*- C++ -*-

#if !defined(DefineFunctions_HH)
#define DefineFunctions_HH

extern void defineFunctions();

extern Double_t splitGauss2PlusConst( Double_t* x, Double_t* par );
extern Double_t splitGauss2         ( Double_t* x, Double_t* par );
extern Double_t expPlusConst        ( Double_t* x, Double_t* par );
extern Double_t halfGauss2          ( Double_t* x, Double_t* par ); 
extern Double_t upperHalfGauss2     ( Double_t* x, Double_t* par ); 
extern Double_t splitGauss3         ( Double_t* x, Double_t* par );

#endif // DefineFunctions_HH
