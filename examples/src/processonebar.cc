#include"examples/include/processonebar.hh"
#include"include/SMIWaveformAnalyzer.hh"
//plugins
#include"plugins/cleanWaveForm.hh"
#include"plugins/changeADCRange.hh"
#include"plugins/leadingEdgeLinear.hh"
#include"plugins/leadingEdgeBasic.hh"
#include"plugins/calculateCharge.hh"
#include"plugins/constantFractionLinear.hh"
#include"plugins/subtractBaseLine.hh"

//stl
#include<iostream>

#include<TCanvas.h>
processOneBar::processOneBar() : event(0), h(NULL), h2(NULL) {
  //list.add(new plugin::cleanWaveForm          );
  list.add(new plugin::changeADCRange(1/4095.));
  //list.add(new plugin::subtractBaseLine());
  list.add(new plugin::leadingEdgeLinear(40,200,0.05)   );
  list.add(new plugin::calculateCharge(40,200)      );
  list.add(new plugin::constantFractionLinear());

  h = new TH1D*[16];
  h[ 0] = new TH1D("","",200,0,80);
  h[ 1] = new TH1D("","",200,0,80);
  h[ 2] = new TH1D("","",200,0,80);
  h[ 3] = new TH1D("","",200,0,80);
  h[ 4] = new TH1D("","",400,0,100);
  h[ 5] = new TH1D("","",400,0,100);
  h[ 6] = new TH1D("","",200,-40,40);
  h[ 7] = new TH1D("","",400,0,100);
  h[ 8] = new TH1D("","",200,-1,1);
  h[ 9] = new TH1D("","",200,0,1);
  h[10] = new TH1D("","",200,0,1);
  h[11] = new TH1D("","",200,0,1);
  h[12] = new TH1D("","",400,0,100);
  h[13] = new TH1D("","",400,0,100);
  h[14] = new TH1D("","",400,0,100);
  h[15] = new TH1D("","",400,0,100);

  h3 = new TH1D*[16];
  h3[ 0] = new TH1D("","",200,-1,1);
  h3[ 1] = new TH1D("","",200,-1,1);
  h3[ 2] = new TH1D("","",200,-80,80);
  h3[ 3] = new TH1D("","",200,-80,80);
  h3[ 4] = new TH1D("","",400,0,100);
  h3[ 5] = new TH1D("","",400,0,100);
  h3[ 6] = new TH1D("","",200,-40,40);
  h3[ 7] = new TH1D("","",400,0,100);
  h3[ 8] = new TH1D("","",200,0,1);
  h3[ 9] = new TH1D("","",200,0,1);
  h3[10] = new TH1D("","",200,0,1);
  h3[11] = new TH1D("","",200,0,1);
  h3[12] = new TH1D("","",400,0,100);
  h3[13] = new TH1D("","",400,0,100);
  h3[14] = new TH1D("","",400,0,100);
  h3[15] = new TH1D("","",400,0,100);

  h2 = new TH2D*[16];
  h2[ 0] = new TH2D("","",400,0,100,200,0,80);
  h2[ 1] = new TH2D("","",400,0,100,200,0,80);
  h2[ 2] = new TH2D("","",400,0,100,200,0,1);
  h2[ 3] = new TH2D("","",400,0,100,200,0,1);

  h2[ 4] = new TH2D("","",400,0,100,200,0,80);
  h2[ 5] = new TH2D("","",400,0,100,200,0,80);
  h2[ 6] = new TH2D("","",400,0,100,200,0,80);
  h2[ 7] = new TH2D("","",400,0,100,200,0,80);

  h2[ 8] = new TH2D("","",400,0,100,200,0,1);
  h2[ 9] = new TH2D("","",400,0,100,200,0,1);
  h2[10] = new TH2D("","",400,0,100,200,0,1);
  h2[11] = new TH2D("","",400,0,100,200,0,1);
  
  h2[12] = new TH2D("","",400,0,100,400,0,100);
  h2[13] = new TH2D("","",400,0,100,400,0,100);
  h2[14] = new TH2D("","",400,0,100,400,0,100);
  h2[15] = new TH2D("","",400,0,100,400,0,100);

  h4 = new TH2D*[4];
  h4[0] = new TH2D("","",200,0,200,200,0,1);
  h4[1] = new TH2D("","",200,0,200,200,0,1);
  h4[2] = new TH2D("","",200,0,200,200,0,1);
  h4[3] = new TH2D("","",200,0,200,200,0,1);
}

void processOneBar::operator()(SMIWaveformAnalyzer &wave){
  //subtractBaseLine(wave);
  wave.processAll(list);
  printEvent(wave);
  
  double qscalefactor1d = 15.56/12.26;
  double qscalefactor1u = 15.56/15.09;
  double qscalefactor2d = 15.56/17.14;
  // wave.channel["IFES1d"].result["Q"] *= qscalefactor1d;
  // wave.channel["IFES1u"].result["Q"] *= qscalefactor1u;
  // wave.channel["IFES2d"].result["Q"] *= qscalefactor2d;

  qscalefactor1d = 0.2224/0.1858;
  qscalefactor1u = 0.2224/0.2287;
  qscalefactor2d = 0.2224/0.257;

  // wave.channel["IFES1d"].result["A"] *= qscalefactor1d;
  // wave.channel["IFES1u"].result["A"] *= qscalefactor1u;
  // wave.channel["IFES2d"].result["A"] *= qscalefactor2d;

  double q1 = wave.channel["TopR"].result["Q"]*1.2;
  double q2 = wave.channel["TopL"].result["Q"];

  h[ 0]->Fill(q1);
  h[ 1]->Fill(q2);

  h[ 8]->Fill((q1-q2)/(q1+q2));
  h[ 9]->Fill((q1)/(q1+q2));

  //printEvent(wave);
  event++;
}

void processOneBar::Print(){
  TCanvas *c1 = new TCanvas("c1","multipads",1900,1200);
  c1->cd();
  h[ 0]->Draw();
  h[ 0]->SetXTitle("Charge");
  h[ 0]->SetYTitle("Counts");
  h[ 1]->SetLineColor(kRed);
  h[ 1]->Draw("SAME");
  c1->Print("ChargePlot.pdf");
  h[ 8]->Draw();
  h[ 8]->SetXTitle("(q1-q2)/(q1+q2)");
  h[ 8]->SetYTitle("Counts");
  c1->Print("PositonPlot.pdf");
  h[ 9]->Draw();
  h[ 9]->SetXTitle("q1/(q1+q2)");
  h[ 9]->SetYTitle("Counts");
  c1->Print("PositonPlot2.pdf");
}

void processOneBar::subtractBaseLine(SMIWaveformAnalyzer &wave){
  std::map<std::string, double>::iterator it = baseline.begin();
  while(it!=baseline.end()){
    wave.getWaveForm(it->first) -= it->second;
    it++;
  }
}

void processOneBar::printEvent(SMIWaveformAnalyzer &wave){
  static int count = 0;
  char tmp[128];
  sprintf(tmp,"wv-%i.dat",count++);
  ofile.open(tmp);
  for(int i=0; i<1024; i++){
  ofile<< wave.channel.begin()->second.t[i] << " ";
    for(auto it : wave.channel){
      ofile << it.second.V[i] << " ";
    }
    ofile << std::endl;
  }
  ofile.close();
  ofile.clear();
}
