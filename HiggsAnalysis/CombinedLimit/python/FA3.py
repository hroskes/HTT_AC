from HiggsAnalysis.CombinedLimit.PhysicsModel import *
import re

class FA3(PhysicsModel):
    def doParametersOfInterest(self):
        """Create POI and other parameters, and define the POI set."""
        xsecs = {
          "sigma1_HZZ": 290.58626,
          "sigma3_HZZ": 581.17253,
          "sigma1_VBF": 968.674,
          "sigma3_VBF": 10909.54,
          "sigma1_ZH":  9022.36,
          "sigma3_ZH":  434763.7,
          "sigma1_WH":  30998.54,
          "sigma3_WH":  2028656
        }

        #self.modelBuilder.doVar("muf[1.0,0.0,10.0]")
        self.modelBuilder.doVar("fa03[0.0,0.0,1.0]");
        self.modelBuilder.doVar("mu[1.0,0.0,10.0]");
        self.modelBuilder.doSet("POI","fa03,mu")
        #self.modelBuilder.out.var("muf").setAttribute("flatParam")

        self.modelBuilder.factory_('expr::smCoupling("@0*(1-@1)", mu,fa03)')
        self.modelBuilder.factory_('expr::bsmCoupling("@0*@1", mu,fa03)')
    def getYieldScale(self,bin,process):
        if process in ["ggH_htt",]:
            #return 'muf'
            return 'mu'
        if process in ["qqH_htt", "WH_htt", "ZH_htt"]:
            return 'smCoupling'
        if process in ["qqH_htt_0M", "WH_htt_0M", "ZH_htt_0M"]:
            return 'bsmCoupling'
        return 1


FA3 = FA3()
