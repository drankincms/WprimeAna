//#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include<sstream>
#include<cmath>
//#include<stdio.h>
  
using namespace std;
  
  
void alVSar_2D_plots_expected (std::string chan) {
  //"inner" array go through ar values. each inner array corresponds to al value. in  exp_1000[i][j] i goes through al values, j ar values.
  // 21 mass values
  Double_t obslim[11][11][31];
  Double_t theory[11][11];
  
  int gran = 300;
  
  //open file to save histograms
  TFile *file=new TFile("plots_expected.root","recreate");
  gStyle->SetTextFont(22);
  
  //make a 3d array (ar,al,mass) 
  Double_t x[11][11][31],t[11][11][31];
  Double_t m[31]={1000,1100,1200,1300,1400,1500,1600,1700,1800,1900,2000,2100,2200,2300,2400,2500,2600,2700,2800,2900,3000,3100,3200,3300,3400,3500,3600,3700,3800,3900,4000};
  Double_t ar[11]={0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1};
  Double_t al[11]={0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1};
  Double_t mlim[11][11];
  Double_t mlim_al[31][11];
  
  Double_t theoryR[31] = {2.6218,1.7833,1.2420,0.88188,0.63665,0.46602,0.34533,0.25886,0.19584,0.14922,0.11459,0.088534,0.068860,0.053847,0.042275,0.033343,0.026400,0.020986,0.016738,0.013397,0.010756,0.008669,0.006997,0.005672,0.004608,0.003758,0.003074,0.002523,0.002078,0.001718,0.001425};
  	  
  Double_t theoryM[31] = {4.8530,4.0787,3.5912,3.2735,3.0576,2.9107,2.8126,2.7431,2.6946,2.6614,2.6378,2.6209,2.6093,2.6019,2.5976,2.5948,2.5921,2.5917,2.5910,2.5918,2.5929,2.5931,2.5942,2.5977,2.5976,2.5993,2.6011,2.6024,2.6032,2.6048,2.6066};
  
  Double_t theoryL[31] = {4.6217,3.8865,3.4221,3.1252,2.9292,2.7983,2.7107,2.6508,2.6107,2.5848,2.5677,2.5567,2.5502,2.5469,2.5454,2.5457,2.5474,2.5493,2.5519,2.5551,2.5578,2.5620,2.5650,2.5672,2.5704,2.5733,2.5764,2.5790,2.5813,2.5837,2.5861};
  
  
  Double_t theorySM=3.44; // in pb
  
  
  
  
  	
  bool set=false;
  //Wprime_Histos_combined-4-1
  const char mid[] = "-";
  char filename [80];
  char line[80];
  string header;
  float dum[35],mx[35],y[35],y0lo[35],y0hi[35],y1lo[35],y1hi[35];
  int num=0;
  int numal=0;
  int numar=0;
  
  for (int il=0; il<11; il++){
    for (int ir=0; ir<11; ir++){
      //if (ir==0 && il==0){ continue;}
      //       cout << il << " "<< ir << endl;
      numal=il;
      numar=ir;
      sprintf(filename, "%s%d%s%d%s","bayesian_limits_expected_",numal,mid,numar,".txt");
      //cout << filename << endl;
      num++;
      //       cout << " analysis num " << num << endl;
      ifstream inFile;
      inFile.open(filename);
      //for (int i=0;i<5;i++){
        //inFile >> line;
        //cout << line << endl;
      //}
      int i=0;
      while(!inFile.eof() ){
        //inFile >> dum[i] >> mx[i] >> y[i] >> y0lo[i] >> y0hi[i] >> y1lo[i] >> y1hi[i];
        inFile >> mx[i] >> y[i] >> y0lo[i] >> y0hi[i] >> y1lo[i] >> y1hi[i];
        if (il==0 && ir==0) cout<<mx[i]<<" "<<y[i]<<" "<<y0lo[i]<<" "<<y0hi[i]<<" "<<y1lo[i]<<" "<<y1hi[i]<<endl;
        if (i<31) { // 21 mass values
          x[il][ir][i]=y[i];
        }
        i++;
        //std::cout<<mx[i-1]<<std::endl;
      }
      int imax=i-1;
      for (int ip=0;ip<imax;ip++){
        //cout<<mx[ip]<<" "<<y[ip]<<" "<<y0lo[ip]<<" "<<y0hi[ip]<<" "<<y1lo[ip]<<" "<<y1hi[ip]<<endl;
      }
    } 
     
  }
  
  
  // cout << il << " "<< ir << endl;
  /// this is the theory cross section for each al/ar combination -
  for (int k=0; k<11; k++) {
    for (int j=0; j<11; j++) {
      double aal=0.1*k;
      double aar=0.1*j;
      //     cout << " al/ar " << aal << " " << aar << k << " " << j <<endl;
      double rt_wgt=0.;
      double lt_wgt=0.;
      double st_wgt=0.;
      double mt_wgt=0.;
      if (k>0 || j>0) {
        rt_wgt=aar*aar*(aar*aar - aal*aal)/(aal*aal + aar*aar);
        lt_wgt=aal*aal*(aal*aal - aar*aar)/(aal*aal + aar*aar);
        st_wgt=(1 - aal*aal - 2*aal*aal*aar*aar/(aal*aal+aar*aar));
        mt_wgt=4*aal*aal*aar*aar/(aal*aal + aar*aar);
      } 
      for (int im=0; im<31; im++){
        t[k][j][im]=(1.0*(rt_wgt*theoryR[im]+lt_wgt*theoryL[im]+mt_wgt*theoryM[im])+st_wgt*theorySM);
        //x[k][j][im]=x[k][j][im];
        //x[k][j][im]=x[k][j][im]*t[k][j][im];
        //if (k==0 && j==0) cout << "al/ar/ mass "<< " " << aal << " " << aar << " " << m[im] << " " << x[k][j][im]<< " " << t[k][j][im]<< " " << st_wgt << " " << rt_wgt << " " << lt_wgt << " " << mt_wgt <<endl; 
  
      }
    }
  }
  
      
  
  
  //========================.
   // below this is the D0 code. there we had 9 masses... now we have 21 or 23  masses for CMS 
   //
  
   for (int i=0;i<11;i++){
     for (int j=0;j<11;j++){
       //find intersection between observed limit and predicted cross section
       for (int k=1;k<31;k++){
         if (x[i][j][k]>1.){
  	 mlim[i][j]=m[k]-(m[k]-m[k-1])*(x[i][j][k]-1.);
  	 set=true;
  	 cout<< "al/ar/exphi/explo/mlim " << i << " " << j << " " << x[i][j][k] << " " << x[i][j][k-1] << " " << mlim[i][j]<<endl;    
         break;
         }//if
       }//k
       if (!set) mlim[i][j]=999; set=false;
     }//j
   }// i
      
   //make a contour plot from the original grid ar vs al
   TH2 *obs2 = new TH2F("obs2","ar vs al", 11,-0.05,1.05, 11,-0.05, 1.05);
   for (int i=0;i< 11;i++){
     for (int j=0;j< 11;j++){
       //cout << mlim[i][j] << "; " ;
       obs2->SetBinContent(i+1,j+1,mlim[i][j]);
     }
     //cout <<endl;
   }
      
   Double_t ax[31],at[31];
  
   //interpolate linearly between grid points to make a finer grid
   Double_t marray[gran+1][gran+1];
   for (int j=0;j<gran+1;j++){
     float s=j;
     s=s/gran;
     int js=int(s*10);
     //cout<<"js="<<js<<endl;
     for (int i=0;i<gran+1;i++){
       float r=i;
       r=r/gran;
       int ir=int(r*10);
       //cout << "ir="<<ir<<endl;
       for (int k=0;k<31;k++){
         float ax1,at1,ax2,at2;
         if (ir<10){
  	   ax1=x[ir][js][k]+(10*r-ir)*(x[ir+1][js][k]-x[ir][js][k]);
  	   at1=t[ir][js][k]+(10*r-ir)*(t[ir+1][js][k]-t[ir][js][k]);
         }else{
  	   ax1=x[10][js][k];
  	   at1=t[10][js][k];
         }
         ax[k]=ax1;
         at[k]=at1;
         if (js<10){
  	   if (ir<10){
  	     ax2=x[ir][js+1][k]+(10*r-ir)*(x[ir+1][js+1][k]-x[ir][js+1][k]);
  	     at2=t[ir][js+1][k]+(10*r-ir)*(t[ir+1][js+1][k]-t[ir][js+1][k]);
  	   }else{
  	     ax2=x[10][js+1][k];
  	     at2=t[10][js+1][k];
  	   }
         }
         ax[k]=ax1+(10*s-js)*(ax2-ax1);
         at[k]=at1+(10*s-js)*(at2-at1);
         //cout << ax[k] <<";"<< at[k] << endl;
       }
  
       //find the mass limit
       //cout<<ax[0]<<" "<<at[0]<<endl;
       marray[i][j]=999;
       for (int k=1;k<31;k++){
         if (ax[k]>1.){
  	   marray[i][j]=m[k]-(m[k]-m[k-1])*(ax[k]-1.);
  	   break;
         }
       }
     }
   }
  
   //now we fill a histogram with the finer grid
  
   TH2 *obs1 = new TH2F("obs1","ar vs al", gran+1,-0.005,1.005, gran+1,-0.005, 1.005);
   for (int i=0;i<gran+1;i++){
     for (int j=0;j<gran+1;j++){
       //cout << marray[i][j] << ";" ;
       //mass limit cannot be below 800 because we have no data below 800
       if (marray[i][j]<999) marray[i][j]=999;
       obs1->SetBinContent(i+1,j+1,marray[i][j]);
     }
     //cout <<endl;
   }
     
   TCanvas *c1 = new TCanvas("c1","c1",0,0,600,600);
   gStyle->SetOptStat(kFALSE);
   gStyle->SetOptTitle(kFALSE);
  color=(TColor*)(gROOT->GetListOfColors()->At(100));
  color->SetRGB(1,0,0);
  color=(TColor*)(gROOT->GetListOfColors()->At(99));
  color->SetRGB(1,0.1,0.1);
  color=(TColor*)(gROOT->GetListOfColors()->At(98));
  color->SetRGB(1,0.2,0.2);
  color=(TColor*)(gROOT->GetListOfColors()->At(97));
  color->SetRGB(1,0.3,0.3);
  color=(TColor*)(gROOT->GetListOfColors()->At(96));
  color->SetRGB(1,0.4,0.4);
  color=(TColor*)(gROOT->GetListOfColors()->At(95));
  color->SetRGB(1,0.5,0.5);
  color=(TColor*)(gROOT->GetListOfColors()->At(94));
  color->SetRGB(1,0.6,0.6);
  color=(TColor*)(gROOT->GetListOfColors()->At(93));
  color->SetRGB(1,0.7,0.7);
  color=(TColor*)(gROOT->GetListOfColors()->At(92));
  color->SetRGB(1,0.8,0.8);
  color=(TColor*)(gROOT->GetListOfColors()->At(91));
  color->SetRGB(1,0.9,0.9);
  color=(TColor*)(gROOT->GetListOfColors()->At(90));
  color->SetRGB(1,1,1);
  Int_t color_indices[11]={90,91,92,93,94,95,96,97,98,99,100};
  gStyle->SetPalette(11,color_indices);
  
   c1->SetFillColor(kWhite); //add                                                                                           
   c1->SetBottomMargin(0.15); //change 2 0.15
   c1->SetRightMargin(0.15);
   c1->SetLeftMargin(0.17);
   obs1->SetContour(20,m);
  
   obs1->GetZaxis()->SetLabelFont(22);
  
  // obs1->Draw("cont4z");
   obs1->Draw("cont3zcol");
   obs1->SetLineWidth(3);
   c1->SetBottomMargin(0.15); //change 2 0.15
   c1->SetRightMargin(0.15);
   c1->SetLeftMargin(0.17);
   obs1->SetContour(20,m);
   obs1->Draw("cont2same");  
   obs1->GetYaxis()->SetTitleFont(22);
   obs1->GetXaxis()->SetTitleFont(22);
   obs1->GetYaxis()->SetTitleSize(0.07);
   obs1->GetXaxis()->SetTitleSize(0.07);
   obs1->GetXaxis()->SetLabelFont(22); //add times new roman 'bold'
   obs1->GetYaxis()->SetLabelFont(22); //add times new roman 'bold' 
   obs1->GetXaxis()->SetTitle("a^{L}");
   obs1->GetYaxis()->SetTitle("a^{R}");
   //obs1->GetXaxis()->SetTitleOffset(-0.5); //add
  
  
  TLatex *t1 = new TLatex();
  float x1=0.2;
  float y1=.85;
  
  t1->SetNDC();
  t1->SetTextFont(22);
  t1->SetTextColor(1);
  t1->SetTextAlign(11);
  t1->SetTextSize(0.06);
  t1->SetTextFont(22);
  
  TLatex *t1temp=0;
  //t1temp=t1->DrawLatex(x1,y1,"(b)   D\351 2.3 fb^{  -1}");
  //t1temp=t1->DrawLatex(x1,y1-0.09,"Limits on M(W')");
  c1->Update();
  //c1->Print(Form("contour_arVSal-%s_expected.pdf",chan.c_str()));
  //c1->Print(Form("contour_arVSal-%s_expected.png",chan.c_str()));
  
  // TCanvas *c2 = new TCanvas("c2","c2",600,0,600,600);
  
  // obs1->Draw("text");
   obs1->Write();
   obs2->Write();
      
}//file
