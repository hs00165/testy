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


void crap_thresh()
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

	ifstream E_calib_params ("calib_params_SIDAR_OLD.txt");

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





/*

	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run41.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run42.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run43.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run44.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run45.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run46.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run48.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run51.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run54.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run55.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run56.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run57.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run58.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run60.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run65.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run66.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run67.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run68.root");

*/
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run69.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run77.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run78.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run79.root");


	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run80.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run81.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run82.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run83.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run84.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run87.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run88.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run89.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run90.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run91.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run92.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run95.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run100.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run101.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run104.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run105.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run106.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run107.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run108.root");

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


	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run109.root");

	TCanvas *c1 = new TCanvas("c1","c1",1000,1000);
	TCanvas *c2 = new TCanvas("c2","c2",500,500);
	TCanvas *c3 = new TCanvas("c3","c3",500,500);
	TCanvas *c4 = new TCanvas("c4","c4",1000,1500);
	c1->Divide(2,2);
	c4->Divide(2,3);


	TH2D* Energy_spectrum = new TH2D("Energy_spectrum", "Energy_spectrum", 100, 0.0, 10,18,0,17);
	TH2D* dE_E = new TH2D("dE_E", "dE_E", 200, 0, 20,200,0,20);
	TH2D* dE_E_prots = new TH2D("dE_E_prots", "dE_E_prots", 200, 0, 20,200,0,20);
	TH2D* dE_E_trig = new TH2D("dE_E_trig", "dE_E_trig", 200, 0, 20,200,0,20);
	TH2D* dE_E_mult = new TH2D("dE_E_mult", "dE_E_mult", 200, 0, 20,200,0,20);

	TH2D* dE_E_temp = new TH2D("dE_E_temp", "dE_E_temp", 300, 0, 15,300,0,15);

	TH2D* dE_E_temp1 = new TH2D("dE_E_temp1", "dE_E_temp1", 300, 0, 15,300,0,15);
	TH2D* dE_E_temp2 = new TH2D("dE_E_temp2", "dE_E_temp2", 300, 0, 15,300,0,15);
	TH2D* dE_E_temp3 = new TH2D("dE_E_temp3", "dE_E_temp3", 300, 0, 15,300,0,15);
	TH2D* dE_E_temp4 = new TH2D("dE_E_temp4", "dE_E_temp4", 300, 0, 15,300,0,15);
	TH2D* dE_E_temp5 = new TH2D("dE_E_temp5", "dE_E_temp5", 300, 0, 15,300,0,15);
	TH2D* dE_E_temp6 = new TH2D("dE_E_temp6", "dE_E_temp6", 300, 0, 15,300,0,15);

	TH2D* E_theta = new TH2D("E_theta", "E_theta", 20, 160, 180,580,-12,12);


	TH1D* trig_spectrumdE = new TH1D("trig_spectrumdE", "trig_spectrumdE", 5, 0, 5);
	TH1D* strip_spec = new TH1D("strip_spec", "strip_spec", 17, 0, 17);
	TH1D* trig_spectrumE = new TH1D("trig_spectrumE", "trig_spectrumE", 5, 0, 5);
	TH1D* strip_spectrum = new TH1D("strip_spectrum", "strip_spectrum", 16, 1, 16);
	TH1D* Mult_spectrum_trash = new TH1D("Mult_spectrum_trash", "Mult_spectrum_trash", 8, 0, 8);


	TH1D* pE_spectrum1_4 = new TH1D("E_spectrum1_4", "E_spectrum1_4", 250, 0, 12);
	TH1D* pE_spectrum5_8 = new TH1D("E_spectrum5_8", "E_spectrum5_8", 250, 0, 12);
	TH1D* pE_spectrum9_12 = new TH1D("E_spectrum9_12", "E_spectrum9_12", 250, 0, 12);
	TH1D* pE_spectrum13_16 = new TH1D("E_spectrum13_16", "E_spectrum13_16", 250, 0, 12);
	TH1D* E_spectrum1_4 = new TH1D("E_spectrum1_4", "E_spectrum1_4", 250, 0, 12);
	TH1D* E_spectrum5_8 = new TH1D("E_spectrum5_8", "E_spectrum5_8", 250, 0, 12);
	TH1D* E_spectrum9_12 = new TH1D("E_spectrum9_12", "E_spectrum9_12", 250, 0, 12);
	TH1D* E_spectrum13_16 = new TH1D("E_spectrum13_16", "E_spectrum13_16", 250, 0, 12);

	TH1D* E_d1s1 = new TH1D("E_d1s1", "E_d1s1", 250, 0, 12);
	TH1D* E_d1s2 = new TH1D("E_d1s2", "E_d1s2", 250, 0, 12);
	TH1D* E_d1s3 = new TH1D("E_d1s3", "E_d1s3", 250, 0, 12);
	TH1D* E_d1s4 = new TH1D("E_d1s4", "E_d1s4", 250, 0, 12);
	TH1D* E_d1s5 = new TH1D("E_d1s5", "E_d1s5", 250, 0, 12);
	TH1D* E_d1s6 = new TH1D("E_d1s6", "E_d1s6", 250, 0, 12);
	TH1D* E_d1s7 = new TH1D("E_d1s7", "E_d1s7", 250, 0, 12);
	TH1D* E_d1s8 = new TH1D("E_d1s8", "E_d1s8", 250, 0, 12);
	TH1D* E_d1s9 = new TH1D("E_d1s9", "E_d1s9", 250, 0, 12);
	TH1D* E_d1s10 = new TH1D("E_d1s10", "E_d1s10", 250, 0, 12);
	TH1D* E_d1s11 = new TH1D("E_d1s11", "E_d1s11", 250, 0, 12);
	TH1D* E_d1s12 = new TH1D("E_d1s12", "E_d1s12", 250, 0, 12);
	TH1D* E_d1s13 = new TH1D("E_d1s13", "E_d1s13", 250, 0, 12);
	TH1D* E_d1s14 = new TH1D("E_d1s14", "E_d1s14", 250, 0, 12);
	TH1D* E_d1s15 = new TH1D("E_d1s15", "E_d1s15", 250, 0, 12);
	TH1D* E_d1s16 = new TH1D("E_d1s16", "E_d1s16", 250, 0, 12);

	TH1D* E_d2s1 = new TH1D("E_d2s1", "E_d2s1", 250, 0, 12);
	TH1D* E_d2s2 = new TH1D("E_d2s2", "E_d2s2", 250, 0, 12);
	TH1D* E_d2s3 = new TH1D("E_d2s3", "E_d2s3", 250, 0, 12);
	TH1D* E_d2s4 = new TH1D("E_d2s4", "E_d2s4", 250, 0, 12);
	TH1D* E_d2s5 = new TH1D("E_d2s5", "E_d2s5", 250, 0, 12);
	TH1D* E_d2s6 = new TH1D("E_d2s6", "E_d2s6", 250, 0, 12);
	TH1D* E_d2s7 = new TH1D("E_d2s7", "E_d2s7", 250, 0, 12);
	TH1D* E_d2s8 = new TH1D("E_d2s8", "E_d2s8", 250, 0, 12);
	TH1D* E_d2s9 = new TH1D("E_d2s9", "E_d2s9", 250, 0, 12);
	TH1D* E_d2s10 = new TH1D("E_d2s10", "E_d2s10", 250, 0, 12);
	TH1D* E_d2s11 = new TH1D("E_d2s11", "E_d2s11", 250, 0, 12);
	TH1D* E_d2s12 = new TH1D("E_d2s12", "E_d2s12", 250, 0, 12);
	TH1D* E_d2s13 = new TH1D("E_d2s13", "E_d2s13", 250, 0, 12);
	TH1D* E_d2s14 = new TH1D("E_d2s14", "E_d2s14", 250, 0, 12);
	TH1D* E_d2s15 = new TH1D("E_d2s15", "E_d2s15", 250, 0, 12);
	TH1D* E_d2s16 = new TH1D("E_d2s16", "E_d2s16", 250, 0, 12);

	TH1D* E_d3s1 = new TH1D("E_d3s1", "E_d3s1", 250, 0, 12);
	TH1D* E_d3s2 = new TH1D("E_d3s2", "E_d3s2", 250, 0, 12);
	TH1D* E_d3s3 = new TH1D("E_d3s3", "E_d3s3", 250, 0, 12);
	TH1D* E_d3s4 = new TH1D("E_d3s4", "E_d3s4", 250, 0, 12);
	TH1D* E_d3s5 = new TH1D("E_d3s5", "E_d3s5", 250, 0, 12);
	TH1D* E_d3s6 = new TH1D("E_d3s6", "E_d3s6", 250, 0, 12);
	TH1D* E_d3s7 = new TH1D("E_d3s7", "E_d3s7", 250, 0, 12);
	TH1D* E_d3s8 = new TH1D("E_d3s8", "E_d3s8", 250, 0, 12);
	TH1D* E_d3s9 = new TH1D("E_d3s9", "E_d3s9", 250, 0, 12);
	TH1D* E_d3s10 = new TH1D("E_d3s10", "E_d3s10", 250, 0, 12);
	TH1D* E_d3s11 = new TH1D("E_d3s11", "E_d3s11", 250, 0, 12);
	TH1D* E_d3s12 = new TH1D("E_d3s12", "E_d3s12", 250, 0, 12);
	TH1D* E_d3s13 = new TH1D("E_d3s13", "E_d3s13", 250, 0, 12);
	TH1D* E_d3s14 = new TH1D("E_d3s14", "E_d3s14", 250, 0, 12);
	TH1D* E_d3s15 = new TH1D("E_d3s15", "E_d3s15", 250, 0, 12);
	TH1D* E_d3s16 = new TH1D("E_d3s16", "E_d3s16", 250, 0, 12);

	TH1D* E_d4s1 = new TH1D("E_d4s1", "E_d4s1", 250, 0, 12);
	TH1D* E_d4s2 = new TH1D("E_d4s2", "E_d4s2", 250, 0, 12);
	TH1D* E_d4s3 = new TH1D("E_d4s3", "E_d4s3", 250, 0, 12);
	TH1D* E_d4s4 = new TH1D("E_d4s4", "E_d4s4", 250, 0, 12);
	TH1D* E_d4s5 = new TH1D("E_d4s5", "E_d4s5", 250, 0, 12);
	TH1D* E_d4s6 = new TH1D("E_d4s6", "E_d4s6", 250, 0, 12);
	TH1D* E_d4s7 = new TH1D("E_d4s7", "E_d4s7", 250, 0, 12);
	TH1D* E_d4s8 = new TH1D("E_d4s8", "E_d4s8", 250, 0, 12);
	TH1D* E_d4s9 = new TH1D("E_d4s9", "E_d4s9", 250, 0, 12);
	TH1D* E_d4s10 = new TH1D("E_d4s10", "E_d4s10", 250, 0, 12);
	TH1D* E_d4s11 = new TH1D("E_d4s11", "E_d4s11", 250, 0, 12);
	TH1D* E_d4s12 = new TH1D("E_d4s12", "E_d4s12", 250, 0, 12);
	TH1D* E_d4s13 = new TH1D("E_d4s13", "E_d4s13", 250, 0, 12);
	TH1D* E_d4s14 = new TH1D("E_d4s14", "E_d4s14", 250, 0, 12);
	TH1D* E_d4s15 = new TH1D("E_d4s15", "E_d4s15", 250, 0, 12);
	TH1D* E_d4s16 = new TH1D("E_d4s16", "E_d4s16", 250, 0, 12);

	TH1D* E_d5s1 = new TH1D("E_d5s1", "E_d5s1", 250, 0, 12);
	TH1D* E_d5s2 = new TH1D("E_d5s2", "E_d5s2", 250, 0, 12);
	TH1D* E_d5s3 = new TH1D("E_d5s3", "E_d5s3", 250, 0, 12);
	TH1D* E_d5s4 = new TH1D("E_d5s4", "E_d5s4", 250, 0, 12);
	TH1D* E_d5s5 = new TH1D("E_d5s5", "E_d5s5", 250, 0, 12);
	TH1D* E_d5s6 = new TH1D("E_d5s6", "E_d5s6", 250, 0, 12);
	TH1D* E_d5s7 = new TH1D("E_d5s7", "E_d5s7", 250, 0, 12);
	TH1D* E_d5s8 = new TH1D("E_d5s8", "E_d5s8", 250, 0, 12);
	TH1D* E_d5s9 = new TH1D("E_d5s9", "E_d5s9", 250, 0, 12);
	TH1D* E_d5s10 = new TH1D("E_d5s10", "E_d5s10", 250, 0, 12);
	TH1D* E_d5s11 = new TH1D("E_d5s11", "E_d5s11", 250, 0, 12);
	TH1D* E_d5s12 = new TH1D("E_d5s12", "E_d5s12", 250, 0, 12);
	TH1D* E_d5s13 = new TH1D("E_d5s13", "E_d5s13", 250, 0, 12);
	TH1D* E_d5s14 = new TH1D("E_d5s14", "E_d5s14", 250, 0, 12);
	TH1D* E_d5s15 = new TH1D("E_d5s15", "E_d5s15", 250, 0, 12);
	TH1D* E_d5s16 = new TH1D("E_d5s16", "E_d5s16", 250, 0, 12);

	TH1D* E_d6s1 = new TH1D("E_d6s1", "E_d6s1", 250, 0, 12);
	TH1D* E_d6s2 = new TH1D("E_d6s2", "E_d6s2", 250, 0, 12);
	TH1D* E_d6s3 = new TH1D("E_d6s3", "E_d6s3", 250, 0, 12);
	TH1D* E_d6s4 = new TH1D("E_d6s4", "E_d6s4", 250, 0, 12);
	TH1D* E_d6s5 = new TH1D("E_d6s5", "E_d6s5", 250, 0, 12);
	TH1D* E_d6s6 = new TH1D("E_d6s6", "E_d6s6", 250, 0, 12);
	TH1D* E_d6s7 = new TH1D("E_d6s7", "E_d6s7", 250, 0, 12);
	TH1D* E_d6s8 = new TH1D("E_d6s8", "E_d6s8", 250, 0, 12);
	TH1D* E_d6s9 = new TH1D("E_d6s9", "E_d6s9", 250, 0, 12);
	TH1D* E_d6s10 = new TH1D("E_d6s10", "E_d6s10", 250, 0, 12);
	TH1D* E_d6s11 = new TH1D("E_d6s11", "E_d6s11", 250, 0, 12);
	TH1D* E_d6s12 = new TH1D("E_d6s12", "E_d6s12", 250, 0, 12);
	TH1D* E_d6s13 = new TH1D("E_d6s13", "E_d6s13", 250, 0, 12);
	TH1D* E_d6s14 = new TH1D("E_d6s14", "E_d6s14", 250, 0, 12);
	TH1D* E_d6s15 = new TH1D("E_d6s15", "E_d6s15", 250, 0, 12);
	TH1D* E_d6s16 = new TH1D("E_d6s16", "E_d6s16", 250, 0, 12);


	TH1D* E_spectrum_prots = new TH1D("E_spectrum_prots", "E_spectrum_prots", 250, 0, 12);
	TH2D* dE_vs_E = new TH2D("dE_vs_E", "dE_vs_E", 80, 0, 80, 80, 0, 80);

	TH1D* Q_value = new TH1D("Q_value", "Q_value", 150, -6, 6);
	TH1D* Q_value_singles = new TH1D("Q_value_singles", "Q_value_singles", 150, -6, 6);


	unsigned long long int nEntries = Chain1->GetEntries();
	cout << "Entries:" << nEntries << endl;

	std::vector<SIDAR_detclass>* SIDAR;
	SIDAR = new std::vector<SIDAR_detclass>;

	MTDC_detclass* MTDC;
	MTDC = new MTDC_detclass;

	short trig;

	Chain1->SetBranchAddress("SIDAR", &SIDAR);
	Chain1->SetBranchAddress("trig", &trig);
	Chain1->SetBranchAddress("MTDC", &MTDC);

	int SIDAR_vect_size;
	Int_t detID_SIDAR;
	std::vector<short> E_strip_SIDAR;
	std::vector<float> E_energy_SIDAR;
	std::vector<short> dE_strip_SIDAR;
	std::vector<float> dE_energy_SIDAR;
	std::vector<float> cal_dE_energy_SIDAR;
	std::vector<float> cal_E_energy_SIDAR;
	std::vector<float> total_energy_SIDAR;

	std::vector<unsigned int> E1_up;
	std::vector<unsigned int> E1_dwn;

	double cal_slope, cal_offset, energy, qValue, theta, countn = 0;
	int min_strip_size;

	double progress, prog_pos;
	double bar_width = 70.0;

	int inv_strip;
	double x_i = 268.2, y_i = 97.62, dx, dy, x_hit, y_hit, angle_SIDAR;
	

	//graphical cut on protons in dE_E plot
	TCutG *protons = new TCutG("protons",9);
	protons->SetVarX("E");
	protons->SetVarY("dE");
   protons->SetPoint(0,2.16944,2.11341);
   protons->SetPoint(1,3.19997,2.17955);
   protons->SetPoint(2,4.56809,1.4066);
   protons->SetPoint(3,7.23326,0.943657);
   protons->SetPoint(4,10.9823,0.637785);
   protons->SetPoint(5,8.05148,0.519917);
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

			MTDC_detclass *MTDC_data = MTDC;
			E1_up = MTDC_data->e1up_hits;
			E1_dwn = MTDC_data->e1down_hits;

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

			//cout << E1_up.size() << endl;


			for(int k=0;k<dE_strip_SIDAR.size();k++)
			{
				for(int j=0;j<E_strip_SIDAR.size();j++)
				{

					if(fabs(dE_strip_SIDAR.at(k)-E_strip_SIDAR.at(j))<=1)
					{


						if(detID_SIDAR==1 && dE_strip_SIDAR.at(k)==1) E_d1s1->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==1 && dE_strip_SIDAR.at(k)==2) E_d1s2->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==1 && dE_strip_SIDAR.at(k)==3) E_d1s3->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==1 && dE_strip_SIDAR.at(k)==4) E_d1s4->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==1 && dE_strip_SIDAR.at(k)==5) E_d1s5->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==1 && dE_strip_SIDAR.at(k)==6) E_d1s6->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==1 && dE_strip_SIDAR.at(k)==7) E_d1s7->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==1 && dE_strip_SIDAR.at(k)==8) E_d1s8->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==1 && dE_strip_SIDAR.at(k)==9) E_d1s9->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==1 && dE_strip_SIDAR.at(k)==10) E_d1s10->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==1 && dE_strip_SIDAR.at(k)==11) E_d1s11->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==1 && dE_strip_SIDAR.at(k)==12) E_d1s12->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==1 && dE_strip_SIDAR.at(k)==13) E_d1s13->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==1 && dE_strip_SIDAR.at(k)==14) E_d1s14->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==1 && dE_strip_SIDAR.at(k)==15) E_d1s15->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==1 && dE_strip_SIDAR.at(k)==16) E_d1s16->Fill(cal_E_energy_SIDAR.at(j));

						if(detID_SIDAR==2 && dE_strip_SIDAR.at(k)==1) E_d2s1->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==2 && dE_strip_SIDAR.at(k)==2) E_d2s2->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==2 && dE_strip_SIDAR.at(k)==3) E_d2s3->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==2 && dE_strip_SIDAR.at(k)==4) E_d2s4->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==2 && dE_strip_SIDAR.at(k)==5) E_d2s5->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==2 && dE_strip_SIDAR.at(k)==6) E_d2s6->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==2 && dE_strip_SIDAR.at(k)==7) E_d2s7->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==2 && dE_strip_SIDAR.at(k)==8) E_d2s8->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==2 && dE_strip_SIDAR.at(k)==9) E_d2s9->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==2 && dE_strip_SIDAR.at(k)==10) E_d2s10->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==2 && dE_strip_SIDAR.at(k)==11) E_d2s11->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==2 && dE_strip_SIDAR.at(k)==12) E_d2s12->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==2 && dE_strip_SIDAR.at(k)==13) E_d2s13->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==2 && dE_strip_SIDAR.at(k)==14) E_d2s14->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==2 && dE_strip_SIDAR.at(k)==15) E_d2s15->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==2 && dE_strip_SIDAR.at(k)==16) E_d2s16->Fill(cal_E_energy_SIDAR.at(j));

						if(detID_SIDAR==3 && dE_strip_SIDAR.at(k)==1) E_d3s1->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==3 && dE_strip_SIDAR.at(k)==2) E_d3s2->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==3 && dE_strip_SIDAR.at(k)==3) E_d3s3->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==3 && dE_strip_SIDAR.at(k)==4) E_d3s4->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==3 && dE_strip_SIDAR.at(k)==5) E_d3s5->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==3 && dE_strip_SIDAR.at(k)==6) E_d3s6->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==3 && dE_strip_SIDAR.at(k)==7) E_d3s7->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==3 && dE_strip_SIDAR.at(k)==8) E_d3s8->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==3 && dE_strip_SIDAR.at(k)==9) E_d3s9->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==3 && dE_strip_SIDAR.at(k)==10) E_d3s10->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==3 && dE_strip_SIDAR.at(k)==11) E_d3s11->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==3 && dE_strip_SIDAR.at(k)==12) E_d3s12->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==3 && dE_strip_SIDAR.at(k)==13) E_d3s13->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==3 && dE_strip_SIDAR.at(k)==14) E_d3s14->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==3 && dE_strip_SIDAR.at(k)==15) E_d3s15->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==3 && dE_strip_SIDAR.at(k)==16) E_d3s16->Fill(cal_E_energy_SIDAR.at(j));

						if(detID_SIDAR==4 && dE_strip_SIDAR.at(k)==1) E_d4s1->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==4 && dE_strip_SIDAR.at(k)==2) E_d4s2->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==4 && dE_strip_SIDAR.at(k)==3) E_d4s3->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==4 && dE_strip_SIDAR.at(k)==4) E_d4s4->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==4 && dE_strip_SIDAR.at(k)==5) E_d4s5->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==4 && dE_strip_SIDAR.at(k)==6) E_d4s6->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==4 && dE_strip_SIDAR.at(k)==7) E_d4s7->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==4 && dE_strip_SIDAR.at(k)==8) E_d4s8->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==4 && dE_strip_SIDAR.at(k)==9) E_d4s9->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==4 && dE_strip_SIDAR.at(k)==10) E_d4s10->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==4 && dE_strip_SIDAR.at(k)==11) E_d4s11->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==4 && dE_strip_SIDAR.at(k)==12) E_d4s12->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==4 && dE_strip_SIDAR.at(k)==13) E_d4s13->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==4 && dE_strip_SIDAR.at(k)==14) E_d4s14->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==4 && dE_strip_SIDAR.at(k)==15) E_d4s15->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==4 && dE_strip_SIDAR.at(k)==16) E_d4s16->Fill(cal_E_energy_SIDAR.at(j));

						if(detID_SIDAR==5 && dE_strip_SIDAR.at(k)==1) E_d5s1->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==5 && dE_strip_SIDAR.at(k)==2) E_d5s2->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==5 && dE_strip_SIDAR.at(k)==3) E_d5s3->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==5 && dE_strip_SIDAR.at(k)==4) E_d5s4->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==5 && dE_strip_SIDAR.at(k)==5) E_d5s5->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==5 && dE_strip_SIDAR.at(k)==6) E_d5s6->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==5 && dE_strip_SIDAR.at(k)==7) E_d5s7->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==5 && dE_strip_SIDAR.at(k)==8) E_d5s8->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==5 && dE_strip_SIDAR.at(k)==9) E_d5s9->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==5 && dE_strip_SIDAR.at(k)==10) E_d5s10->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==5 && dE_strip_SIDAR.at(k)==11) E_d5s11->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==5 && dE_strip_SIDAR.at(k)==12) E_d5s12->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==5 && dE_strip_SIDAR.at(k)==13) E_d5s13->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==5 && dE_strip_SIDAR.at(k)==14) E_d5s14->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==5 && dE_strip_SIDAR.at(k)==15) E_d5s15->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==5 && dE_strip_SIDAR.at(k)==16) E_d5s16->Fill(cal_E_energy_SIDAR.at(j));

						if(detID_SIDAR==6 && dE_strip_SIDAR.at(k)==1) E_d6s1->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==6 && dE_strip_SIDAR.at(k)==2) E_d6s2->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==6 && dE_strip_SIDAR.at(k)==3) E_d6s3->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==6 && dE_strip_SIDAR.at(k)==4) E_d6s4->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==6 && dE_strip_SIDAR.at(k)==5) E_d6s5->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==6 && dE_strip_SIDAR.at(k)==6) E_d6s6->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==6 && dE_strip_SIDAR.at(k)==7) E_d6s7->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==6 && dE_strip_SIDAR.at(k)==8) E_d6s8->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==6 && dE_strip_SIDAR.at(k)==9) E_d6s9->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==6 && dE_strip_SIDAR.at(k)==10) E_d6s10->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==6 && dE_strip_SIDAR.at(k)==11) E_d6s11->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==6 && dE_strip_SIDAR.at(k)==12) E_d6s12->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==6 && dE_strip_SIDAR.at(k)==13) E_d6s13->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==6 && dE_strip_SIDAR.at(k)==14) E_d6s14->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==6 && dE_strip_SIDAR.at(k)==15) E_d6s15->Fill(cal_E_energy_SIDAR.at(j));
						if(detID_SIDAR==6 && dE_strip_SIDAR.at(k)==16) E_d6s16->Fill(cal_E_energy_SIDAR.at(j));









						//Angle calculations
						inv_strip = dE_strip_SIDAR.at(k);
						dx = 3.41*inv_strip;
						dy = 3.66*inv_strip;
						x_hit = 268.2 + dx;
						y_hit = 97.62 - dy;
						angle_SIDAR = 180.0 - atan(y_hit/x_hit)*180.0/3.1415;

						//cout << 180 - angle_SIDAR << endl;

						//if(cal_dE_energy_SIDAR.at(k) >= 0.4 && cal_E_energy_SIDAR.at(j) >= 1)
						//{
							trig_spectrumdE->Fill(dE_strip_SIDAR.size());
							trig_spectrumE->Fill(E_strip_SIDAR.size());

							if(detID_SIDAR==1 && dE_strip_SIDAR.at(k) !=2 && dE_strip_SIDAR.at(k) !=3)
							{
								//dE_E_temp1->Fill(cal_E_energy_SIDAR.at(j),cal_dE_energy_SIDAR.at(k));
				
								strip_spec->Fill(dE_strip_SIDAR.at(k));
							}
							if(detID_SIDAR==2 || detID_SIDAR==3 || detID_SIDAR==4 || detID_SIDAR==5 || detID_SIDAR==6)
							{
								//dE_E_temp2->Fill(cal_E_energy_SIDAR.at(j),cal_dE_energy_SIDAR.at(k));
							}
							if(detID_SIDAR==3)
							{
								//dE_E_temp3->Fill(cal_E_energy_SIDAR.at(j),cal_dE_energy_SIDAR.at(k));
							}
							if(detID_SIDAR==4)
							{
								dE_E_temp4->Fill(cal_E_energy_SIDAR.at(j),cal_dE_energy_SIDAR.at(k));
							}
							if(detID_SIDAR==5)
							{
								dE_E_temp5->Fill(cal_E_energy_SIDAR.at(j),cal_dE_energy_SIDAR.at(k));
							}
							if(detID_SIDAR==6)
							{
								dE_E_temp6->Fill(cal_E_energy_SIDAR.at(j),cal_dE_energy_SIDAR.at(k));
							}
						//}


						//attempt at excitation spectrum

						qValue = (86.0/85.0)*(cal_dE_energy_SIDAR.at(k)+cal_E_energy_SIDAR.at(j)) - 44.47 - 13.259*sqrt(cal_dE_energy_SIDAR.at(k)+cal_E_energy_SIDAR.at(j))*cos(angle_SIDAR*(3.1415/180.0));

						//cout << qValue << " " << (cal_dE_energy_SIDAR.at(k)+cal_E_energy_SIDAR.at(j)) << " " << angle_SIDAR << endl;

						if(protons->IsInside(cal_dE_energy_SIDAR.at(k)+cal_E_energy_SIDAR.at(j),cal_dE_energy_SIDAR.at(k)) && detID_SIDAR != 1)
						{
							if(E1_up.size()>=1)
							{
								Q_value->Fill(qValue);
								E_theta->Fill(angle_SIDAR,qValue);
							}
							Q_value_singles->Fill(qValue);

						}


						if(E1_up.size()>=1)
						{
							dE_E_temp1->Fill(cal_E_energy_SIDAR.at(j),cal_dE_energy_SIDAR.at(k));
						}
						if(E1_dwn.size()>=1)
						{
							dE_E_temp2->Fill(cal_E_energy_SIDAR.at(j),cal_dE_energy_SIDAR.at(k));
						}
						if(E1_up.size()>=1 && E1_dwn.size()>=1)
						{
							dE_E_temp3->Fill(cal_E_energy_SIDAR.at(j),cal_dE_energy_SIDAR.at(k));
						}




						if(dE_strip_SIDAR.at(k) <=6 && protons->IsInside(cal_dE_energy_SIDAR.at(k)+cal_E_energy_SIDAR.at(j),cal_dE_energy_SIDAR.at(k)) && detID_SIDAR != 1)
						{
							if(E1_up.size()>=1)
							{
								pE_spectrum1_4->Fill(cal_dE_energy_SIDAR.at(k)+cal_E_energy_SIDAR.at(j));

								countn++;
							}
							E_spectrum1_4->Fill(cal_dE_energy_SIDAR.at(k)+cal_E_energy_SIDAR.at(j));
						}

						if(dE_strip_SIDAR.at(k) <= 7 && dE_strip_SIDAR.at(k) >= 2 && protons->IsInside(cal_dE_energy_SIDAR.at(k)+cal_E_energy_SIDAR.at(j),cal_dE_energy_SIDAR.at(k)) && (detID_SIDAR != 1 && (dE_strip_SIDAR.at(k) !=2 && dE_strip_SIDAR.at(k) !=3)))
						{
							if(E1_up.size()>=1)
							{
								pE_spectrum5_8->Fill(cal_dE_energy_SIDAR.at(k)+cal_E_energy_SIDAR.at(j));
							}
							E_spectrum5_8->Fill(cal_dE_energy_SIDAR.at(k)+cal_E_energy_SIDAR.at(j));
						}

						if(protons->IsInside(cal_dE_energy_SIDAR.at(k)+cal_E_energy_SIDAR.at(j),cal_dE_energy_SIDAR.at(k)) && (detID_SIDAR != 1 && (dE_strip_SIDAR.at(k) !=2 && dE_strip_SIDAR.at(k) !=3)))
						{
							if(E1_up.size()>=1)
							{
								pE_spectrum9_12->Fill(cal_dE_energy_SIDAR.at(k)+cal_E_energy_SIDAR.at(j));
							}
							E_spectrum9_12->Fill(cal_dE_energy_SIDAR.at(k)+cal_E_energy_SIDAR.at(j));
						}

						if(dE_strip_SIDAR.at(k) >=10 && protons->IsInside(cal_dE_energy_SIDAR.at(k)+cal_E_energy_SIDAR.at(j),cal_dE_energy_SIDAR.at(k)) && (detID_SIDAR != 1 && (dE_strip_SIDAR.at(k) !=2 && dE_strip_SIDAR.at(k) !=3)))
						{
							if(E1_up.size()>=1)
							{
								pE_spectrum13_16->Fill(cal_dE_energy_SIDAR.at(k)+cal_E_energy_SIDAR.at(j));
							}
							E_spectrum13_16->Fill(cal_dE_energy_SIDAR.at(k)+cal_E_energy_SIDAR.at(j));
						}


					}
				}
			}

		}


		cal_E_energy_SIDAR.clear();			
		cal_dE_energy_SIDAR.clear();
		


		//Progress update
		if(floor(1.0*i/100000) == 1.0*i/100000)
		{
			std::cout << "[";
			prog_pos = bar_width*(1.0*i/nEntries);
			for(int h=0; h<bar_width; h++)
			{
				if(h<=prog_pos) std::cout << "=";
				else std::cout << " ";
			}
			std::cout << "]" << int((1.0*i/nEntries)*100.0) << "%" << " Trig count: " << countn;
		}
		std::cout << "\r";	
	}
	std::cout << endl;
	std::cout << "~~fin~~" << endl;


	TFile *dE_thresh = new TFile("E_thresh.root","RECREATE");

	E_d1s1->Write();
	E_d1s2->Write();
	E_d1s3->Write();
	E_d1s4->Write();
	E_d1s5->Write();
	E_d1s6->Write();
	E_d1s7->Write();
	E_d1s8->Write();
	E_d1s9->Write();
	E_d1s10->Write();
	E_d1s11->Write();
	E_d1s12->Write();
	E_d1s13->Write();
	E_d1s14->Write();
	E_d1s15->Write();
	E_d1s16->Write();

	E_d2s1->Write();
	E_d2s2->Write();
	E_d2s3->Write();
	E_d2s4->Write();
	E_d2s5->Write();
	E_d2s6->Write();
	E_d2s7->Write();
	E_d2s8->Write();
	E_d2s9->Write();
	E_d2s10->Write();
	E_d2s11->Write();
	E_d2s12->Write();
	E_d2s13->Write();
	E_d2s14->Write();
	E_d2s15->Write();
	E_d2s16->Write();

	E_d3s1->Write();
	E_d3s2->Write();
	E_d3s3->Write();
	E_d3s4->Write();
	E_d3s5->Write();
	E_d3s6->Write();
	E_d3s7->Write();
	E_d3s8->Write();
	E_d3s9->Write();
	E_d3s10->Write();
	E_d3s11->Write();
	E_d3s12->Write();
	E_d3s13->Write();
	E_d3s14->Write();
	E_d3s15->Write();
	E_d3s16->Write();

	E_d4s1->Write();
	E_d4s2->Write();
	E_d4s3->Write();
	E_d4s4->Write();
	E_d4s5->Write();
	E_d4s6->Write();
	E_d4s7->Write();
	E_d4s8->Write();
	E_d4s9->Write();
	E_d4s10->Write();
	E_d4s11->Write();
	E_d4s12->Write();
	E_d4s13->Write();
	E_d4s14->Write();
	E_d4s15->Write();
	E_d4s16->Write();

	E_d5s1->Write();
	E_d5s2->Write();
	E_d5s3->Write();
	E_d5s4->Write();
	E_d5s5->Write();
	E_d5s6->Write();
	E_d5s7->Write();
	E_d5s8->Write();
	E_d5s9->Write();
	E_d5s10->Write();
	E_d5s11->Write();
	E_d5s12->Write();
	E_d5s13->Write();
	E_d5s14->Write();
	E_d5s15->Write();
	E_d5s16->Write();

	E_d6s1->Write();
	E_d6s2->Write();
	E_d6s3->Write();
	E_d6s4->Write();
	E_d6s5->Write();
	E_d6s6->Write();
	E_d6s7->Write();
	E_d6s8->Write();
	E_d6s9->Write();
	E_d6s10->Write();
	E_d6s11->Write();
	E_d6s12->Write();
	E_d6s13->Write();
	E_d6s14->Write();
	E_d6s15->Write();
	E_d6s16->Write();



	c1->cd(1);
	E_spectrum1_4->Draw();
	pE_spectrum1_4->Draw("SAME");
	c1->cd(2);
	E_spectrum5_8->Draw();
	pE_spectrum5_8->Draw("SAME");
	c1->cd(3);
	E_spectrum9_12->Draw();
	pE_spectrum9_12->Draw("SAME");
	c1->cd(4);
	E_spectrum13_16->Draw();
	pE_spectrum13_16->Draw("SAME");

	c2->cd();
	Q_value_singles->Draw();
	Q_value->Draw("SAME");

	c3->cd();
	E_theta->Draw("COLZ");

	c4->cd(1);
	gPad->SetLogz();
	dE_E_temp1->Draw();


	c4->cd(2);
	gPad->SetLogz();
	dE_E_temp2->Draw();

	c4->cd(3);
	gPad->SetLogz();
	dE_E_temp3->Draw();

	c4->cd(4);
	gPad->SetLogz();
	dE_E_temp4->Draw();

	c4->cd(5);
	gPad->SetLogz();
	dE_E_temp5->Draw();


	c4->cd(6);
	gPad->SetLogz();
	dE_E_temp6->Draw();

	return;

}




























