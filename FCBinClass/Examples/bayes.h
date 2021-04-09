const double kBack = 4.0;		// # expected background events  
const double kBackErr = 0.3;		// expected background error
const double kObsv = 9;			// # observed events
const double kIntLimit = 10;		// integration upper limit
const double kStepSize = 0.1;		// step size
const double kConfidence = 0.95;	// cofnfidence level
const double kAcceptance = 0.2;		// signal acceptance
const double kAccErr = 0.02;		// error on acceptance
const double kLuminosity = 346.;	// dataset integrated luminosity
const bool   kFast = true;		// if true, use minuit to find Poisson

static double sm_Sigma;			// cross section

// Poisson with errors; P(n_obsv | n_exp) * Gaus(Accept_err) * Gaus (Back_err)
void fcn();

double find_max_P_obsv( double sigma );

void find_bayes_limit();

void set_sigma ( double sigma ) { sm_Sigma = sigma; }

static double sigma () { return sm_Sigma; }



