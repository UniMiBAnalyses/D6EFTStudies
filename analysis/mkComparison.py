#!/usr/bin/python

import ROOT
import os
import sys
import argparse

ROOT.v5.TFormula.SetMaxima(100000,100000,1000000)

class canvMaker():
    
    count = 0
    def __init__(self):
        print("Initialized canvas handler")

    def getCanv(self):
        c = ROOT.TCanvas("c_{}".format(self.count), "c_{}".format(self.count), 700, 800)
        ROOT.gPad.SetFrameLineWidth(3)

        pad1 = ROOT.TPad("pad", "pad", 0, 0.3, 1, 1)
        pad1.SetFrameLineWidth(2)
        pad1.SetBottomMargin(0.005)
        pad1.Draw()

        pad2 = ROOT.TPad("pad2", "pad2", 0, 0.0, 1, 0.3)
        pad2.SetFrameLineWidth(2)
        pad2.SetTopMargin(0.05)

        pad2.SetFrameBorderMode(0)
        pad2.SetBorderMode(0)
        pad2.SetBottomMargin(0.4)
        pad2.Draw()
        
        self.count += 1

        return c, pad1, pad2
    

def drawLumi(lumi):
   #Fancy stuffs
   tex3 = ROOT.TLatex(0.90,.915,"{:.1f}".format(lumi) + " fb^{-1} (13 TeV)")
   tex3.SetNDC()
   tex3.SetTextAlign(31)
   tex3.SetTextFont(42)
   tex3.SetTextSize(0.05)
   tex3.SetLineWidth(3)
   return tex3


def drawCMS():
   tex4 = ROOT.TLatex(0.2,.915, "CMS")
   tex4.SetNDC()
   tex4.SetTextAlign(31)
   tex4.SetTextFont(62)
   tex4.SetTextSize(0.06)
   tex4.SetLineWidth(3)
   return tex4


def drawSimulation():
   tex5 = ROOT.TLatex(0.38,.917, "Simulation")
   tex5.SetNDC()
   tex5.SetTextAlign(31)
   tex5.SetTextFont(52)
   tex5.SetTextSize(0.048)
   tex5.SetLineWidth(3)
   return tex5


def drawPreliminary():
   tex6 = ROOT.TLatex(0.57,.917, "Preliminary")
   tex6.SetNDC()
   tex6.SetTextAlign(31)
   tex6.SetTextFont(52)
   tex6.SetTextSize(0.048)
   tex6.SetLineWidth(3) 
   return tex6

def getHisto(path, tree, var, bins, range_, weight, luminosity):
    f = ROOT.TFile(path)
    global_numbers             = f.Get ( tree + "_nums")
    cross_section              = global_numbers.GetBinContent (1) 
    sum_weights_total          = global_numbers.GetBinContent (2) 
    #sum_weights_selected       = global_numbers.GetBinContent (3) 
    #NB luminosity in fb, cross-section expected in pb in the config files
    normalization = cross_section * 1000. * luminosity / (sum_weights_total)
    
    t = f.Get(tree)
 
    if var is not "events": 
       h = ROOT.TH1F("h_{}_{}".format(tree, var), "h", bins, range_[0], range_[1])
       n = t.Draw("{} >> h_{}_{}".format(var, tree, var), "{}".format(weight))
       h.SetBinContent(h.GetNbinsX(), h.GetBinContent(h.GetNbinsX()) + h.GetBinContent(h.GetNbinsX()+1))
       h.SetBinContent(1, h.GetBinContent(0) + h.GetBinContent(1))
       h.Scale(normalization)
    else:
       h = ROOT.TH1F("h_{}_{}".format(tree, var), "h", 1, 0, 2)
       n = t.Draw("1 >> h_{}_{}".format(tree, var), "{}".format(weight))
       h.SetBinContent(h.GetNbinsX(), h.GetBinContent(h.GetNbinsX()) + h.GetBinContent(h.GetNbinsX()+1))
       h.Scale(normalization)
       h.SetMinimum(1300)

    
  
    h.SetDirectory(0)
    
    h.SetTitle("")

    h.GetYaxis().SetTitle("Events")
    h.GetYaxis().SetTitleSize(0.05)
    h.GetYaxis().SetLabelSize(0.04)
    h.GetYaxis().SetTitleOffset(1)
    h.GetYaxis().SetMaxDigits(3)
    
    return h


def getRatio(numerator, denominator):
    
    h_alt = numerator.Clone("clone")
    h_alt.Divide(denominator)
    
    return h_alt


def getDefaultRatio(default, ratio_range):
   
    h_bench = default.Clone("clone")
    h_bench.Divide(default)
    h_bench.SetFillStyle(3004)
    h_bench.SetFillColor(ROOT.kBlack)
    h_bench.SetLineWidth(2)
    h_bench.SetLineColor(ROOT.kBlack)

    h_bench.GetXaxis().SetTitleSize(0.13)
    h_bench.GetYaxis().SetLabelSize(0.08)
    h_bench.GetXaxis().SetLabelSize(0.1)

    h_bench.GetYaxis().SetTitleSize(0.1)
    h_bench.GetYaxis().SetTitleOffset(0.5)
    h_bench.GetYaxis().SetNdivisions(4)

    h_bench.GetYaxis().SetRangeUser(ratio_range[0],ratio_range[1])

    h_bench.SetLineColor(ROOT.kBlack)
   
    return h_bench
    

def getLegend(pos="upperright"):
    if pos == "upperright":
        leg = ROOT.TLegend(0.2, 0.6, 0.89, 0.86)
        leg.SetBorderSize(0)
        leg.SetNColumns(1)
        leg.SetTextSize(0.035)
    elif pos == "lowerright":
        leg = ROOT.TLegend(0.4, 0.14, 0.89, 0.4)
        leg.SetBorderSize(0)
        leg.SetNColumns(1)
        leg.SetTextSize(0.035)
        
    elif pos == "lowerleft":
        leg = ROOT.TLegend(0.4, 0.6, 0.89, 0.86)
        leg.SetBorderSize(0)
        leg.SetNColumns(1)
        leg.SetTextSize(0.035)
    elif pos == "upperleft":
        leg = ROOT.TLegend(0.4, 0.6, 0.89, 0.86)
        leg.SetBorderSize(0)
        leg.SetNColumns(1)
        leg.SetTextSize(0.035)
    
    return leg


if __name__ == "__main__":

    parser = argparse.ArgumentParser(description='Command line parser for model testing')
    parser.add_argument('--cfg',     dest='cfg',     help='the config file', required = True)
    parser.add_argument('--int',     dest='integral',     help='Show integral in legend name for each entry', required = False, default = False, action = "store_true")
    
    args = parser.parse_args()
   
    ROOT.gROOT.SetBatch(1)
    ROOT.TH1.SetDefaultSumw2(True)
    ROOT.gStyle.SetOptStat(0)

    samples = {}
    variables = {}
    output = ""
    luminosity = 0
    ratio_range = [0.5,1.5]
    showIntegral = args.integral
    scaleMax = 1.5 # factor to scale the maximum of all the histos in a pad
    execfile(args.cfg)
    
    if not os.path.isdir(output):
       os.mkdir(output)

    os.system("cp /afs/cern.ch/user/g/gboldrin/public/index.php " + output)

    cm = canvMaker()

    for plot_comp in samples.keys():
       
       for b_name in variables.keys():

          histo_dict = {}
          histo_stack_dict = {}

          max_d = {}

          for sample in samples[plot_comp].keys():
             print(samples[plot_comp][sample]["path"])
             histo_dict[sample] = getHisto(samples[plot_comp][sample]["path"], 
                                           samples[plot_comp][sample]["path"].split("/")[-1].split("ntuple_")[1].split(".root")[0], 
                                           b_name, 
 					   variables[b_name]["bins"], 
					   [variables[b_name]["min"], variables[b_name]["max"]], 
					   samples[plot_comp][sample]["weight"], 
					   luminosity)

             if sample is not "Main":
                histo_dict[sample].SetLineColor(ROOT.kRed if "color" not in samples[plot_comp][sample].keys() else samples[plot_comp][sample]["color"])
                histo_dict[sample].SetMarkerStyle(20 if "markerstyle" not in samples[plot_comp][sample].keys() else samples[plot_comp][sample]["markerstyle"])
                histo_dict[sample].SetMarkerSize(0.5 if "markersize" not in samples[plot_comp][sample].keys() else samples[plot_comp][sample]["markersize"])
                histo_dict[sample].SetMarkerColor(ROOT.kRed if "color" not in samples[plot_comp][sample].keys() else samples[plot_comp][sample]["color"])
       
          
                    
             else:
                histo_dict[sample].SetLineColor(ROOT.kBlack)
                histo_dict[sample].SetFillColorAlpha(ROOT.kGray, 0.3)
                histo_dict[sample].GetYaxis().SetTitle("Events" if "ylabel" not in variables[b_name].keys() else variables[b_name].keys()["ylabel"])
         
             if "stack" in samples[plot_comp][sample].keys():
                stack = samples[plot_comp][sample]["stack"]
                if stack not in histo_stack_dict.keys(): histo_stack_dict[stack] = []
                histo_stack_dict[stack].append(histo_dict[sample]) 
      
                # store the maximum so we can scale histos
                if stack not in max_d.keys():
                   max_d[stack] = histo_dict[sample].GetMaximum()
                else: max_d[stack] += histo_dict[sample].GetMaximum()

             else:
                max_d[sample] = histo_dict[sample].GetMaximum()




          # MAIN PLOT
         

          ovmax = max(value for key,value in max_d.items())
           
          c, pad1, pad2 = cm.getCanv()
          
          leg = getLegend("upperright" if "legpos" not in variables[b_name].keys() else variables[b_name]["legpos"]) 
          name = samples[plot_comp]["Main"]["nameHR"] + " [{:.1f}]".format(histo_dict["Main"].Integral()) if showIntegral is True else samples[plot_comp]["Main"]["nameHR"]
          leg.AddEntry(histo_dict["Main"], name , "F")
          
          for sample in samples[plot_comp].keys():
             if sample is not "Main" and "stack" not in samples[plot_comp][sample].keys(): 
                name = samples[plot_comp][sample]["nameHR"] + " [{:.1f}]".format(histo_dict[sample].Integral()) if showIntegral else samples[plot_comp][sample]["nameHR"]
                leg.AddEntry(histo_dict[sample], name, "PE")


          pad1.cd()

          histo_dict["Main"].SetMaximum(scaleMax * ovmax)
          histo_dict["Main"].Draw("hist E")
          for sample in samples[plot_comp].keys():
             if "stack" in samples[plot_comp][sample].keys(): continue
             if sample is not "Main": 
                histo_dict[sample].Draw("PE same")

          # Now build stacks
          for stack_name in histo_stack_dict.keys():
             h_ = histo_stack_dict[stack_name][0].Clone("h_" + stack_name)
             for hist in histo_stack_dict[stack_name][1:]: h_.Add(hist)

             histo_stack_dict[stack_name] = h_

             name = stack_name + " [{:.1f}]".format(h_.Integral()) if showIntegral else stack_name
             leg.AddEntry(h_, name, "PE")
             h_.Draw("PE same")


          l = drawLumi(luminosity)
          cms = drawCMS()
          simu = drawSimulation()
          prel = drawPreliminary()
           
          cms.Draw() 
          simu.Draw()
          prel.Draw()
          l.Draw()
 
          if "setlogy" in variables[b_name].keys(): pad1.SetLogy()

          leg.Draw()
        
          # RATIO PLOT
   
          pad2.cd()
          
          histo_ratio_dict = {}
          histo_ratio_dict["Main"] = getDefaultRatio(histo_dict["Main"], ratio_range)
          
          for sample in samples[plot_comp].keys():
             if sample is not "Main" and "stack" not in samples[plot_comp][sample].keys(): histo_ratio_dict[sample] = getRatio(histo_dict[sample], histo_dict["Main"])

          for stack_name in histo_stack_dict.keys():
             histo_ratio_dict[stack_name] = getRatio(histo_stack_dict[stack_name], histo_dict["Main"])

          histo_ratio_dict["Main"].SetMarkerSize(0)
          histo_ratio_dict["Main"].GetYaxis().SetTitle("Ratio" if "ratioylabel" not in variables[b_name].keys() else variables[b_name].keys()["ratioylabel"])
          histo_ratio_dict["Main"].GetXaxis().SetTitle(b_name if "xlabel" not in variables[b_name].keys() else variables[b_name].keys()["xlabel"])

          histo_ratio_dict["Main"].Draw("E2")
          for sample in samples[plot_comp].keys():
             if sample is not "Main" and "stack" not in samples[plot_comp][sample].keys(): histo_ratio_dict[sample].Draw("PE same")

          for stack_name in histo_stack_dict.keys():
             histo_ratio_dict[stack_name].Draw("PE same")

          c.Draw()

          c.Print(output + "/{}_{}.pdf".format(plot_comp, b_name))
          c.Print(output + "/{}_{}.png".format(plot_comp, b_name)) 
