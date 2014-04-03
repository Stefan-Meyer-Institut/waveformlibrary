#include"examples/include/processDemo.hh"
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
processDemo::processDemo() : event(0), h(NULL), h2(NULL) {
  //list.add(new plugin::cleanWaveForm          );
  list.add(new plugin::changeADCRange(1/4095.));
  list.add(new plugin::subtractBaseLine());
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
  h[ 8] = new TH1D("","",200,0,1);
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

}

void processDemo::operator()(SMIWaveformAnalyzer &wave){
  //subtractBaseLine(wave);
  wave.processAll(list);

  double qscalefactor1d = 15.56/12.26;
  double qscalefactor1u = 15.56/15.09;
  double qscalefactor2d = 15.56/17.14;
  wave.channel["IFES1d"].result["Q"] *= qscalefactor1d;
  wave.channel["IFES1u"].result["Q"] *= qscalefactor1u;
  wave.channel["IFES2d"].result["Q"] *= qscalefactor2d;

  qscalefactor1d = 0.2224/0.1858;
  qscalefactor1u = 0.2224/0.2287;
  qscalefactor2d = 0.2224/0.257;

  wave.channel["IFES1d"].result["A"] *= qscalefactor1d;
  wave.channel["IFES1u"].result["A"] *= qscalefactor1u;
  wave.channel["IFES2d"].result["A"] *= qscalefactor2d;

  h[ 0]->Fill(wave.channel["IFES1d"].result["Q"]);
  h[ 1]->Fill(wave.channel["IFES1u"].result["Q"]);
  h[ 2]->Fill(wave.channel["IFES2d"].result["Q"]);
  h[ 3]->Fill(wave.channel["IFES2u"].result["Q"]);

  double t1 = std::abs(wave.channel["IFES1d"].result["CF--3.00"] 
		      - wave.trigger[wave.channeltrigger["IFES1d"]].result["CF--3.00"]);
  double t2 = std::abs(wave.channel["IFES1u"].result["CF--3.00"] 
		      - wave.trigger[wave.channeltrigger["IFES1u"]].result["CF--3.00"]);

  double t3 = std::abs(wave.channel["IFES2d"].result["CF--3.00"] 
		      - wave.trigger[wave.channeltrigger["IFES2d"]].result["CF--3.00"]);

  double t4 = std::abs(wave.channel["IFES2u"].result["CF--3.00"] 
		      - wave.trigger[wave.channeltrigger["IFES2u"]].result["CF--3.00"]);

  h[ 4]->Fill((t1+t2)/2);
  h[ 5]->Fill((t3+t4)/2);
  h[ 6]->Fill((t1+t2)/2 - (t3+t4)/2);
  h[ 7]->Fill(((t1+t2)/2 + (t3+t4)/2)/2);

  h[ 8]->Fill(wave.channel["IFES1d"].result["A"]);
  h[ 9]->Fill(wave.channel["IFES1u"].result["A"]);
  h[10]->Fill(wave.channel["IFES2d"].result["A"]);
  h[11]->Fill(wave.channel["IFES2u"].result["A"]);

  double q1d =wave.channel["IFES1d"].result["Q"];
  double q1u =wave.channel["IFES1u"].result["Q"];
  double q2d =wave.channel["IFES2d"].result["Q"];
  double q2u =wave.channel["IFES2u"].result["Q"];

  double a1d =wave.channel["IFES1d"].result["A"];
  double a1u =wave.channel["IFES1u"].result["A"];
  double a2d =wave.channel["IFES2d"].result["A"];
  double a2u =wave.channel["IFES2u"].result["A"];

  h[12]->Fill((q1d+q1u)/2);
  h[13]->Fill((q2d+q2u)/2);

  h[14]->Fill((t1+t2)/2);
  h[15]->Fill((t3+t4)/2);
    
  h3[0]->Fill( ((q1d+q1u)/2 - (q2d+q2u)/2) / ((q1d+q1u)/2 + (q2d+q2u)/2) );
  //std::cerr << (q1d+q1u)/2 << " " << (q2d+q2u)/2 << " " << ((q1d+q1u)/2 - (q2d+q2u)/2) / ((q1d+q1u)/2 + (q2d+q2u)/2) << std::endl;

  h3[1]->Fill( ((a1d+a1u)/2 - (a2d+a2u)/2) / ((a1d+a1u)/2 + (a2d+a2u)/2) );

  h2[ 0]->Fill((t1+t2)/2,(wave.channel["IFES1d"].result["Q"] + wave.channel["IFES1u"].result["Q"])/2);
  h2[ 1]->Fill((t3+t4)/2,(wave.channel["IFES2d"].result["Q"] + wave.channel["IFES2u"].result["Q"])/2);

  h2[ 2]->Fill((t1+t2)/2,(wave.channel["IFES1d"].result["A"] + wave.channel["IFES1u"].result["A"])/2);
  h2[ 3]->Fill((t3+t4)/2,(wave.channel["IFES2d"].result["A"] + wave.channel["IFES2u"].result["A"])/2);

  h2[ 4]->Fill(t1,(wave.channel["IFES1d"].result["Q"] ));
  h2[ 5]->Fill(t2,(wave.channel["IFES1u"].result["Q"] ));
  h2[ 6]->Fill(t3,(wave.channel["IFES2d"].result["Q"] ));
  h2[ 7]->Fill(t4,(wave.channel["IFES2u"].result["Q"] ));

  h2[ 8]->Fill(t1,(wave.channel["IFES1d"].result["A"] ));
  h2[ 9]->Fill(t2,(wave.channel["IFES1u"].result["A"] ));
  h2[10]->Fill(t3,(wave.channel["IFES2d"].result["A"] ));
  h2[11]->Fill(t4,(wave.channel["IFES2u"].result["A"] ));

  h2[12]->Fill(t1,t2);
  h2[13]->Fill(t3,t4);
  h2[14]->Fill((t1+t2)/2,(t3+t4)/2);

  h2[15]->Fill((wave.channel["IFES1d"].result["Q"] + wave.channel["IFES1u"].result["Q"])/2,(wave.channel["IFES1d"].result["A"] + wave.channel["IFES1u"].result["A"])/2);
  // for(int i=0; i<1024; i++){
  //     for(auto a : wave.channel){
  // 	std::cout << a.second.V[i] << " ";
  //     }
  //     for(auto a : wave.trigger){
  // 	std::cout << a.second.V[i] << " ";
  //     }
  //     std::cout << std::endl;
  //   }

  //printEvent(wave);

   // std::cout << "Event: " << event << std::endl;
   // std::cout << "-----------------------------------------------" << std::endl;
   // std::cout << wave;
   // std::cout << "-----------------------------------------------" << std::endl;

  event++;
}

void processDemo::Print(){
  h[ 6]->Fit("gaus","VM","VM", -3, 3);

  TCanvas *c1 = new TCanvas("c1","multipads",1900,1200);
  c1->Divide(4,4,0,0);
  for(int l=0; l<16; l++){
    c1->cd(l+1);
    h[l]->Draw();
  }
  c1->Print("test.pdf");

  for(int l=0; l<16; l++){
    c1->cd(l+1);
    h2[l]->Draw("COLZ");
  }
  c1->Print("test2.pdf");

  c1->cd(0);
  h3[0]->Draw();
  c1->Print("h3Q.pdf");
  Double_t q[5] = {0.025,0.25,0.5,0.75,0.975};
  Double_t q2[5] = {0,0,0,0,0};
  h3[0]->GetQuantiles(5,q2,q);
  std::cout << "Charge: " << q2[0] << " " << q2[1] << " " << q2[2] << " " << q2[3] << " " << q2[4] << " " << std::endl;

  h3[1]->Draw();
  c1->Print("h3A.pdf");
  h3[1]->GetQuantiles(5,q2,q);
  std::cout << "Amplitude: " << q2[0] << " " << q2[1] << " " << q2[2] << " " << q2[3] << " " << q2[4] << " " << std::endl;
}

void processDemo::subtractBaseLine(SMIWaveformAnalyzer &wave){
  std::map<std::string, double>::iterator it = baseline.begin();
  while(it!=baseline.end()){
    wave.getWaveForm(it->first) -= it->second;
    it++;
  }
}

void processDemo::printEvent(SMIWaveformAnalyzer &wave){
  for(int i=0; i<1024; i++){
    for(auto it : wave.channel){
      std::cout << it.second.V[i] << " ";
    }
    std::cout << std::endl;
  }
}
