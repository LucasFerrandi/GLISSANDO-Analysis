// ref: https://root-forum.cern.ch/t/tree-and-histogram-read-data-from-txt-file/19800/2
#include "TString.h"
#include "TSystem.h"
#include "TInterpreter.h"
#include "TFile.h"
#include "TNtuple.h"
#include "TH1.h"
//#include <iostream>
#include <fstream>
#include <cstdio>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <cmath>
#include <vector>
#include <string.h>



void analiseHotSpots()
{

gROOT->SetBatch(kTRUE); //Nao gera os canvas quando TRUE

Int_t b= 6;
Int_t SEED= 6200;
double r2_hs= 0.2;
Int_t r2_hs10=(10*r2_hs);

const char *file_in = Form("/home/lucas/Downloads/root/Macro-Fev2022/ResultadosFinaisIC/PointsERoots/b=%d/glissando%d.root.points",b,SEED);
 const char *file_out = Form("/home/lucas/Downloads/root/Macro-Fev2022/ResultadosFinaisIC/PointsERoots/b=%d/ResultHS-b=%d-SEED=%d.root",b,b,SEED);
 //const char *file_out2 = Form("AnáliseHotSpotsPbPb,b=%d.txt",b);

ifstream in;
in.open(file_in);

Float_t x, y;
Int_t nlines = 0;  //Nsources
Int_t nbinarias = 0; //N col binarias
Int_t Nw = 0; //N wounded nuc

TFile *f = new TFile(file_out,"RECREATE");
//TFile *f2 = new TFile(file_out2,"RECREATE");
//TNtuple *TuplaResults = new TNtuple("Resultados","data from ?","eps_fix_fireball:eps_part_fireball:Media_eps_fix_hs:Media_eps_part_hs");
TNtuple *ntuple_A = new TNtuple("Nw A","data from .points","x:y");
TNtuple *ntuple_B = new TNtuple("Nw B","data from .points","x:y");
TNtuple *ntuple_nbin = new TNtuple("Nbin","data from .points","x:y");
TNtuple *ntuple_core_A = new TNtuple("Core A","data from .points","x:y");
TNtuple *ntuple_mantle_A = new TNtuple("Mantle A","data from .points","x:y");

Int_t l, k_int, k_int2, pos1, pos2, c, m=0;
Float_t n=0;
string str;
Int_t contagem=0;


string line;
while(getline(in,line)) {
contagem ++;
//printf("contagem=%i \n",contagem);
if(m!=0){
nlines++;
pos1 = line.find(" ");
pos2 = line.find(" ",pos1+1);

x = stof(line.substr(0,pos1));
y = stof(line.substr(pos1+1,pos2-pos1));
c = stoi(line.substr(pos2+1));
if(c!=0){
Nw++;
}
if(c<0){    // Nw nucleo A
ntuple_A->Fill(x,y);
}
if(c>0){    // Nw nucleo B
ntuple_B->Fill(x,y);
}
 if(c==0){   // colisoes binarias
ntuple_nbin->Fill(x,y);
nbinarias++;
} 
if(c<-1){    // core A
ntuple_core_A->Fill(x,y);
}
if(c==-1){   // mantle A
ntuple_mantle_A->Fill(x,y);
}
// ARRUMAR CORE E MANTLE


m = m - 1;
}
else{
if(floor(n)==n){  // primeira parte de um bloco
k_int = line.find(" ");
k_int2 = line.find(" ",k_int+1);
//printf("k_int=%i, k_int2=%i \n",k_int,k_int2);
str = line.substr(k_int+1,k_int2-k_int-1);
//printf("str=%s \n",str.c_str());
m = stoi(str);
//printf("m=%i \n",m);  //NwAB
}
else{      // segunda parte de um bloco
m = stoi(line.substr(0));
//printf("m=%i\n", m); //Nbin
}
n += 0.5;
}
}

in.close();

//x
Double_t xmin = ntuple_nbin->GetMinimum("x");
Double_t xmax = ntuple_nbin->GetMaximum("x");
Double_t difx = xmax-xmin;
 xmax += difx*0.15;
 xmin -= difx*0.15;
 
//y
Double_t ymin = ntuple_nbin->GetMinimum("y");
 Double_t ymax = ntuple_nbin->GetMaximum("y");
 Double_t dify = ymax-ymin;
ymax += dify*0.15;
ymin -= dify*0.15;

Double_t binxlen=0.2;     //Largura de cada bin (usar 0.2fm)
Double_t binylen=0.2;     //Altura de cada bin
//Double_t EscalaThresh = 0.2/binxlen;
Int_t xbins = (xmax-xmin)/binxlen;   //Num de bins
Int_t ybins = (ymax-ymin)/binylen;

Int_t numbins=xbins*ybins;

Double_t binxlen2=0.033333;     //Largura de cada bin (usar0.033333fm)
Double_t binylen2=0.033333;     //Altura de cada bin
Int_t xbins2 = (xmax-xmin)/binxlen2;   //Num de bins
Int_t ybins2 = (ymax-ymin)/binylen2;
Int_t numbins2=xbins2*ybins2;

 //----------------------------------------------------------------------------

//PRODUZ HISTOGRAMAS:
//auto c1 = new TCanvas("nwA","nwA",600,400);
TH2F *h1 = new TH2F("h1","Nucleus A Nw distribution",xbins,xmin,xmax,ybins,ymin,ymax);
ntuple_A->Project("h1","y:x");
h1->GetXaxis()->SetTitle("X (fm)");
h1->GetYaxis()->SetTitle("Y (fm)");
h1->Draw("colz");


TH2F *h12 = new TH2F("h12","Nucleus A Nw distribution",xbins2,xmin,xmax,ybins2,ymin,ymax);
ntuple_A->Project("h12","y:x");
h12->Draw("colz");

//h1->TH2::ShowPeaks(2,"noMarkov",0.9);      
//Encontra hotspots. arg=(sigma,option,threshold); picos com largura > sigma, altura > threshold*maximum bin content of this
//h1->TH2::Smooth(1,"k5b");


// nucleo B ----------------------------------------------------------------------------

 
 //auto c2 = new TCanvas("nwB","nwB",600,400);
 TH2F *h2 = new TH2F("h2","nwB",xbins,xmin,xmax,ybins,ymin,ymax);
 ntuple_B->Project("h2","y:x");
 h2->GetXaxis()->SetTitle("X (fm)");
 h2->GetYaxis()->SetTitle("Y (fm)");
 h2->Draw("colz");
 
 TH2F *h22 = new TH2F("h22","nwB2",xbins2,xmin,xmax,ybins2,ymin,ymax);
 ntuple_B->Project("h22","y:x");
 h22->Draw("colz");


 gStyle->SetOptStat(1110);
 gPad->Update();
 TPaveStats *st2 = (TPaveStats*)h2->FindObject("stats");

 //c2->Print("Nucleus B Nw distribution.jpg");

 //auto c3 = new TCanvas("Nbin","Nbin",600,400);
 TH2F *h3 = new TH2F("h3","Nbin distribution",xbins,xmin,xmax,ybins,ymin,ymax);
 ntuple_nbin->Project("h3","y:x");
 h3->GetXaxis()->SetTitle("X (fm)");
 h3->GetYaxis()->SetTitle("Y (fm)");  

 h3->Draw("colz");

// c3->Print("Nbin-sig=2,0.5.jpg");

TH2F *h32 = new TH2F("h32","Nbin distribution",xbins2,xmin,xmax,ybins2,ymin,ymax);
ntuple_nbin->Project("h32","y:x");
h32->Draw("colz");  

/////////////////////////////////////////////////////////
  //Histogramas de RDS
//auto cnwAB = new TCanvas("nwAB","nwAB",600,400);
TH2F *HnwAB = new TH2F("HnwAB","HnwAB",xbins,xmin,xmax,ybins,ymin,ymax);
HnwAB->Add(h1,h2,1,1);
HnwAB->Draw("colz");

TH2F *HnwAB2 = new TH2F("HnwAB2","HnwAB2",xbins2,xmin,xmax,ybins2,ymin,ymax);
HnwAB2->Add(h12,h22,1,1);
HnwAB2->Draw("colz");

Double_t ALPHA = 0.12;
gStyle->SetLegendFillColor(2);
auto crds = new TCanvas("RDS","RDS",1080,1080);
TH2F *hrds = new TH2F("hrds",Form("Energy Distribution (smoothed), b=%d, binwidth=0.2fm",b),xbins,xmin,xmax,ybins,ymin,ymax);

gStyle->SetPalette(53);
gStyle->SetAxisColor(TColor::GetColor("#E0C191"),"xy");
gStyle->SetLabelColor(TColor::GetColor("#E0C191"),"xyz");
gStyle->SetLabelSize(0.025,"xyz");
gStyle->SetStatColor(TColor::GetColor("#1d1d1f"));
gStyle->SetStatTextColor(TColor::GetColor("#E0C191"));
gStyle->SetStatFont(42);
gStyle->SetStatX(0.9);
gStyle->SetStatY(0.9);
gStyle->SetStatW(0.17);
gStyle->SetStatH(0.17);
gStyle->SetTitleFont(42);
crds->SetFillColor(TColor::GetColor("#1d1d1f"));
gStyle->SetTitleTextColor(TColor::GetColor("#E0C191"));

hrds->Add(HnwAB,h3,(1-ALPHA)/2,ALPHA);
Double_t RDS=hrds->GetEntries();
Double_t Escala = (7.88/RDS+1);  
hrds->Add(hrds, Escala-1);
Double_t binmax=hrds->GetBinContent(hrds->GetMaximumBin());
Double_t Mcc=2.54; //GeV - Massa de dois Charms - https://pdglive.lbl.gov/DataBlock.action?node=Q004M
Double_t threshold = Mcc/binmax;
hrds->ShowPeaks(1,"noMarkov",threshold/3);
hrds->Draw("colz");
hrds->Smooth(1,"k5b");

/////////////////////////////////////////////////////////
   //Coordenadas dos hotspots (hs)   
   
TList *functions = hrds->GetListOfFunctions();
Double_t* xhs;
Double_t* yhs;
TPolyMarker *pm = (TPolyMarker*)functions->FindObject("TPolyMarker");
pm->SetMarkerSize(1.2);
pm->SetMarkerColor(kAzure-2);
//crds->Print(Form("Energy Distribution (smoothed), b=%d, binwidth=0.2fm.png",b));
xhs=pm->GetX();
yhs=pm->GetY();

/////////////////////////////////////////////////////////

auto crdsA = new TCanvas("RDS0","RDS0",1080,1080);
TH2F *hrds0 = new TH2F("hrds0","Energy Distribution(sem smooth),bin=0.2fm",xbins,xmin,xmax,ybins,ymin,ymax);
hrds0->SetTitle(Form("Energy Distribution, b=%d, binwidth=0.2fm",b));
hrds0->Add(HnwAB,h3,(1-ALPHA)/2,ALPHA);
//cout<<"RDS="<<RDS;  
hrds0->Add(hrds0, Escala-1);
gStyle->SetPalette(53);
gStyle->SetAxisColor(TColor::GetColor("#E0C191"),"xy");
gStyle->SetLabelColor(TColor::GetColor("#E0C191"),"xyz");
crdsA->SetFillColor(TColor::GetColor("#1d1d1f"));
gStyle->SetTitleTextColor(TColor::GetColor("#E0C191"));

hrds0->Draw("colz");
pm->Draw();
//crdsA->Print(Form("Energy Distribution, b=%d, binwidth=0.2fm.png",b));
/*
printf("\nnbinarias=%i\nNw=%d\n",nbinarias,Nw);
printf("Nsources=%d\n", nlines);
cout<<"binmax="<<binmax<<endl;
cout<<"RDS="<<RDS;
*/

auto crds2 = new TCanvas("RDS2","RDS2",1080,1080);
TH2F *hrds2 = new TH2F("hrds2","Energy Distribution (bin_width=0.033333fm)",xbins2,xmin,xmax,ybins2,ymin,ymax);
gStyle->SetPalette(53);
gStyle->SetAxisColor(TColor::GetColor("#E0C191"),"xy");
gStyle->SetLabelColor(TColor::GetColor("#E0C191"),"xyz");
crds2->SetFillColor(TColor::GetColor("#1d1d1f"));
gStyle->SetTitleTextColor(TColor::GetColor("#E0C191"));

hrds2->SetTitle(Form("Energy Distribution (smoothed), b=%d, binwidth=0.033333fm",b));
hrds2->Add(HnwAB2,h32,(1-ALPHA)/2,ALPHA);
hrds2->Add(hrds2, Escala-1);
Double_t binmax2=hrds2->GetBinContent(hrds2->GetMaximumBin());
//cout<<endl<<"binmax2="<<binmax2<<endl;
hrds2->Smooth(1,"k5b");
hrds2->Draw("colz");
pm->Draw();
//crds2->Print(Form("Energy Distribution (smoothed), b=%d, binwidth=0.033333fm.png",b));

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//CÁLCULO DO eps (BASEADO NA LINHA 250 DO distrib.h)

Int_t Nbins = (xbins+2)*(ybins+2);                        
Int_t mf=2; // rank of the Fourier moment, m=0,2,3,4,5,...  
Double_t scfix=0;
Double_t ss=0;
Float_t numsin=0;  //Para phi rot

for(Int_t iy=0;iy<=ybins;iy++){
	for(Int_t ix=0;ix<=xbins;ix++){    
		Int_t bin = hrds->GetBin(ix,iy);            //bin= numero do bin
		Double_t bincont=hrds->GetBinContent(bin);  //bincont
		Double_t coordy=((TAxis*)hrds->GetYaxis())->GetBinCenter(iy);  //coord y
		Double_t coordx=((TAxis*)hrds->GetXaxis())->GetBinCenter(ix);  //coord x
		Double_t ph = atan2(coordy,coordx);
		Double_t r2 = (coordx*coordx)+(coordy*coordy);
		numsin     += r2*sin(mf*ph)*bincont;					  
		scfix      += (r2*cos(mf*ph))*bincont;		
		ss         += r2*bincont;	
		};
	}; 
Float_t phrot = atan2(numsin,scfix)/mf;
Double_t epsfixtotal=scfix/ss; 
//cout<<"phiRot="<<phrot<<endl;
//cout<<"binmax="<<binmax<<endl;

/////////////////////epspart//////////////////////////////////
Double_t scpart=0;
for(Int_t iy=0;iy<=ybins;iy++){
	for(Int_t ix=0;ix<=xbins;ix++){
		Int_t bin = hrds->GetBin(ix,iy);            //bin= numero do bin
		Double_t bincont=hrds->GetBinContent(bin);  //bincont    
		Double_t coordy=((TAxis*)hrds->GetYaxis())->GetBinCenter(iy);  //coord y
		Double_t coordx=((TAxis*)hrds->GetXaxis())->GetBinCenter(ix);  //coord x
		Double_t ph = atan2(coordy,coordx);
		Double_t r2 = (coordx*coordx)+(coordy*coordy);					  
		scpart     += (r2*cos(mf*(ph-phrot)))*bincont;
		};
	};	
Double_t epsparttotal = 	scpart/ss;

/////////////////////eps-HotSpots//////////////////////////////////

TH1F *hepsfix = new TH1F("hepsfix","eps_fix distribution",200,-1,1);
TH1F *hepspart = new TH1F("hepspart","eps_part distribution",200,0,1);
TH1F *henergia = new TH1F("henergia","HotSpots Energy distribution",100,0,25);
TH1F *hphrot = new TH1F("hphrot","HotSpots PhRot distribution",200,-3.5,3.5);

Int_t Nhs=pm->GetN();
Double_t somaepsfix=0;
Double_t somaepspart=0;
Double_t somaenergia=0;
cout<<endl<<"Numero de hotspots(Nhs)="<<Nhs<<endl;		//Numero de hs
cout<<"b="<<b<<endl;
cout<<"r_hs="<<r2_hs<<endl;
cout<<"SEED="<<SEED<<endl;
for(Int_t iyhs=0;iyhs<Nhs;iyhs++){     //itera sobre cada hs
		Double_t Energia=0;
		Int_t ixhs=iyhs;
		Int_t mf=2;
		Double_t numsin     = 0;					  
		Double_t scfix      = 0;		
		Double_t ss         = 0;
		Double_t scpart     = 0;
		//TNtuple *tHS = new TNtuple("xy-rela","xy-rela","coordx2:coordy2");
		vector<Double_t> xvet;
		vector<Double_t> yvet;
		vector<Double_t> bincontvet;
		for(Int_t iy2=0;iy2<=ybins2;iy2++){        //itera sobre cada ponto do hist de maior resolucao para calcular phrot
			for(Int_t ix2=0;ix2<=xbins2;ix2++){
				Double_t coordx2=((TAxis*)hrds2->GetXaxis())->GetBinCenter(ix2);  //coord x
				Double_t coordy2=((TAxis*)hrds2->GetYaxis())->GetBinCenter(iy2);  //coord y
				Int_t bin2 = hrds2->GetBin(ix2,iy2);					
				Double_t xrela2 = coordx2-xhs[ixhs];
				Double_t yrela2= coordy2-yhs[iyhs];
				Double_t R2=(xrela2*xrela2)+(yrela2*yrela2);
				Double_t bincont2 = hrds2->GetBinContent(bin2);  //bincont
				if(R2<=r2_hs){
					xvet.push_back(coordx2);
					yvet.push_back(coordy2);
					bincontvet.push_back(bincont2);
					//tHS->Fill(coordx2,coordy2);
					Double_t ph = atan2(yrela2,xrela2);
					numsin     += R2*sin(mf*ph)*bincont2;	   //numerador do phrot
								  
					scfix      += (R2*cos(mf*ph))*bincont2;	   //numerador do epsfix
					ss         += R2*bincont2;	
				    };
			};		    			
		};	
		Float_t phrot = atan2(numsin,scfix)/mf;
		hphrot->Fill(phrot);
		Double_t epsfix=scfix/ss;	
		hepsfix->Fill(epsfix);	
		numsin     = 0;					  
		scfix      = 0;		
		ss         = 0;
		scpart     = 0;
		//Int_t entradas=tHS->GetEntries();
		//cout<<"entradas="<<entradas<<endl;
		//tHS->Scan();
		Int_t xsize=xvet.size();		
		for (Int_t ix=0;ix<xsize;ix++){
		/*
		for(Int_t iy2=0;iy2<=ybins2;iy2++){        //Depois de calculado phrot, itera sobre cada ponto do hist para calcular epspart
			for(Int_t ix2=0;ix2<=xbins2;ix2++){
				Double_t coordy2=((TAxis*)hrds2->GetYaxis())->GetBinCenter(iy2);  //coord y
				Double_t coordx2=((TAxis*)hrds2->GetXaxis())->GetBinCenter(ix2);  //coord x

				if(R2<=r2_hs){
			*/
				//tHS->GetEntry(i);
				Double_t coordx2=xvet[ix];
				Double_t coordy2=yvet[ix];
				Double_t bincont2=bincontvet[ix];
				Double_t xrela2 = coordx2-xhs[ixhs];
				Double_t yrela2= coordy2-yhs[iyhs];	
				Double_t R2=(xrela2*xrela2)+(yrela2*yrela2);
				//cout<<"x,y="<<endl<<coordx2<<endl<<coordy2<<endl;
				//Int_t bin2 = hrds2->GetBin(coordx2,coordy2);            //bin= numero do bin
				//Double_t bincont2=hrds2->GetBinContent(bin2);  //bincont    
				//cout<<bincont2<<endl<<endl;
				Double_t ph = atan2(yrela2,xrela2);
				Energia+=bincont2;				  
				scpart     += (R2*cos(mf*(ph-phrot)))*bincont2;
				ss         += R2*bincont2;
			};				
		Double_t epspart = 	scpart/ss;
		hepspart->Fill(epspart);
		henergia->Fill(Energia);
		somaepsfix+=epsfix;
		somaepspart+=epspart;
		somaenergia+=Energia;
		TNtuple *Tuplahs = new TNtuple(Form("HotSpot %d",iyhs),"data from ?","hsx:hsy:eps_fix:eps_part:phrot:Energia");
		Tuplahs->Fill(xhs[ixhs],yhs[iyhs],epsfix,epspart,phrot,Energia);
							  	
	};
Double_t mediaepsfix=somaepsfix/Nhs;
Double_t mediaepspart=somaepspart/Nhs;
Double_t mediaenergia=somaenergia/Nhs;
Double_t mediaphrot=hphrot->GetMean();

auto cepsfix = new TCanvas("epsfix","epsfix",1080,1080);
//cepsfix->SetFillColor(TColor::GetColor("#1d1d1f"));
gStyle->SetHistLineColor();
hepsfix->Draw("colz");

auto cepspart = new TCanvas("epspart","epspart",1080,1080);
//cepspart->SetFillColor(TColor::GetColor("#1d1d1f"));
//gStyle->SetHistLineColor(TColor::GetColor("#E0C191"));
hepspart->Draw("colz");

auto cenergia = new TCanvas("energia","energia",1080,1080);
//cenergia->SetFillColor(TColor::GetColor("#1d1d1f"));
//gStyle->SetHistLineColor(TColor::GetColor("#E0C191"));
henergia->Draw("colz");

auto cphrot = new TCanvas("phrot","phrot",1080,1080);
//cphrot->SetFillColor(TColor::GetColor("#1d1d1f"));
//gStyle->SetHistFillColor(TColor::GetColor("#E0C191"));
hphrot->Draw("colz");


Double_t desvpadmed_epsfix=hepsfix->GetStdDev();
Double_t desvpadmed_epspart=hepspart->GetStdDev();
Double_t desvpadmed_energia=henergia->GetStdDev();
Double_t desvpadmed_phrot=hphrot->GetStdDev();

TNtuple *TuplaResults = new TNtuple("Resultados","data from ?","eps_fix_fireball:eps_part_fireball:Media_phrot:Desvpadmed_phrot:Media_eps_fix_hs:desvpadmed_epsfix:Media_eps_part_hs:desvpadmed_epspart:MediaEnergia:desvpadmed_energia");
TuplaResults->Fill(epsfixtotal,epsparttotal,mediaphrot,desvpadmed_phrot,mediaepsfix,desvpadmed_epsfix,mediaepspart,desvpadmed_epspart,mediaenergia,desvpadmed_energia);    

  
//cout<<endl<<epsfixtotal<<endl;
//cout<<epsparttotal<<endl;
/*
printf("Media epsfix=%f\n",mediaepsfix);
printf("Desvpadmed_epsfix=%f\n",desvpadmed_epsfix);
printf("Media epspart=%f\n",mediaepspart);
printf("Desvpadmed_epspart=%f\n",desvpadmed_epspart);
printf("Media energia=%f\n",mediaenergia);
printf("Desvpadmed_energia=%f\n",desvpadmed_energia);
printf("Media phrot=%f\n",mediaphrot);
printf("Desvpadmed_phrot=%f\n",desvpadmed_phrot);
*/

/*
ofstream myfile;
  myfile.open (Form("Results\nb=%i\nseed=%i.txt",b,SEED),std::ios_base::app);
  myfile << Form("%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n",
  epsfixtotal,
  epsparttotal,
  mediaepsfix,
  desvpadmed_epsfix,
  mediaepspart,
  desvpadmed_epspart,
  mediaenergia,
  desvpadmed_energia,
  mediaphrot,
  desvpadmed_phrot);
  myfile.close();
*/


ofstream myfile (Form("/home/lucas/Downloads/root/Macro-Fev2022/ResultadosFinaisIC/DiferentesSeeds/r2_hs=%de-1/b=%d/ResultHS-b=%d.txt",r2_hs10,b,b),std::ios_base::app);
	myfile << Form("%f;%f;%f;%f;%f;%f;%f;%f;%f;%f\n",
  epsfixtotal,
  epsparttotal,
  mediaepsfix,
  desvpadmed_epsfix,
  mediaepspart,
  desvpadmed_epspart,
  mediaenergia,
  desvpadmed_energia,
  mediaphrot,
  desvpadmed_phrot);
	myfile.close();
	
cout<<"AAAA"<<Form("/home/lucas/Downloads/root/Macro-Fev2022/ResultadosFinaisIC/DiferentesSeeds/r2_hs=%de-1/ResultHS-Total.txt",r2_hs10)<<endl;	
	
ofstream myfile2 (Form("/home/lucas/Downloads/root/Macro-Fev2022/ResultadosFinaisIC/DiferentesSeeds/r2_hs=%de-1/ResultHS-Total.txt",r2_hs10),std::ios_base::app);
	myfile2 << Form("%f;%f;%f;%f;%f;%f;%f;%f;%f;%f\n",
  epsfixtotal,
  epsparttotal,
  mediaepsfix,
  desvpadmed_epsfix,
  mediaepspart,
  desvpadmed_epspart,
  mediaenergia,
  desvpadmed_energia,
  mediaphrot,
  desvpadmed_phrot);
	myfile2.close();	


f->Write();

}




