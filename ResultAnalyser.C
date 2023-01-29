#include "top.hh"
#include <iostream>
#include "TStyle.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TString.h"
#include "TFile.h"
#include "TRandom.h"
#include "TGraphErrors.h"


using namespace std;


void ResultAnalyser()
{
    gStyle->SetOptStat(kFALSE);
	gRandom->SetSeed(0);
	
	// root file load
    TString nameNoOscModel = "GenshinArtifactSimulator_Raiden.root";
	TFile* readFile = new TFile(nameNoOscModel, "read");
	TH2D* simulationResult = (TH2D*)readFile->Get("Visual");

	cout << "simulationResult pointer : " << simulationResult << endl;

	int binNum = simulationResult->GetNbinsY();
	int artifactNum = simulationResult->GetNbinsX();
	double minDam = simulationResult->GetYaxis()->GetXmin();
	double maxDam = simulationResult->GetYaxis()->GetXmax();

	TF1* fitGauss = new TF1("fit", "gaus", minDam, maxDam, "Q0");

	TGraphErrors* gMeanError = new TGraphErrors();
	gMeanError->SetName("Mean and Error");
	TGraph* gChisq = new TGraph();
	gChisq->SetName("Chisq");

	for (int i = 0; i < artifactNum; i++)
	{
        // y projection
        TH1D* projectedHist = simulationResult->ProjectionY(Form("%d-th projected", i), i, i);
		projectedHist->Sumw2();

		// fit
		// fitGauss->SetName(Form("%d-th fit", i));
		projectedHist->Fit("fit");
		double y = projectedHist->GetMean();
		double ey = projectedHist->GetStdDev();
		double chisq = fitGauss->GetChisquare();
		cout << "chisq : " << chisq << endl;

        // save at TGraph
		gMeanError->SetPoint(i, i, y);
		gMeanError->SetPointError(i, 0.5, ey);
		gChisq->SetPoint(i, i, chisq);
	}
    
    // plot

	TCanvas* can1 = new TCanvas("canvas1", "canvas1", 1200, 800);
	gPad->SetLeftMargin(0.12);
	gPad->SetBottomMargin(0.12);
	gPad->SetRightMargin(0.08);
	gPad->SetTopMargin(0.05);
	
	TH1D* hFrame1 = new TH1D("hFrame1", "", 100, 0, artifactNum);
	hFrame1->SetTitle("artifact distribution; #artifacts; Damage; Z axis title");
	hFrame1->GetXaxis()->SetTitle("#artifacts");
	hFrame1->GetYaxis()->SetTitle("Damage");
	hFrame1->GetYaxis()->SetRangeUser(minDam, maxDam);
	hFrame1->Draw();
	gMeanError->Draw("SAMEl");

	TCanvas* can2 = new TCanvas("canvas2", "canvas2", 1200, 800);
	gPad->SetLeftMargin(0.12);
	gPad->SetBottomMargin(0.12);
	gPad->SetRightMargin(0.08);
	gPad->SetTopMargin(0.05);
	
	TH1D* hFrame2 = new TH1D("hFrame2", "", 100, 0, artifactNum);
	hFrame2->SetTitle("#chi^{2}; #artifacts; Chisq; Z axis title");
	hFrame2->GetXaxis()->SetTitle("#artifacts");
	hFrame2->GetYaxis()->SetTitle("Chisq");
	hFrame2->GetYaxis()->SetRangeUser(0, 1000);
	hFrame2->Draw();
	gChisq->Draw("SAMEl");

	cout << "minDam : " << minDam << endl;
	cout << "maxDam : " << maxDam << endl;
}