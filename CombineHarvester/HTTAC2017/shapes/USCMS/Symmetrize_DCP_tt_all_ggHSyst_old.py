#!/usr/bin/env python
import ROOT
from ROOT import *
import re
from array import array

islog=1
unrollSV=1

#file=ROOT.TFile("htt_tt.inputs-sm-13TeV-MELA-5040-Tight_DCP.root","r")
#file1=ROOT.TFile("htt_tt.inputs-sm-13TeV-MELA-5040-Tight_DCP_DCP_test.root","recreate")
# htt_tt.inputs-sm-13TeV-2D_fa2.root
file=ROOT.TFile("htt_tt.inputs-sm-13TeV-MELA-5040-Tight_ggHSyst.root","r")
file1=ROOT.TFile("htt_tt.inputs-sm-13TeV-MELA-5040-Tight_ggHSyst_DCP_DCP_test.root","recreate")


file.cd()
dirList = gDirectory.GetListOfKeys()


for k1 in dirList:
      if "_D0_" in k1.GetName() and "_DCP" not in k1.GetName():
            h1 = k1.ReadObj()
            nom=k1.GetName()
            file1.mkdir(nom+"_DCP_plus")
            print " bkg DCP_plus: ", k1.GetName(), "  -> output: ", nom+"_DCP_plus"
            h1.cd()
            dirList2 = gDirectory.GetListOfKeys()
            name_last=""
            for k2 in dirList2:
                  if (k2.GetName()!=name_last):
                        h2 = k2.ReadObj()
                        h3=h2.Clone()
                        file1.cd(nom+"_DCP_plus")
                        h3.SetName(k2.GetName())
#                        print "    histo: ", k2.GetName(), " yield: ", h3.Integral()
                        h3.Scale(0.5)
#                        print "    -> rescaled  histo: ", k2.GetName(), " yield: ", h3.Integral()
                        if "ggH" not in h3.GetName() and "GGH" not in h3.GetName() and "qqH" not in h3.GetName():
                              h3.Write()
#                              print "       -> save  histo: ", k2.GetName(), " yield: ", h3.Integral()
                        name_last=k2.GetName()

for k1 in dirList:
      if "_D0_" in k1.GetName() and "_DCP" not in k1.GetName():
            h1 = k1.ReadObj()
            nom=k1.GetName()
            file1.mkdir(nom+"_DCP_minus")
            print " bkg DCP_minus: ", k1.GetName(), "  -> output: ", nom+"_DCP_minus"
            h1.cd()
            dirList2 = gDirectory.GetListOfKeys()
            name_last=""
            for k2 in dirList2:
                  if (k2.GetName()!=name_last):
                        h2 = k2.ReadObj()
                        h3=h2.Clone()
                        file1.cd(nom+"_DCP_minus")
                        h3.SetName(k2.GetName())
#                        print "    histo: ", k2.GetName(), " yield: ", h3.Integral()
                        h3.Scale(0.5)
#                        print "    -> rescaled  histo: ", k2.GetName(), " yield: ", h3.Integral()
                        if "ggH" not in h3.GetName() and "GGH" not in h3.GetName()  and "qqH" not in h3.GetName():
                              h3.Write()
#                              print "       -> save  histo: ", k2.GetName(), " yield: ", h3.Integral()
                        name_last=k2.GetName()



for k1 in dirList:
      if "_D0_" in k1.GetName() and "_DCPm" in k1.GetName():
#         print k1.GetName()
         print " signal DCP_minus: ", k1.GetName()
         h1 = k1.ReadObj()
#         nom=k1.GetName().split("DCP")[0]
         nom=k1.GetName().replace("_DCPm","")
         #file1.mkdir(nom+"DCP_minus")
         h1.cd()
         dirList2 = gDirectory.GetListOfKeys()
         print "  -> goto dir: ",nom+"_DCP_minus"
         for k2 in dirList2:
            h2 = k2.ReadObj()
            h3=h2.Clone()
#            print "  -> goto dir: ",nom+"DCP_minus","  -> and save histo: ", h2.GetName() , " as", k2.GetName()
            file1.cd(nom+"_DCP_minus")
            h3.SetName(k2.GetName())
            if "ggH" in h3.GetName() or "GGH" in h3.GetName() or "qqH" in h3.GetName():
               h3.Write()

for k1 in dirList:
      if "_D0_" in k1.GetName() and "_DCPp" in k1.GetName():
#	 print k1.GetName()
         print " signal DCP_plus: ", k1.GetName()
         h1 = k1.ReadObj()
#         nom=k1.GetName().split("DCP")[0]
         nom=k1.GetName().replace("_DCPp","")
         #file1.mkdir(nom+"DCP_plus")
         h1.cd()
         dirList2 = gDirectory.GetListOfKeys()
         for k2 in dirList2:
            h2 = k2.ReadObj()
            h3=h2.Clone()
            file1.cd(nom+"_DCP_plus")
            h3.SetName(k2.GetName())
            if "ggH" in h3.GetName() or "GGH" in h3.GetName() or "qqH" in h3.GetName():
               h3.Write()

