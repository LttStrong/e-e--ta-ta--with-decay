#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <TROOT.h>
#include <TLorentzVector.h>
#include <TLorentzRotation.h>
#include <TVector3.h>

void visualize_lhe_with_lorentz() {
    // 打开 ROOT 文件
    TFile *file = TFile::Open("events.root");
    TTree *tree = (TTree*)file->Get("event_tree");

    // 设置动量变量
    std::vector<float> *px = 0;
    std::vector<float> *py = 0;
    std::vector<float> *pz = 0;
    std::vector<float> *E = 0;
    tree->SetBranchAddress("px", &px);
    tree->SetBranchAddress("py", &py);
    tree->SetBranchAddress("pz", &pz);
    tree->SetBranchAddress("E", &E);

    // 创建直方图
    TH1F *h_px = new TH1F("h_px", "Particle Px", 100, -5, 5);
    TH1F *h_py = new TH1F("h_py", "Particle Py", 100, -5, 5);
    TH1F *h_pz = new TH1F("h_pz", "Particle Pz", 100, -5, 5);
    TH1F *h_E = new TH1F("h_E", "Particle E", 100, -5, 5);

    // 创建洛伦兹变换后的直方图
    TH1F *h_px_boosted = new TH1F("h_px_boosted", "Boosted Px", 100, -5, 5);
    TH1F *h_py_boosted = new TH1F("h_py_boosted", "Boosted Py", 100, -5, 5);
    TH1F *h_pz_boosted = new TH1F("h_pz_boosted", "Boosted Pz", 100, -5, 5);
    TH1F *h_E_boosted = new TH1F("h_E_boosted", "Boosted E", 100, -5, 5);

    // 遍历事件
    Long64_t nentries = tree->GetEntries();
    for (Long64_t i = 0; i < nentries; i++) {
        tree->GetEntry(i);

        // 计算总四动量
        TLorentzVector totalMomentum(0, 0, 0, 0);
        for (size_t j = 0; j < px->size(); j++) {
            TLorentzVector particle((*px)[j], (*py)[j], (*pz)[j], (*E)[j]);
            totalMomentum += particle;
        }

        // 计算质心系速度并生成洛伦兹变换
        TVector3 beta = totalMomentum.BoostVector();
        TLorentzRotation lorentzTransform;
        lorentzTransform.Boost(-beta); // 转换到质心系

        // 对每个粒子进行变换并填充直方图
        for (size_t j = 0; j < px->size(); j++) {
            TLorentzVector particle((*px)[j], (*py)[j], (*pz)[j], (*E)[j]);

            // 原始数据填充
            h_px->Fill(particle.Px());
            h_py->Fill(particle.Py());
            h_pz->Fill(particle.Pz());
            h_E->Fill(particle.E());

            h_px->GetXaxis()->SetTitle("X-axis_Momentum(GeV/c)");
            h_px->GetYaxis()->SetTitle("Particle Numbun");

            h_py->GetXaxis()->SetTitle("Y-axis_Momentum(GeV/c)");
            h_py->GetYaxis()->SetTitle("Particle Numbun");

            h_pz->GetXaxis()->SetTitle("Z-axis_Momentum(GeV/c)");
            h_pz->GetYaxis()->SetTitle("Particle Numbun");

            h_E->GetXaxis()->SetTitle("Energy(GeV)(GeV/c)");
            h_E->GetYaxis()->SetTitle("Particle Numbun");

            // 进行洛伦兹变换
            TLorentzVector boostedParticle = lorentzTransform * particle;

            // 变换后的数据填充
            h_px_boosted->Fill(boostedParticle.Px());
            h_py_boosted->Fill(boostedParticle.Py());
            h_pz_boosted->Fill(boostedParticle.Pz());
            h_E_boosted->Fill(boostedParticle.E());

            h_px_boosted->GetXaxis()->SetTitle("X-axis_Momentum(GeV/c)");
            h_px_boosted->GetYaxis()->SetTitle("Particle Numbun");

            h_py_boosted->GetXaxis()->SetTitle("Y-axis_Momentum(GeV/c)");
            h_py_boosted->GetYaxis()->SetTitle("Particle Numbun");

            h_pz_boosted->GetXaxis()->SetTitle("Z-axis_Momentum(GeV/c)");
            h_pz_boosted->GetYaxis()->SetTitle("Particle Numbun");

            h_E_boosted->GetXaxis()->SetTitle("Energy(GeV)");
            h_E_boosted->GetYaxis()->SetTitle("Particle Numbun");
        }
    }

    // 绘制原始和变换后的动量分布
    TCanvas *c1 = new TCanvas("c1", "Momentum Distributions", 1920, 1080);
    c1->Divide(2, 2);
    c1->cd(1); h_px->Draw();
    c1->cd(2); h_py->Draw();
    c1->cd(3); h_pz->Draw();
    c1->cd(4); h_E->Draw();
    c1->SaveAs("momentum_distributions_original.png");

    TCanvas *c2 = new TCanvas("c2", "Boosted Momentum Distributions", 1920, 1080);
    c2->Divide(2, 2);
    c2->cd(1); h_px_boosted->Draw();
    c2->cd(2); h_py_boosted->Draw();
    c2->cd(3); h_pz_boosted->Draw();
    c2->cd(4); h_E_boosted->Draw();
    c2->SaveAs("momentum_distributions_boosted.png");

    file->Close();
}
