
// This is the script that computes average lepton ID SF times lepton Trig Efficiency to be used in the Analysis 
// The results of the script should be substitued into lepEff() and lepErr() routines of Analysis.cc



// Period  	Run-Range      CEM/CMUP 	CMX                 CEM ID SF             CEM trig                       CMUP ID SF                CMUPtrig             CMX ID SF              CMXtrig            
// 0 	138425-186598 	331.47 	318.11               0.9910 0.0040          0.9620   0.0070            0.9360  0.0055      0.9015   0.0044         1.0098  0.0057        0.9665   0.0036
// 1-4 	190697-203799 	362.94 	359.50               0.9850 0.0040          0.9760   0.0060            0.9286  0.0050      0.9191   0.0039          0.9744  0.0074       0.9120   0.0063 
// 5-7** 	203819-212133  	258.37** 	258.37**            0.9740  0.0040          0.9790   0.0040           0.9174  0.0064      0.9179   0.0048         0.9781  0.0089        0.9049   0.0079 
// 8** 	217990-222426 	166.29** 	166.29**            0.9770  0.0060         0.9590   0.0070           0.9312  0.0078       0.9130   0.0061         0.9704  0.0116       0.9311   0.0076 
// 9 	222529-228596 	156.76 	152.78               0.9780  0.0060         0.9600   0.0020           0.9400  0.0080       0.9270   0.0067         0.9565  0.0132         0.8945   0.0112
// 10 	228644-233111 	243.19 	243.49               0.9780  0.0050          0.9590   0.0020             0.9320  0.0070    0.8520   0.0089         0.9655  0.0110         0.9294   0.0089 
// 11 	233133 - 237795 	234.99 	229.98                0.9680  0.0070         0.9610   0.0040            0.8990  0.0080     0.8623   0.0098          0.9456  0.0113       0.9272   0.0098   
// 12 	237845 - 241664  	162.01 	155.25               0.9610  0.0070          0.9600   0.0030            0.9190  0.0090     0.8417   0.0120         0.9577  0.0135        0.9002   0.0140
// 13 	241665 - 246231 	280.86 	268.35              0.9750  0.0050           0.9570   0.0030            0.9240  0.0070    0.8284   0.0091          0.9466  0.0115         0.8926   0.0094
// 14 	252836 - 254686  	32.01 	30.59                1.0110  0.0130           0.9600   0.0300            0.9100  0.0180    0.8943   0.0222          0.9910  0.0190        0.8904   0.0270
// 15 	254800 - 256824 	161.87 	156.36               0.9960  0.0060          0.9630   0.0050            0.8970  0.0090    0.8790   0.0100          0.9570  0.0100        0.9226   0.0128
// 16 	256840 - 258787 	101.81 	100.74
// 17 	258880 - 261005 	183.56 	182.93
// Total 			 2676.13 	2622.74


{

  gSystem->Load("~/root_macros/Value_cc.so");
  // Si good run list
  //p0-17 

  // note that p0 needs to be calculated separately:
  // cmx lumi for p0 already accounst for the fact that cmx is missing for runs < 150000
  // plus miniskirts and keystones are off

  double Lumi_cem[13] = {331.47, 362.94,  258.37, 166.29, 156.76 , 243.19, 234.99, 162.01, 280.86, 32.01 , 161.87 , 101.81, 183.56};
  double Lumi_cmx[13] = {318.11, 359.50, 258.37, 166.29, 152.78  , 243.49 , 229.98, 155.25  , 268.35   , 30.59  , 156.36, 100.74, 182.93 };

  Value CEM_ID[13] = { Value(0.9910, 0.0040 ), 
		       Value(0.9850, 0.0040 ),
		       Value(0.9740,  0.0040),     
		       Value(0.9770,  0.0060 ),
		       Value(0.9780,  0.0060 ), 
		       Value(0.9780,  0.0050 ),
		       Value(0.9680,  0.0070 ),
		       Value(0.9610,  0.0070   ),
		       Value(0.9750,  0.0050 ),
		       Value(1.0110,  0.0130   ),
		       Value(0.9960,  0.0060 ) ,
		       Value(0.9940, 0.0070),
		       Value(0.9820, 0.0060) };
  

  Value CEM_trig[13] = { Value(0.9620,   0.0070  ),
			 Value(0.9760,   0.0060   ),
			 Value(0.9790,   0.0040  ),
			 Value(0.9590,   0.0070   ),
			 Value(0.9600 ,  0.0020  ),
			 Value(0.9590 ,  0.0020     ),    
			 Value(0.9610  , 0.0040   ),
			 Value(0.9600 ,  0.0030     ),
			 Value(0.9570 ,  0.0030   ),
			 Value(0.9600 ,  0.0300  ),
			 Value(0.9630 ,  0.0050),
			 Value(0.9610, 0.0050 ),
			 Value(0.9620, 0.0030) };



  Value CMUP_ID[13] = { Value(0.9360  ,  0.0055    ),
		       Value(0.9286  ,  0.0050 ),
		       Value(0.9174  ,  0.0064  ),
		       Value(0.9312  ,  0.0078  ),
		       Value(0.9400  ,  0.0080   ),
		       Value(0.9320  ,  0.0070  ),
		       Value(0.8990  ,  0.0080  ),
		       Value(0.9190  ,  0.0090   ),
		       Value(0.9240  ,  0.0070  ),
		       Value(0.9100  ,  0.0180   ),
		       Value( 0.8970   , 0.0090 ) ,
		       Value( 0.8950, 0.0120 ),
		       Value ( 0.9010, 0.0090)};
  

  Value CMUP_trig[13] = { Value(0.9015   ,  0.0044     ),
			 Value(0.9191   ,  0.0039   ),
			 Value(0.9179   ,  0.0048    ),
			 Value(0.9130   ,  0.0061  ),
			 Value(0.9270  ,   0.0067        ),  
			  Value(0.8655, 0.0072 ), // the value corrected by Veronica Value(  0.8520  ,   0.0089   ),
			 Value(0.8623  ,   0.0098 ),
			 Value(0.8417  ,   0.0120   ),
			 Value(0.8284  ,   0.0091    ),
			 Value(0.8943  ,   0.0222   ),
			  Value( 0.8790   ,  0.0100 ) ,
			 Value(0.8939  ,   0.0134   ),
			 Value( 0.8885   ,  0.0097 )   };


  //combination of arches + ms(ks):  0.765 * ar + 0.235* ms
  Value CMX_ID[13] = { Value( 1.0098,  0.0057  ),
		       Value(0.9744,  0.0074   ),
		       Value(0.9781 , 0.0089   ),
		       Value(0.9704 , 0.0116  ),
		       Value(0.9565 , 0.0132   ),
		       Value(0.9655 , 0.0110   ),
		       Value( 0.9456 , 0.0113 ),
		       Value(  0.9577 , 0.0135   ),
		       Value( 0.9466 , 0.0115   ),
		       Value(0.9910 , 0.0190   ),
		       Value(  0.9570 , 0.0100   ), 
		       Value(0.9381 , 0.0157   ),  // these are not checked against the txt file
		       Value(  0.9338 , 0.0105   ) }; // these are not checked against the txt file
  

  Value CMX_trig[13] = { Value(0.9665  ,   0.0036  ),
			 Value(0.9120  ,   0.0063   ),
			 Value(0.9049   ,  0.0079   ),
			 Value(0.8839, 0.0087 ), //corrected by Veronica, Value(0.9311   ,  0.0076   ),
			 Value(0.8659, 0.0084 ), //corrected by Veronica, Value(0.8945   ,  0.0112  ),
			 Value(  0.9294   ,  0.0089   ),
			 Value(0.9272  ,   0.0098     ),
			 Value(0.9002  ,   0.0140  ),
			 Value( 0.8926  ,   0.0094  ),
			 Value(0.8904   ,  0.0270  ),
			 Value(  0.9226  ,   0.0128 ) , 
			 Value(0.8943   ,  0.0235  ),
			 Value(  0.8975  ,   0.0142 )   };


  // calculate efficiencies for p0 separately
  Value CEM_p0 = CEM_ID[0]*CEM_trig[0];
  Value CMUP_p0 = CMUP_ID[0]*CMUP_trig[0];
  Value CMX_p0 = CMX_ID[0]*CMX_trig[0];

  // average efficiency for everything else:
  Value CEM_av(0.), CMUP_av(0.), CMX_av(0.), CMX_rs(0.);
  double totalLumi_cem(0.), totalLumi_cmx(0.);

  for (int i=1; i<13; i++){

    CEM_av += (CEM_ID[i]*CEM_trig[i]*Lumi_cem[i]);
    CMUP_av += (CMUP_ID[i]*CMUP_trig[i]*Lumi_cem[i]);
    CMX_av += (CMX_ID[i]*CMX_trig[i]*Lumi_cmx[i]);
    totalLumi_cem += Lumi_cem[i];
    totalLumi_cmx += Lumi_cmx[i];

  }//i

  CEM_av /= totalLumi_cem;
  CMUP_av /= totalLumi_cem;
  CMX_rs = CMX_av / totalLumi_cem;
  CMX_av /= totalLumi_cmx;

  cout << "Period 0: " << endl;
  cout << "CEM_p0 = " << CEM_p0 << endl; 
  cout << "CMUP_p0 = " << CMUP_p0 << endl; 
  cout << "CMX_p0 = " << CMX_p0 << endl; 

  cout << "Periods 1-17: (uncorrelated errors) " << endl;
  cout << "CEM_av = " << CEM_av << endl; 
  cout << "CMUP_av = " << CMUP_av << endl; 
  cout << "CMX_av = " << CMX_av << endl; 
  cout << "CMX_rs = " << CMX_rs << "      - this value can/should be used with the total cem/cmup luminosity " << endl; 

  cout << " *************************** " << endl;
  cout << "Total Luminosity = " << Lumi_cem[0] + totalLumi_cem << endl;

}
