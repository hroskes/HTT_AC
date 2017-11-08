#include <iostream>
#include <vector>
#include <map>
#include "TCanvas.h"
#include "TLegend.h"
#include "TGraphErrors.h"

#include "TLatex.h"
#include "TH2F.h"
#include "TStyle.h"
#include "THStack.h"
#include "TLine.h"
#include "TColor.h"
#include "TGraphAsymmErrors.h"

#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/format.hpp"
#include "boost/program_options.hpp"
#include "boost/filesystem.hpp"
#include "boost/regex.hpp"

// #include "Core/interface/Plot.h"
// #include "Core/interface/TextElement.h"
// #include "Utilities/interface/FnRootTools.h"
// #include "HiggsTauTau/interface/HTTStatTools.h"
// #include "HiggsTauTau/interface/HTTPlotTools.h"
// #include "HiggsTauTau/interface/HTTAnalysisTools.h"
// #include "HiggsTauTau/interface/mssm_xs_tools.h"

#include "CombineHarvester/CombineTools/interface/CombineHarvester.h"
#include "CombineHarvester/CombineTools/interface/SOverBTools.h"
#include "CombineHarvester/CombineTools/interface/TFileIO.h"

namespace po = boost::program_options;

using namespace std;
// using namespace ic;

void SetMCStackStyle(TH1F & ele, unsigned color) {
  ele.SetFillColor(color);
  ele.SetFillStyle(1001);
  ele.SetLineWidth(2);
  return;
}

void SetDataStyle(TH1F & ele) {
  ele.SetMarkerColor(1);
  ele.SetLineColor(1);
  ele.SetFillColor(1);
  ele.SetFillStyle(0);
  ele.SetLineWidth(2);
  ele.SetMarkerStyle(20);
  ele.SetMarkerSize(1.1);
  return;
}

void SetDataStyle(TGraphAsymmErrors & ele) {
  ele.SetMarkerColor(1);
  ele.SetLineColor(1);
  ele.SetFillColor(1);
  ele.SetFillStyle(0);
  ele.SetLineWidth(2);
  ele.SetMarkerStyle(20);
  ele.SetMarkerSize(1.1);
  return;
}

void LegendStyle(TLegend *legend) {
  legend->SetBorderSize(1);
  legend->SetTextFont(62);
  legend->SetLineColor(0);
  legend->SetLineStyle(1);
  legend->SetLineWidth(1);
  legend->SetFillColor(0);
  legend->SetFillStyle(1001);
}

int main(int argc, char* argv[]){
  // Plot::SetHTTStyle();

  string shape_file = "";
  string bin_id="em_nobtag";
  string mode="ggH";
  string output           = "";

  po::options_description config("configuration");
  po::variables_map vm;
  po::notify(vm);
  config.add_options()
    ("help,h",  "print the help message")
    ("shape_file,f",
      po::value<string>(&shape_file)->required())
    ("bin_id",
      po::value<string>(&bin_id))
    ("mode",
      po::value<string>(&mode))
    ("output",
      po::value<string>(&output)->required(),
      "[REQUIRED] output name (no extension)");
  // HTTPlot plot;
  // config.add(plot.GenerateOptions(""));
  po::store(po::command_line_parser(argc, argv).
     options(config).allow_unregistered().run(), vm);
  po::notify(vm);

  TFile *f = new TFile(shape_file.c_str()); 
  //TH1F * data = new TH1F("data","data",195,0,3900); 
  TH1F * total_bkg = (TH1F*)(f->Get((bin_id+"/total_bkg").c_str()));
  std::map<string,TH1F*> ggH;
  vector<string> masses = {"90","100","110","120","130","140","160","180", "200", "250", "300", "350", "400", "450", "500", "600", "700", "800", "900","1000","1200","1400","1500","1600","1800","2000","2300","2600","2900","3200"};
  for(auto const& mass : masses){
    ggH[mass] = (TH1F*)(f->Get((bin_id+"/"+mode+mass).c_str()));
  }
  
  //string bins[] = {"em_nobtag"};
//  auto bins = cmb.SetFromObs(std::mem_fn(&ch::Observation::bin));
  map<string, ch::SOverBInfo> weights;
  map<string, double> sob_;
  map<string, double> sosb_;
  for (auto const& mass : masses) {
    weights[mass] = ch::SOverBInfo(ggH[mass], total_bkg, 3500, 0.682);
    sob_[mass] = weights[mass].s/(sqrt(weights[mass].b));
    sosb_[mass] = weights[mass].s/(weights[mass].b+weights[mass].s);
    std::cout << " Mass: " << mass << "  " << weights[mass].x_lo << "-" << weights[mass].x_hi << "\n";
    std::cout << "  sig:   " << (weights[mass].x_hi-weights[mass].x_lo)/2. << "\n";
    std::cout << "  s:     " << weights[mass].s << "\n";
    std::cout << "  b:     " << weights[mass].b << "\n";
    std::cout << "  s/s+b: " << weights[mass].s/(weights[mass].s + weights[mass].b) << "\n";
    std::cout << "  s/root(b): " << weights[mass].s/(sqrt(weights[mass].b)) << "\n";
  }

  TGraph *sob_graph = new TGraph(masses.size());
  TGraph *sosb_graph = new TGraph(masses.size());
  sob_graph->SetName("sob_graph");
  sosb_graph->SetName("sosb_graph");

  for(unsigned i = 0; i< masses.size(); ++i){
    sob_graph->SetPoint(i,atoi(masses.at(i).c_str()),sob_[masses.at(i)]);
    sosb_graph->SetPoint(i,atoi(masses.at(i).c_str()),sosb_[masses.at(i)]);
  }
 
  TFile * outfile = new TFile(output.c_str(),"RECREATE");
  sob_graph->Write();
  sosb_graph->Write();
  outfile->Close();

 

/*
  for (auto const& bin : bins) {
    // Has to be an explicit move here. Any function returning a ref to (*this) is an lvalue
    ch::CombineHarvester subset = std::move(cmb.cp().bin({bin}));
    double ratio = weights[bin].s/(weights[bin].s + weights[bin].b);
    std::cout << "Ratio: " << ratio << std::endl;
    subset.ForEachObs([&ratio](ch::Observation * in){
      in->set_rate(in->rate()*ratio); });
    subset.ForEachProc([&ratio](ch::Process * in){
      in->set_rate(in->rate()*ratio); });
  }
  double sig_yield_after = cmb.cp().signals().GetRate();
  double scale_all = sig_yield_before / sig_yield_after;*/
 // std::cout << "Scale all distributions: " << scale_all << std::endl;
  // cmb.ForEachObs([&scale_all](ch::Observation * in){
  //   in->set_rate(in->rate()*scale_all); });
  // cmb.ForEachProc([&scale_all](ch::Process * in){
  //   in->set_rate(in->rate()*scale_all); });

  // vector<string> samples = {"ZTT","QCD","W","ZL","ZJ","ZLL","VV","TT","Ztt","Fakes","EWK","ttbar"};

  // vector<string> signal_procs = {"ggH", "qqH", "VH"};
  // if (mssm) signal_procs = {"ggH","bbH"};

/*  TCanvas canv;
  canv.SetFillColor      (0);
  canv.SetBorderMode     (0);
  canv.SetBorderSize     (10);
  canv.SetLeftMargin     (0.17);
  canv.SetRightMargin    (0.05);
  canv.SetTopMargin      (0.08);
  canv.SetBottomMargin   (0.14);
  canv.SetFrameFillStyle (0);
  canv.SetFrameLineStyle (0);
  canv.SetFrameBorderMode(0);
  canv.SetFrameBorderSize(10);
  canv.SetFrameFillStyle (0);
  canv.SetFrameLineStyle (0);
  canv.SetFrameBorderMode(0);
  canv.SetFrameBorderSize(10);

  TH1F signal_hist = cmb.cp().signals().GetShape();
  SetMCStackStyle(signal_hist, kRed);
  signal_hist.SetFillColor(kRed);
  signal_hist.SetFillStyle(3004);
  signal_hist.SetLineColor(kRed);
  signal_hist.SetLineWidth(2);
  // signal_hist.SetTitle(("SM H("+plot.draw_signal_mass()+")#rightarrow#tau#tau").c_str());
  signal_hist.SetTitle("SM H(125#rightarrow#tau#tau");

  TH1F ztt_hist = cmb.cp().process({"ZTT","Ztt"}).GetShape();
  SetMCStackStyle(ztt_hist, TColor::GetColor(248,206,104));
  ztt_hist.SetTitle("Z#rightarrow#tau#tau");

  TH1F qcd_hist = cmb.cp().process({"QCD","Fakes"}).GetShape();
  SetMCStackStyle(qcd_hist, TColor::GetColor(250,202,255));
  qcd_hist.SetTitle("QCD");

  TH1F ewk_hist = cmb.cp().process({"W","ZL","ZJ","VV","EWK"}).GetShape();
  if (split_zll) ewk_hist = cmb.cp().process({"W","VV","EWK"}).GetShape();
  SetMCStackStyle(ewk_hist, TColor::GetColor(222, 90,106));
  ewk_hist.SetTitle("Electroweak");

  TH1F zll_hist = cmb.cp().process({"ZL","ZJ"}).GetShape();
  SetMCStackStyle(zll_hist, TColor::GetColor(100,182,232));
  zll_hist.SetTitle("Z#rightarrowll");

  TH1F top_hist = cmb.cp().process({"TT","ttbar"}).GetShape();
  SetMCStackStyle(top_hist, TColor::GetColor(155,152,204));
  top_hist.SetTitle("t#bar{t}");

  // total_hist will be used to draw the background error on the main plot
  TH1F total_hist = cmb.cp().backgrounds().GetShapeWithUncertainty(fitresult, 500);
  total_hist.SetMarkerSize(0);
  total_hist.SetFillColor(13);
  total_hist.SetFillStyle(3013);
  total_hist.SetLineWidth(1);
  total_hist.SetTitle("Bkg. uncertainty");

  // copy_hist removes the bin errors from total_hist, and will be used
  // to determine the data-bkg histogram for the inset plot
  TH1F copy_hist = total_hist;
  for (int i = 1; i <= copy_hist.GetNbinsX(); ++i) copy_hist.SetBinError(i, 0);

  // err_hist removes the bin contents from total_hist, and will be used
  // to draw the error band on the inset plot
  TH1F err_hist = total_hist;
  for (int i = 1; i <= err_hist.GetNbinsX(); ++i) err_hist.SetBinContent(i, 0);
  err_hist.SetMarkerSize(0.0);
  err_hist.SetFillColor(13);
  err_hist.SetFillStyle(3013);
  err_hist.SetLineWidth(3);
  err_hist.SetTitle("Bkg. uncertainty");

  TH1F data_hist = cmb.GetObservedShape();
  // TGraphAsymmErrors data_errors = setup.GetObservedShapeErrors();
  SetDataStyle(data_hist);
  // SetDataStyle(data_errors);
  data_hist.SetTitle("Observed");
  // if (plot.blind()) BlindHistogram(&data_hist, plot.x_blind_min(), plot.x_blind_max());

  TH1F diff_hist = data_hist;
  diff_hist.Add(&copy_hist, -1.);
  SetDataStyle(diff_hist);
  diff_hist.SetTitle("Data - background");

  vector<TH1F *> drawn_hists;
  drawn_hists.push_back(&qcd_hist);
  drawn_hists.push_back(&ewk_hist);
  drawn_hists.push_back(&zll_hist);
  drawn_hists.push_back(&ztt_hist);
  drawn_hists.push_back(&top_hist);
  drawn_hists.push_back(&signal_hist);
  drawn_hists.push_back(&data_hist);
  drawn_hists.push_back(&diff_hist);
  drawn_hists.push_back(&err_hist);
  drawn_hists.push_back(&total_hist);

  for (unsigned i = 0; i < drawn_hists.size(); ++i) {
     drawn_hists[i]->SetLineWidth(2);
     drawn_hists[i]->Scale(scale_all);
     drawn_hists[i]->Scale(1.0, "width");
  }


  THStack thstack("stack","stack");
  thstack.Add(&qcd_hist, "HIST");
  thstack.Add(&top_hist, "HIST");
  thstack.Add(&ewk_hist, "HIST");
  if (split_zll) thstack.Add(&zll_hist, "HIST");
  thstack.Add(&ztt_hist, "HIST");
  thstack.Add(&signal_hist, "HIST");

  // thstack.SetMaximum(thstack.GetMaximum()*1.1*plot.extra_pad());
  thstack.Draw();
  // thstack.GetXaxis()->SetTitle(plot.x_axis_label().c_str());
  // thstack.GetYaxis()->SetTitle(plot.y_axis_label().c_str());
  thstack.GetHistogram()->SetTitleSize  (0.05,"Y");
  thstack.GetHistogram()->SetTitleOffset(1.600,"Y");
  thstack.GetHistogram()->SetLabelOffset(0.014,"Y");
  thstack.GetHistogram()->SetLabelSize  (0.040,"Y");
  thstack.GetHistogram()->SetLabelFont  (42   ,"Y");
  thstack.GetHistogram()->SetTitleFont  (62   ,"Y");
  thstack.GetHistogram()->SetTitleSize  (0.05,"X");
  thstack.GetHistogram()->SetTitleOffset(1.100,"X");
  thstack.GetHistogram()->SetLabelOffset(0.014,"X");
  thstack.GetHistogram()->SetLabelSize  (0.040,"X");
  thstack.GetHistogram()->SetLabelFont  (42   ,"X");
  thstack.GetHistogram()->SetTitleFont  (62   ,"X");

  // canv.Update();
  total_hist.Draw("SAMEE2");
  data_hist.Draw("SAME");


  TLegend *legend = new TLegend(0.57,0.22,0.92,0.46,"","brNDC");
  legend->SetTextFont(620);
  legend->AddEntry(&signal_hist, "", "F");
  legend->AddEntry(&data_hist, "", "LP");
  legend->AddEntry(&ztt_hist, "", "F");
  if (split_zll) legend->AddEntry(&zll_hist, "", "F");
  legend->AddEntry(&ewk_hist, "", "F");
  legend->AddEntry(&top_hist, "", "F");
  legend->AddEntry(&qcd_hist, "", "F");
  LegendStyle(legend);
  legend->Draw();


  TPad padBack("padBack","padBack",0.55,0.55,0.975,0.956);//TPad must be created after TCanvas otherwise ROOT crashes
  padBack.SetFillColor(0);

  TPad pad("diff","diff",0.45,0.47,0.9763,0.959);//TPad must be created after TCanvas otherwise ROOT crashes
  pad.cd();
  pad.SetFillColor(0);
  pad.SetFillStyle(0);
  double sig_max_val = signal_hist.GetBinContent(signal_hist.GetMaximumBin());
  double dif_max_val = 0;
  for (int i = 1; i < diff_hist.GetNbinsX(); ++i) {
    dif_max_val = std::max(diff_hist.GetBinContent(i) + diff_hist.GetBinError(i), dif_max_val);
  }
  double err_max_val = 0;
  for (int i = 1; i < err_hist.GetNbinsX(); ++i) {
    err_max_val = std::max(err_hist.GetBinContent(i) + err_hist.GetBinError(i), err_max_val);
  }
  double inset_y_max = 1.4 * std::max({sig_max_val, dif_max_val, err_max_val});

  err_hist.GetYaxis()->SetNdivisions(5);
  err_hist.GetYaxis()->SetLabelSize(0.05);
  err_hist.GetXaxis()->SetTitle("#bf{m_{#tau#tau} [GeV]}    ");
  err_hist.GetXaxis()->SetTitleColor(kBlack);
  err_hist.GetXaxis()->SetTitleSize(0.06);
  err_hist.GetXaxis()->SetTitleOffset(0.95);
  err_hist.GetXaxis()->SetLabelSize(0.05);
  err_hist.SetNdivisions(405);
  err_hist.Draw("E2");
  err_hist.GetXaxis()->SetRangeUser(0,360);
  err_hist.GetYaxis()->SetRangeUser(-inset_y_max,inset_y_max);
  signal_hist.Draw("HISTSAME");
  diff_hist.Draw("SAME");

  pad.RedrawAxis();
  TLine line;
  line.DrawLine(0,0,350,0);
  line.DrawLine(1, -inset_y_max, 1, inset_y_max);
  TLegend *inlegend = new TLegend(0.5,0.72,0.9,0.89,"","brNDC");
  inlegend->SetTextFont(62);
  inlegend->AddEntry(&signal_hist, "", "F");
  inlegend->AddEntry(&diff_hist, "", "LP");
  inlegend->AddEntry(&err_hist, "", "F");
  LegendStyle(inlegend);
  inlegend->Draw();

  canv.cd();
  padBack.Draw();
  pad.Draw();
  TLatex *title_latex = new TLatex();
  title_latex->SetNDC();
  title_latex->SetTextSize(0.035);
  title_latex->SetTextFont(62);
  title_latex->SetTextAlign(31);
  // title_latex->DrawLatex(0.95,0.935,plot.title_right().c_str());
  title_latex->SetTextAlign(11);
  // title_latex->DrawLatex(0.17,0.931,plot.title_left().c_str());
  title_latex->SetTextSize(0.045);
  title_latex->DrawLatex(0.20,0.87,(text1 + (postfit? "" : " (prefit)")).c_str());
  title_latex->DrawLatex(0.20,0.82,text2.c_str());
  canv.SaveAs((output+".pdf").c_str());
  canv.SaveAs((output+".png").c_str());
*/

  return 0;
}

