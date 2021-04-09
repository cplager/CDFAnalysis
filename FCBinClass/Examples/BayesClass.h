// -*- C++ -*-

#if !defined(BayesClass_H)
#define BayesClass_H

#include <list>
#include <vector>

class BayesClass
{
   public:
      //////////////////////
      // Public Constants //
      //////////////////////

      //////////////////////////
      //            _         //
      // |\/|      |_         //
      // |  |EMBER | UNCTIONS //
      //                      //
      //////////////////////////

      /////////////////////////////////
      // Constructors and Destructor //
      /////////////////////////////////
      BayesClass();
      
      ////////////////
      // One Liners //
      ////////////////
      // access functions  
      
      // setting functions
      
      // static access functions
      static double back()       { return sm_back;      } 
      static double back2()	 { return sm_back2;	}
      static double backErr()    { return sm_backErr;   }
      static double backErr2()	 { return sm_backErr2;  } 
      static int    numObsv()    { return sm_numObsv;   }
      static int    numObsv2()	 { return sm_numObsv2;  }
      static double intLimit()   { return sm_intLimit;  }
      static double stepSize()   { return sm_stepSize;  }
      static double confid()     { return sm_confid;    }
      static double acclum()	 { return sm_acclum;	}
      static double acclum2()	 { return sm_acclum2;	}      
      static double acclumErr()  { return sm_acclumErr; }
      static double acclumErr2() { return sm_acclumErr2;}
      static bool   fast()	 { return sm_fast;      }
      static double sigma()	 { return sm_sigma;     }
      static int    numDim()     { return sm_numDim;    }
      
      //static setting functions
      static void setBack      (double back)      { sm_back      = back;      }
      static void setBack2     (double back2)     { sm_back2     = back2;     }      
      static void setBackErr   (double backErr)   { sm_backErr   = backErr;   }
      static void setBackErr2  (double backErr2)  { sm_backErr2  = backErr2;  }      
      static void setNumObsv   (int    numObsv)   { sm_numObsv   = numObsv;   }
      static void setNumObsv2  (int    numObsv2)  { sm_numObsv2  = numObsv2;  }      
      static void setIntLimit  (double intLimit)  { sm_intLimit  = intLimit;  }
      static void setStepSize  (double stepSize)  { sm_stepSize  = stepSize;  }
      static void setConfid    (double confid)    { sm_confid    = confid;    }
      static void setAcclum    (double acclum)    { sm_acclum    = acclum;    }
      static void setAcclum2   (double acclum2)   { sm_acclum2   = acclum2;   } 
      static void setAcclumErr (double acclumErr) { sm_acclumErr = acclumErr; }
      static void setAcclumErr2(double acclumErr2){ sm_acclumErr2= acclumErr2;}            
      static void setFast      (bool   fast)      { sm_fast      = fast;      }
      static void setSigma     (double sigma)     { sm_sigma     = sigma;     }
      static void setNumDim    (int    numDim)    { sm_numDim    = numDim;    }

      //////////////////////////////
      // Regular Member Functions //
      //////////////////////////////
      
      void find_bayes_limit();
      
      double find_max_P_obsv ( double sigma, int numDim );
      
      /////////////////////////////
      // Static Member Functions //
      /////////////////////////////
      
      static void _fcn(int &npar, double* grad, double &chisq, double *par,
                       int iflag);
            
      
  protected:
      /////////////////////////
      // Protected Constants //
      /////////////////////////

      ///////////////////////////
      // Protected Member Data //
      ///////////////////////////

  private:
      ///////////////////////
      // Private Constants //
      ///////////////////////

      /////////////////////////
      // Private Member Data //
      /////////////////////////

      ///////////////////////////////////////////////////////////////
      // Note: C++ will create a default copy constructor.  This   //
      // ONLY works if all of the data members themselves have     //
      // good default copy constructors.  Built-in types are fine, //
      // pointers are NOT.                                         //
      ///////////////////////////////////////////////////////////////       

      // static member data
      static double sm_back, sm_back2;
      static double sm_backErr, sm_backErr2;
      static int    sm_numObsv, sm_numObsv2;
      static double sm_intLimit;
      static double sm_stepSize;
      static double sm_confid;
      static double sm_acclum, sm_acclum2;
      static double sm_acclumErr, sm_acclumErr2;
      static bool   sm_fast; 
      static double sm_sigma;   
      static int    sm_numDim;
};

#endif // BayesClass_H
