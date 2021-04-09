import FWCore.ParameterSet.Config as cms
import FWCore.PythonUtilities.LumiList as LumiList

process = cms.Process("SHYFTFilterTheEvents")
process.load("SHyFT.SkimFilter.SHYFTAnalyzer_cfi")

process.shyftAnalyzer.selectionDetails.minJets = cms.int32(0)
process.shyftAnalyzer.selectionDetails.pvSelector.maxZ = 24.0
process.shyftAnalyzer.verbose = cms.untracked.bool(False)
process.shyftAnalyzer.trigRequired = cms.untracked.bool(True)

process.maxEvents = cms.untracked.PSet(
	input = cms.untracked.int32(-1)
	)

process.MessageLogger = cms.Service(
	"MessageLogger",
	cout = cms.untracked.PSet(threshold = cms.untracked.string('WARNING')
				  ),
	categories     = cms.untracked.vstring('SHyFTSkimFilter'),	
	destinations   = cms.untracked.vstring('cout')
	)


process.source = cms.Source("PoolSource",
			    skipEvents = cms.untracked.uint32(0),
			    fileNames = cms.untracked.vstring(),
                            lumisToProcess = cms.untracked.VLuminosityBlockRange()
			    )

readFiles = []

readFiles.extend( [
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_9_1_6MH.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_8_1_lNx.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_87_1_u61.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_86_1_z2j.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_85_1_Ha6.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_84_1_zsa.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_83_1_oYc.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_82_1_g3y.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_81_1_H5x.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_80_1_Wmk.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_7_1_WNU.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_79_1_r6m.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_78_4_sTb.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_77_1_miZ.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_76_1_2ou.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_75_1_VIE.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_74_1_CWy.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_73_1_UHK.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_72_1_DFT.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_71_1_vDq.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_70_1_T1D.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_6_1_DPq.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_69_1_DLv.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_68_1_V2l.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_67_1_fyw.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_66_1_WZQ.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_65_1_N47.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_64_1_e2O.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_63_1_2CK.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_62_1_ukC.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_61_1_Rzq.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_60_1_EP4.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_5_1_q0m.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_59_2_vPj.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_58_2_PoV.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_57_1_SwJ.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_56_1_ZdR.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_55_1_1ws.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_54_1_q1L.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_53_1_Jek.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_52_2_mSg.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_51_2_QBl.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_50_1_PS0.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_4_1_S3z.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_49_1_VWO.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_48_1_ebv.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_47_1_1f3.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_46_1_PSj.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_45_1_JFx.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_44_1_x1N.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_43_1_ntc.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_42_2_O4a.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_41_1_MdY.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_40_1_fOG.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_3_1_As7.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_39_1_AqV.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_38_1_fq7.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_37_1_3iR.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_36_1_W3b.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_35_1_3qa.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_34_1_TtF.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_33_1_vWC.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_32_1_FAN.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_31_1_w91.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_30_1_TRX.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_2_1_kOV.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_29_1_rbo.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_28_1_50Y.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_27_1_F7W.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_26_1_wMU.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_25_1_VJz.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_24_1_CHq.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_23_2_uYN.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_22_1_g0Y.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_21_1_YZ8.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_20_1_xLt.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_1_1_mkb.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_19_1_9Pa.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_18_2_HKH.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_17_1_TUy.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_16_1_bPv.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_15_1_0Sr.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_14_1_i8B.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_13_1_Xpl.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_12_1_9nl.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_11_1_wWM.root',
       '/store/user/sboutle/Mu/SHYFT_pat362_muskim_selv3_Aug25/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_10_1_dAO.root' 
 ] );



for file in readFiles:
#    process.source.fileNames.append('dcap:///pnfs/cms/WAX/11'+file)
    process.source.fileNames.append(file)


# get JSON file correctly parced
JSONfile = 'json/Cert_132440-143336_7TeV_StreamExpress_Collisions10_JSON_TopAug26_Filtered_138564-143336_Extend_143337-143835_Recover_noESDCS.txt'
#JSONfile = 'json/Cert_132440-143179_7TeV_StreamExpress_Collisions10_JSON.txt'
myList = LumiList.LumiList (filename = JSONfile).getCMSSWString().split(',')
for entry in myList :
    process.source.lumisToProcess.append(str(entry))


process.out = cms.OutputModule("PoolOutputModule",
			       outputCommands = cms.untracked.vstring('keep *'),
			       # Disable fast cloning to resolve 34X issue
			       # fastCloning = cms.untracked.bool(False),
			       fileName = cms.untracked.string("/uscms_data/d2/sboutle/dataskim_selv3/dataSkim_selv3_prompt_143835_Aug26.root"),
			       maxSize = cms.untracked.int32(1048576),

			       # This is how you require a filter
			       # for the path
			       SelectEvents = cms.untracked.PSet(
	SelectEvents = cms.vstring('p1')
	)
			       )

process.p1 = cms.Path(
	process.shyftAnalyzer
	)

process.outpath = cms.EndPath(process.out)




