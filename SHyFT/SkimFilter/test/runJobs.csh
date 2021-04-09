


#cmsRun SHYFTAnalyzer_cfg.py print files_load="ttbar_pythia_10TeV_v1.txt" output="file:/uscms_data/d2/slaunwhj/scratch0/ljmet_skimmedEvents_all.root" maxEvents=-1


#cmsRun SHYFTAnalyzer_cfg.py print files_load="ttbar_7TeV.list" output="file:/uscms_data/d2/slaunwhj/scratch0/ttbar_7TeV_skimmed_minJets1_jes0.9.root" jetScale=0.9 maxEvents=-1 >&! ttbar_jes0.9_all.log 

#cmsRun SHYFTAnalyzer_cfg.py print files_load="wjets.list" output="file:/uscms_data/d2/slaunwhj/scratch0/wjets_7TeV_skimmed_minJets1_jes0.9.root" jetScale=0.9 maxEvents=-1
#cmsRun SHYFTAnalyzer_cfg.py print files_load="wjets.list" output="file:/uscms_data/d2/slaunwhj/scratch0/wjets_7TeV_skimmed_minJets1_jes1.1.root" jetScale=1.1 maxEvents=-1 >&! wjets_jes1.1_all.log


#cmsRun SHYFTAnalyzer_cfg.py print files_load="ttbar_7TeV.list" output="file:/uscms_data/d2/slaunwhj/scratch0/ttbar_7TeV_skimmed_minJets1_jes1.1.root" jetScale=1.1 maxEvents=10 trigRequired=0 >&! ttbar_jes1.1_noTrig_all.log
cmsRun SHYFTAnalyzer_cfg.py print files_load="ttbar_7TeV.list" output="file:/uscms_data/d2/slaunwhj/scratch0/ttbar_7TeV_skimmed_minJets1_jes1.1.root" jetScale=1.1 maxEvents=-1 trigRequired=0 >&! ttbar_jes1.1_noTrig_all.log
