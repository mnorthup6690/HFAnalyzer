import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        'root://cmsxrootd.fnal.gov//store/user/mnguyen/Hydjet_Quenched_MinBias_5020GeV/HydjetMB_740pre8_MCHI2_74_V3_53XBS_DIGI-RAW/6da45e4e90741bc03dbd9aec5f36c050/step2_DIGI_L1_DIGI2RAW_HLT_RAW2DIGI_L1Reco_100_1_nRy.root'
    )
)

process.demo = cms.EDAnalyzer("HFAnalyzer",

	HcalDigi_label = cms.InputTag("simHcalUnsuppressedDigis","","HLT")
#	HcalDigi_label = cms.InputTag("simHcalUnsuppressedDigis",
#	"HFUpgradeDigiCollection","HLT")
)


process.p = cms.Path(process.demo)
