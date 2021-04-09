#! /usr/bin/env bash

time ./combineST.pl -output comb/Wbottom.txt  MEfiles/Wbottom_?jet.txt  NNfiles/Wbb_j?t?.txt       LFfiles/*Wbb_j?.txt
time ./combineST.pl -output comb/Wcharm.txt   MEfiles/Wcharm_?jet.txt   NNfiles/Wc*_j?t?.txt       LFfiles/*Wcc_j?.txt
time ./combineST.pl -output comb/Wlight.txt   MEfiles/Wlight_?jet.txt   NNfiles/Wlight_j?t?.txt    LFfiles/*mistag_j?.txt
time ./combineST.pl -output comb/Zjets.txt    MEfiles/Zjets_?jet.txt    NNfiles/ZHFjets_j?t?.txt   LFfiles/ztop*_j?.txt
time ./combineST.pl -output comb/WW.txt       MEfiles/WW_?jet.txt       NNfiles/diboson_j?t?.txt   LFfiles/itopww_j?.txt
time ./combineST.pl -output comb/WZ.txt       MEfiles/WZ_?jet.txt       NNfiles/diboson_j?t?.txt   LFfiles/itopwz_j?.txt
time ./combineST.pl -output comb/ZZ.txt       MEfiles/ZZ_?jet.txt       NNfiles/diboson_j?t?.txt   LFfiles/itopzz_j?.txt
time ./combineST.pl -output comb/ttlj.txt     MEfiles/ttlj_?jet.txt     NNfiles/ttbar_j?t?.txt     LFfiles/ttop75_j?.txt
time ./combineST.pl -output comb/ttdil.txt    MEfiles/ttdil_?jet.txt    NNfiles/ttbar_j?t?.txt     LFfiles/ttop75_j?.txt
time ./combineST.pl -output comb/schan.txt    MEfiles/schan_?jet.txt    NNfiles/schan_j?t?.txt     LFfiles/stop00_j?.txt
time ./combineST.pl -output comb/tchan.txt    MEfiles/tchan_?jet.txt    NNfiles/tchan_j?t?.txt     LFfiles/stopm0_j?.txt
time ./combineST.pl -output comb/nonW.txt     MEfiles/nonW_?jet.txt     NNfiles/nonW_j?t?.txt      LFfiles/nonw1sample_j?.txt
