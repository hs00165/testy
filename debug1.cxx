#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdio>
#include <string>
#include <vector>
#include <map>
#include <random>
#include <cassert>
#include <time.h>
#include <math.h>
#include <ctype.h>
#include <list>
#include <functional>
#include <algorithm>
#include "se84_detclasses.h"


#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TChain.h"


void calib()
{

	//this is the array that will hold all of the E & dE calibration info 
	//for SIDAR. The format is as follows:
	//
	// [Detector#] [Strip#] [0] = slope 
	// [Detector#] [Strip#] [1] = offset	
	double calib_dE_SIDAR [7][17][2];
	double calib_E_SIDAR [7][17][2];

	std::string flag;
	std::string line;
	std::string temp_string, temp_string1;
	int C_det; //to pull the detector number from the calibration file
	int strip_no=0; //strip number from txt file
	double slope=0;
	double offset=0;

	ifstream E_calib_params ("calib_params_SIDAR.txt");

	if(!E_calib_params)
	{
		cout << "Cannot open E calibration file, you doughnut..." << endl;
		return 1;
	}


	//=====================READING IN THE CALIBRATION FILE=========================
	while(getline(E_calib_params,line))
	{
		temp_string1 = line.at(0);

		//For dE SIDAR detectors
		if(flag == "SIDAR dE" && (temp_string1 != "S" || temp_string1 != "X" || temp_string1 != "B")) //ie, if line doesnt contain strings
		{

			istringstream in(line);
			in >> strip_no >> slope >> offset;

			calib_dE_SIDAR [C_det][strip_no][0] = slope;
			calib_dE_SIDAR [C_det][strip_no][1] = offset;
		}
		//For E SIDAR detectors
		if(flag == "SIDAR E" && (temp_string1 != "S" || temp_string1 != "X" || temp_string1 != "B")) //ie, if line doesnt contain strings
		{

			istringstream in(line);
			in >> strip_no >> slope >> offset;

			calib_E_SIDAR [C_det][strip_no][0] = slope;
			calib_E_SIDAR [C_det][strip_no][1] = offset;
		}

		//checks if the detector is dE SIDAR
		if(line.compare(0,8,"SIDAR dE") == 0)
		{
			flag = "SIDAR dE";

			//finds the corresponding detector number
			temp_string = line.at(8);
			istringstream(temp_string) >> C_det;
		}
		//checks if the detector is E SIDAR
		else if(line.compare(0,7,"SIDAR E") == 0)
		{
			flag = "SIDAR E";

			//finds the corresponding detector number
			temp_string = line.at(7);
			istringstream(temp_string) >> C_det;
		}
	}


	E_calib_params.close();

	TChain* Chain1= new TChain("se84");


	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run100.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run101.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run104.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run105.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run106.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run107.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run108.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run109.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run110.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run111.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run112.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run113.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run114.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run115.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run116.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run117.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run118.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run119.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run120.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run121.root");

	TCanvas *c4 = new TCanvas("c4","c4",1000,1000);
	c4->Divide(2,2);


	TH2D* Energy_spectrum = new TH2D("Energy_spectrum", "Energy_spectrum", 100, 0.0, 10,18,0,17);
	TH2D* dE_E = new TH2D("dE_E", "dE_E", 200, 0, 20,200,0,20);
	TH2D* dE_E_prots = new TH2D("dE_E_prots", "dE_E_prots", 200, 0, 20,200,0,20);
	TH1D* Mult_spectrum = new TH1D("Mult_spectrum", "Ult_spectrum", 8, 0, 8);
	TH1D* E_spectrum = new TH1D("E_spectrum", "E_spectrum", 250, 0, 12);
	TH1D* E_spectrum_prots = new TH1D("E_spectrum_prots", "E_spectrum_prots", 250, 0, 12);
	TH2D* dE_vs_E = new TH2D("dE_vs_E", "dE_vs_E", 80, 0, 80, 80, 0, 80);




	unsigned long long int nEntries = Chain1->GetEntries();
	cout << "Entries:" << nEntries << endl;

	std::vector<SIDAR_detclass>* SIDAR;
	SIDAR = new std::vector<SIDAR_detclass>;

	short trig;

	Chain1->SetBranchAddress("SIDAR", &SIDAR);
	Chain1->SetBranchAddress("trig", &trig);

	int SIDAR_vect_size;
	Int_t detID_SIDAR;
	std::vector<short> E_strip_SIDAR;
	std::vector<float> E_energy_SIDAR;
	std::vector<short> dE_strip_SIDAR;
	std::vector<float> dE_energy_SIDAR;
	std::vector<float> cal_dE_energy_SIDAR;
	std::vector<float> cal_E_energy_SIDAR;
	std::vector<float> total_energy_SIDAR;

	double cal_slope, cal_offset, energy;
	int p_s, p_s_s, p_s_s_t;


	//graphical cut on protons in dE_E plot
	TCutG *protons = new TCutG("protons",9);
	protons->SetVarX("E");
	protons->SetVarY("dE");
   protons->SetPoint(0,2.16944,2.11341);
   protons->SetPoint(1,3.19997,2.17955);
   protons->SetPoint(2,4.56809,1.4066);
   protons->SetPoint(3,7.23326,0.943657);
   protons->SetPoint(4,10.9823,0.637785);
   protons->SetPoint(5,6.45148,0.579917);
   protons->SetPoint(6,3.66194,1.01393);
   protons->SetPoint(7,2.13391,1.99354);
   protons->SetPoint(8,2.16944,2.11341);

	for(unsigned long long int i=0;i<nEntries;i++)
	{
		Chain1->GetEntry(i);
		SIDAR_vect_size = SIDAR->size();

		for( int j=0;j<SIDAR_vect_size;j++)
		{
			SIDAR_detclass data = SIDAR->at(j);
			detID_SIDAR = data.detID;
			E_strip_SIDAR = data.E_strips;
			E_energy_SIDAR = data.E_energies;
			dE_strip_SIDAR = data.dE_strips;
			dE_energy_SIDAR = data.dE_energies;



			//E strips
			for(int k=0;k<E_strip_SIDAR.size();k++)
			{
				cal_slope = calib_E_SIDAR[detID_SIDAR][E_strip_SIDAR.at(k)][0];
				cal_offset = calib_E_SIDAR[detID_SIDAR][E_strip_SIDAR.at(k)][1];

				energy = E_energy_SIDAR.at(k)*cal_slope + cal_offset;

				cal_E_energy_SIDAR.push_back(energy);
			}



			//dE strips
			for(int k=0;k<dE_strip_SIDAR.size();k++)
			{
				cal_slope = calib_dE_SIDAR[detID_SIDAR][dE_strip_SIDAR.at(k)][0];
				cal_offset = calib_dE_SIDAR[detID_SIDAR][dE_strip_SIDAR.at(k)][1];

				energy = dE_energy_SIDAR.at(k)*cal_slope + cal_offset;

				cal_dE_energy_SIDAR.push_back(energy);			
			}


			Mult_spectrum->Fill(E_strip_SIDAR.size());

			if(dE_strip_SIDAR.size() >=1 && E_strip_SIDAR.size() >=1)
			{
				//Filling histograms - if mult=1, and dE strip is within 1 strip of E
				if(protons->IsInside(cal_dE_energy_SIDAR.at(0)+cal_E_energy_SIDAR.at(0),cal_dE_energy_SIDAR.at(0)))
				{

					if(dE_strip_SIDAR.size() == E_strip_SIDAR.size() && dE_strip_SIDAR.size() == 1 && fabs(dE_strip_SIDAR.at(0)-E_strip_SIDAR.at(0))<=1)
					{
						dE_E_prots->Fill(cal_dE_energy_SIDAR.at(0)+cal_E_energy_SIDAR.at(0),cal_dE_energy_SIDAR.at(0));

						//if S800 trigger
						if(trig == 2)
						{
							Energy_spectrum->Fill(cal_dE_energy_SIDAR.at(0)+cal_E_energy_SIDAR.at(0),dE_strip_SIDAR.at(0));

							E_spectrum_prots->Fill(cal_dE_energy_SIDAR.at(0)+cal_E_energy_SIDAR.at(0));

							p_s_s_t++;

						}	
						p_s_s++;
					
						E_spectrum->Fill(cal_dE_energy_SIDAR.at(0)+cal_E_energy_SIDAR.at(0));
	
					}
					for(int k=0;k<dE_strip_SIDAR.size();k++)
					{
						p_s++;
					}
					for(int k=0;k<E_strip_SIDAR.size();k++)
					{
						p_s++;
					}
				
					//dE_E->Fill(cal_dE_energy_SIDAR.at(0)+cal_E_energy_SIDAR.at(0),cal_dE_energy_SIDAR.at(0));
				}
			}


			cal_E_energy_SIDAR.clear();			
			cal_dE_energy_SIDAR.clear();
		}

	}

	cout<<"proton singles: "<< p_s <<" with strip gate: "<< p_s_s <<" with S800 & strip gate: "<< p_s_s_t << endl;

	c4->cd(1);
	dE_E->Draw();

	c4->cd(2);
	dE_E_prots->Draw();
	protons->Draw("SAME");

	c4->cd(3);
	E_spectrum->Draw();
	E_spectrum_prots->Draw("SAME");

	c4->cd(4);
	Energy_spectrum->Draw();






	

	return;




}




























