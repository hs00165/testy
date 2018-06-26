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


void analysis_shell()
{

	//============================================================================
	//**************************READING FROM RAW TTREE****************************
	//============================================================================

	TChain* Chain1= new TChain("cal_84Se");

	Chain1->Add("run109_cal.root");

	unsigned long long int nEntries = Chain1->GetEntries();
	cout << "Entries:" << nEntries << endl;

	std::vector<Barrel_detclass>* cBarrelUp;
	cBarrelUp = new std::vector<Barrel_detclass>;

	std::vector<Barrel_detclass>* cBarrelDown;
	cBarrelDown = new std::vector<Barrel_detclass>;

	std::vector<SIDAR_detclass>* cSIDAR;
	cSIDAR = new std::vector<SIDAR_detclass>;

	short trig;

	Chain1->SetBranchAddress("cSIDAR", &cSIDAR);
	Chain1->SetBranchAddress("cBarrelUp", &cBarrelUp);
	Chain1->SetBranchAddress("cBarrelDown", &cBarrelDown);

	int SIDAR_vect_size;
	int BarrelUp_vect_size;
	int BarrelDown_vect_size;

	Int_t detID_SIDAR;
	std::vector<short> E_strip_SIDAR;
	std::vector<float> E_energy_SIDAR;
	std::vector<short> dE_strip_SIDAR;
	std::vector<float> dE_energy_SIDAR;
	std::vector<float> cal_dE_energy_SIDAR;
	std::vector<float> cal_E_energy_SIDAR;
	std::vector<float> total_energy_SIDAR;
	std::vector<float> cal_energy_BB10;
	std::vector<float> cal_energy_X3;

	Int_t detID_UpB;
	Int_t detID_DownB;
	std::vector<short> E_front_cont_UpB;
	std::vector<float> E_front_en_UpB;
	std::vector<short> E_back_strip_UpB;
	std::vector<float> E_back_en_UpB;
	std::vector<short> dE_strip_UpB;
	std::vector<float> dE_energy_UpB;
	std::vector<short> E_strip_UpB;
	std::vector<float> E_en_UpB;
	std::vector<float> E_pos_UpB;
	std::vector<float> E_cal_back_en_UpB;
	std::vector<short> E_cal_back_strip_UpB;

	std::vector<short> E_front_cont_DownB;
	std::vector<float> E_front_en_DownB;
	std::vector<short> E_back_strip_DownB;
	std::vector<float> E_back_en_DownB;
	std::vector<short> dE_strip_DownB;
	std::vector<float> dE_energy_DownB;
	std::vector<short> E_strip_DownB;
	std::vector<float> E_en_DownB;
	std::vector<float> E_pos_DownB;
	std::vector<float> E_cal_back_en_DownB;
	std::vector<short> E_cal_back_strip_DownB;

	std::vector<int> Scaler_Ch;
	std::vector<double> Scaler_Val;
	std::vector<int> TDC_Ch;
	std::vector<double> TDC_Val;
	double UpMCP, XF_scint, RF_vetoed, RF, DownMCP, UpMCP_viaMPD4, DownMCP_viaMPD4;

	unsigned long long int val_901,val_902,val_903,val_905,val_906;
	unsigned long long int val_908,val_909,val_998,val_999;
	unsigned long long int ORRUBA_10MHz_clock, other_100Hz_clock;
	unsigned long long int vetoed_100Hz_clock, VME_clock;

	int inv_strip;
	double dx, dy, x_hit, y_hit, angle_SIDAR, angle_UpB, angle_DownB;
	double qValue;

	double cal_slope, cal_offset, energy;
	double prog_pos;
	double bar_width = 70.0;
	int min_strip_size;
	bool lflag = true;

	double E_n, E_f;

	//TFile write(outname.c_str(), "recreate");


	for(unsigned long long int i=0;i<nEntries;i++)
	{
		Chain1->GetEntry(i);
		SIDAR_vect_size = cSIDAR->size();
		BarrelUp_vect_size = cBarrelUp->size();
		BarrelDown_vect_size = cBarrelDown->size();

		//====================================================
		//*********************ORRUBA-UP**********************
		//====================================================
		for( int l=0;l<BarrelUp_vect_size;l++)
		{
			Barrel_detclass UpB = cBarrelUp->at(l);

			detID_UpB = UpB.detID;

			E_front_cont_UpB = UpB.E_front_contacts;
			E_front_en_UpB = UpB.E_front_energies;

			E_back_strip_UpB = UpB.E_back_strips;
			E_back_en_UpB = UpB.E_back_energies;

			dE_strip_UpB = UpB.dE_strips;
			dE_energy_UpB = UpB.dE_energies;


			//*************************BB10 detectors***********************
			for(int k=0;k<dE_strip_UpB.size();k++)
			{
				cout << "test BB10" << endl;
			}

			//**********************Resistive strip detectors******************
			for( int j=0;j<E_front_cont_UpB.size();j++)
			{
				cout << "test Urs" << endl;
			}


			//Backside stuffs
			for(int g=0;g<E_back_strip_UpB.size();g++)
			{
				cout << "test U_back" << endl;
			}

			E_strip_UpB.clear();
			E_en_UpB.clear();
			E_pos_UpB.clear();
			cal_energy_BB10.clear();
			E_cal_back_strip_UpB.clear();
			E_cal_back_en_UpB.clear();


		}


		//====================================================
		//********************ORRUBA-DOWN*********************
		//====================================================
		for( int l=0;l<BarrelDown_vect_size;l++)
		{

			Barrel_detclass DownB = cBarrelDown->at(l);

			detID_DownB = DownB.detID;

			E_front_cont_DownB = DownB.E_front_contacts;
			E_front_en_DownB = DownB.E_front_energies;

			E_back_strip_DownB = DownB.E_back_strips;
			E_back_en_DownB = DownB.E_back_energies;

			dE_strip_DownB = DownB.dE_strips;
			dE_energy_DownB = DownB.dE_energies;

			//*************************X3 detectors***********************
			for(int k=0;k<dE_energy_DownB.size();k++)
			{
				cout << "test X3" << endl;
			}

			//**********************Resistive strip detectors******************
			for( int j=0;j<E_front_cont_DownB.size();j++)
			{
				cout << "test Drs" << endl;
			}

			//Backside
			for(int g=0;g<E_back_strip_DownB.size();g++)
			{
				cout << "test D_back" << endl;
			}

			E_strip_DownB.clear();
			E_en_DownB.clear();
			E_pos_DownB.clear();
			cal_energy_X3.clear();
			E_cal_back_strip_DownB.clear();
			E_cal_back_en_DownB.clear();
		}

		//====================================================
		//***********************SIDAR************************
		//====================================================
		for( int j=0;j<SIDAR_vect_size;j++)
		{
			SIDAR_detclass data = cSIDAR->at(j);

			detID_SIDAR = data.detID;

			E_strip_SIDAR = data.E_strips;
			E_energy_SIDAR = data.E_energies;

			dE_strip_SIDAR = data.dE_strips;
			dE_energy_SIDAR = data.dE_energies;

			//E strips
			for(int k=0;k<E_strip_SIDAR.size();k++)
			{
				cout << "test SIDAR_E" << endl;
			}

			//dE strips
			for(int k=0;k<dE_strip_SIDAR.size();k++)
			{
				cout << "test SIDAR_dE" << endl;		
			}

			cal_E_energy_SIDAR.clear();			
			cal_dE_energy_SIDAR.clear();
		}


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
			std::cout << "]" << int((1.0*i/nEntries)*100.0) << "%";
		}
		std::cout << "\r";

	}

	std::cout << endl;
	std::cout << "~~fin~~" << endl;

	return;

}




























