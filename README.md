# HTT_AC
HTT AC code

# Running fa3 limits:

creating datacards

    newFolder=AC_tt_mt_et_CR_s3HZZfix_JHUscalar_intAntiSym_fa3_JHU_ggH_2
    MorphingSM2016_D0merged_DCP_ggHSyst --output_folder=${newFolder} --postfix="-2D_D0merged_mttmerged_mjjmerged_DCP_fa2_normJHU" --control_region=1 --manual_rebin=false --real_data=true --mm_fit=false --ttbar_fit=true

for some reason the ttbar dir is not created. This got broken few days ago but I did not fix this since the lack of time. I will fix this is Htt*.py file once I am back. ttbar CR anyway has a tiny contribution from signal so just copy the files from here:

    cp -r /afs/cern.ch/user/s/senka/public/forYurii/htt_ttbar_1_13TeV output/AC_tt_mt_et_CR_s3HZZfix_JHUscalar_intAntiSym_fa3_JHU_ggH_2/
    cp -r /afs/cern.ch/user/s/senka/public/forYurii/ttbar output/AC_tt_mt_et_CR_s3HZZfix_JHUscalar_intAntiSym_fa3_JHU_ggH_2/
    cp /afs/cern.ch/user/s/senka/public/forYurii/*ttbar* output/AC_tt_mt_et_CR_s3HZZfix_JHUscalar_intAntiSym_fa3_JHU_ggH_2/cmb/125/
    cp /afs/cern.ch/user/s/senka/public/forYurii/htt_input_ttbar1.root output/AC_tt_mt_et_CR_s3HZZfix_JHUscalar_intAntiSym_fa3_JHU_ggH_2/cmb/common/
    MorphingSM2016_D0merged_DCP_ggHSyst --output_folder=${newFolder} --postfix="-2D_D0merged_mttmerged_mjjmerged_DCP_fa2_normJHU" --control_region=1 --manual_rebin=false --real_data=true --mm_fit=false --ttbar_fit=true

now in ./cmb/125/htt_tt_24_13TeV.txt, ./tt/125/htt_tt_24_13TeV.txt,./cmb/125/htt_tt_28_13TeV.txt, ./tt/125/htt_tt_28_13TeV.txt remove CMS_scale_t_3prong_13TeV systeamtics for these bins since for some strange reason the combine is refusing to use them.. Will fix this in Htt*.py file once I am back.

create combined cards and workspaces:

    cd output/AC_tt_mt_et_CR_s3HZZfix_JHUscalar_intAntiSym_fa3_JHU_ggH_2
    combineTool.py -M T2W -i {cmb,et,mt,tt}/* -o workspace.root --parallel 18
    combineTool.py -M T2W -m 125 -P HiggsAnalysis.CombinedLimit.FA3_Interference_JHU_ggHSyst:FA3_Interference_JHU_ggHSyst -i tt/125/combined.txt.cmb -o fa03_Interference_Workspace_tt.root
    combineTool.py -M T2W -m 125 -P HiggsAnalysis.CombinedLimit.FA3_Interference_JHU_ggHSyst:FA3_Interference_JHU_ggHSyst -i mt/125/combined.txt.cmb -o fa03_Interference_Workspace_mt.root
    combineTool.py -M T2W -m 125 -P HiggsAnalysis.CombinedLimit.FA3_Interference_JHU_ggHSyst:FA3_Interference_JHU_ggHSyst -i et/125/combined.txt.cmb -o fa03_Interference_Workspace_et.root
    combineTool.py -M T2W -m 125 -P HiggsAnalysis.CombinedLimit.FA3_Interference_JHU_ggHSyst:FA3_Interference_JHU_ggHSyst -i cmb/125/combined.txt.cmb -o fa03_Interference_Workspace_cmb.root

# Running fa2 limits:
differences wrt running fa3 are:

    MorphingSM2016_D0merged_DCP_fa2 --output_folder=${newFolder} --postfix="-2D_D0merged_mttmerged_mjjmerged_DCP_D0hplus_normJHU" --control_region=1 --manual_rebin=false --real_data=true --mm_fit=false --ttbar_fit=true
    ...
    combineTool.py -M T2W -m 125 -P HiggsAnalysis.CombinedLimit.FA2_Interference_JHU:FA2_Interference_JHU -i tt/125/combined.txt.cmb -o fa03_Interference_Workspace_tt.root
    combineTool.py -M T2W -m 125 -P HiggsAnalysis.CombinedLimit.FA2_Interference_JHU:FA2_Interference_JHU -i mt/125/combined.txt.cmb -o fa03_Interference_Workspace_mt.root
    combineTool.py -M T2W -m 125 -P HiggsAnalysis.CombinedLimit.FA2_Interference_JHU:FA2_Interference_JHU -i et/125/combined.txt.cmb -o fa03_Interference_Workspace_et.root
    combineTool.py -M T2W -m 125 -P HiggsAnalysis.CombinedLimit.FA2_Interference_JHU:FA2_Interference_JHU -i cmb/125/combined.txt.cmb -o fa03_Interference_Workspace_cmb.root



# files with running limits commands:
for 1D scans:
    /afs/cern.ch/user/s/senka/public/forYurii/run*
    
for 2D scan (fa3):
    
    python ../../AC_kappaV_Multithreading_FITTER_exp_ggH_6k.py

for 2D scan (fa2):

    python ../../AC_kappaV_Multithreading_FITTER_exp_6k.py
    
hadd outputs and plot:

    python ../../CombineHarvester/scripts/plotMultiDimFit_CVAC_fa3.py higgsCombineCvAC_FITTER_exp_m0p1to0p1_6k.0to7.MultiDimFit.mH125.root --x-title #mu_{V} --y-title f_{a3} --sm-exp SM_EXP --cms-sub ""


# datacards location:
    /hdfs/store/user/senka/HTT_stuff/datacards/
