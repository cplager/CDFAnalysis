#! /usr/bin/env bash

time ./combineST.pl -output comb/Wbottom_j2.txt  MEfiles/Wbottom_2jet.txt       NNfiles/Wbb_j2t?.txt       LFfiles/*Wbb_j2.txt
time ./combineST.pl -output comb/Wbottom_j3.txt  MEfiles/Wbottom_3jet.txt       NNfiles/Wbb_j3t1.txt       LFfiles/*Wbb_j3.txt
time ./combineST.pl -output comb/Wcharm_j2.txt   MEfiles/Wcharm_2jet.txt        NNfiles/Wc*_j2t?.txt       LFfiles/*Wcc_j2.txt
time ./combineST.pl -output comb/Wcharm_j3.txt   MEfiles/Wcharm_3jet.txt        NNfiles/Wc*_j3t1.txt       LFfiles/*Wcc_j3.txt
time ./combineST.pl -output comb/Wlight_j2.txt   MEfiles/Wlight_2jet.txt        NNfiles/Wlight_j2t?.txt    LFfiles/*mistag_j2.txt
time ./combineST.pl -output comb/Wlight_j3.txt   MEfiles/Wlight_3jet.txt        NNfiles/Wlight_j3t1.txt    LFfiles/*mistag_j3.txt
time ./combineST.pl -output comb/Zjets_j2.txt    MEfiles/Zjets_2jet.txt         NNfiles/ZHFjets_j2t?.txt   LFfiles/ztop*_j2.txt
time ./combineST.pl -output comb/Zjets_j3.txt    MEfiles/Zjets_3jet.txt         NNfiles/ZHFjets_j3t1.txt   LFfiles/ztop*_j3.txt
time ./combineST.pl -output comb/diboson_j2.txt  MEfiles/{WW,WZ,ZZ}_2jet.txt    NNfiles/diboson_j2t?.txt   LFfiles/itop??_j2.txt
time ./combineST.pl -output comb/diboson_j3.txt  MEfiles/{WW,WZ,ZZ}_3jet.txt    NNfiles/diboson_j3t1.txt   LFfiles/itop??_j3.txt
time ./combineST.pl -output comb/ttbar_j2.txt    MEfiles/tt{lj,dil}_2jet.txt    NNfiles/ttbar_j2t?.txt     LFfiles/ttop75_j2.txt
time ./combineST.pl -output comb/ttbar_j3.txt    MEfiles/tt{lj,dil}_3jet.txt    NNfiles/ttbar_j3t1.txt     LFfiles/ttop75_j3.txt
time ./combineST.pl -output comb/schan_j2.txt    MEfiles/schan_2jet.txt         NNfiles/schan_j2t?.txt     LFfiles/stop00_j2.txt
time ./combineST.pl -output comb/schan_j3.txt    MEfiles/schan_3jet.txt         NNfiles/schan_j3t1.txt     LFfiles/stop00_j3.txt
time ./combineST.pl -output comb/tchan_j2.txt    MEfiles/tchan_2jet.txt         NNfiles/tchan_j2t?.txt     LFfiles/stopm0_j2.txt
time ./combineST.pl -output comb/tchan_j3.txt    MEfiles/tchan_3jet.txt         NNfiles/tchan_j3t1.txt     LFfiles/stopm0_j3.txt
time ./combineST.pl -output comb/nonW_j2.txt     MEfiles/nonW_2jet.txt          NNfiles/nonW_j2t?.txt      LFfiles/nonw1sample_j2.txt
time ./combineST.pl -output comb/nonW_j3.txt     MEfiles/nonW_3jet.txt          NNfiles/nonW_j3t1.txt      LFfiles/nonw1sample_j3.txt
