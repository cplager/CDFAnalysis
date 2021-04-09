#!/bin/csh -f

echo Fixing DCACHE by adding /opt/d-cache/dcap/lib to path
setenv LD_LIBRARY_PATH /opt/d-cache/dcap/lib/:$LD_LIBRARY_PATH

echo ttbar, jes = 1.0
shyft_pretag inputfiles_load=ttbar_madgraph_7TeV.txt outputfile=ttbar_jes100 maxdump=10 minnjets=0 jetscale=1.0 fitnorm=1 >&! ttbar_jes100.log

echo W+jets, jes = 1.0
shyft_pretag inputfiles_load=wjets_madgraph_7TeV_preprod.txt outputfile=wjets_jes100 maxdump=10 minnjets=0 jetscale=1.0 fitnorm=1 >&! wjets_jes100.log

#-----------------------------------------------

# echo ttbar, jes = 1.1
# shyft_pretag inputfiles_load=ttbar_madgraph_7TeV.txt outputfile=ttbar_jes110 maxdump=10 minnjets=0 jetscale=1.1 fitnorm=1 >&! ttbar_jes110.log

# echo W+jets, jes = 1.1
# shyft_pretag inputfiles_load=wjets_madgraph_7TeV_preprod.txt outputfile=wjets_jes110 maxdump=10 minnjets=0 jetscale=1.1 fitnorm=1 >&! wjets_jes110.log

# echo ttbar, jes = 0.9
# shyft_pretag inputfiles_load=ttbar_madgraph_7TeV.txt outputfile=ttbar_jes090 maxdump=10 minnjets=0 jetscale=0.9 fitnorm=1 >&! ttbar_jes090.log

# echo W+jets, jes = 0.9
# shyft_pretag inputfiles_load=wjets_madgraph_7TeV_preprod.txt outputfile=wjets_jes090 maxdump=10 minnjets=0 jetscale=0.9 fitnorm=1 >&! wjets_jes090.log
