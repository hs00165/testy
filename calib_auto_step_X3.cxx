#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
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


void calib_auto_step_X3()
{

	//char gmfile[] = "SX3_res_strip_cal_params.txt";
	//char fcfile[] = "may_8_test4.txt";
	string gmfile = "GM_UU_res_strip_cal_params_f.txt";
	string fcfile;

	cout << endl;
	cout << "***************************************************************************" << endl;
	cout << "                           84Se SIDAR Auto Calibration                       " << endl;
	cout << endl;
	cout << "Input gain matched calibration file name:  ";
	//cin >> gmfile;
	cout << endl;
	cout << "Output fine calibration file name:  ";
	cin >> fcfile;
	cout << endl;
	cout << "starting fine calibration...";
	cout << endl;

	//============================================================================
	//*************************SIDAR CALIBRATION FILE*****************************
	//============================================================================
	//this is the array that will hold all of the E & dE calibration info 
	//for SIDAR. The format is as follows:
	//
	// [Detector#] [Strip#] [0] = slope 
	// [Detector#] [Strip#] [1] = offset	
	double calib_dE_SIDAR [7][17][2];
	double calib_E_SIDAR [7][17][2];

	std::string flag;
	std::string line;
	std::string temp_string, temp_string1, temp_string2, temp_string3;
	int C_det; //to pull the detector number from the calibration file
	int strip_no=0; //strip number from txt file
	int back_strip, front_strip, temp_int;
	double slope=0;
	double offset=0;
	double offX, offY, slopeGM, slopeEC, LERP, RERP;

	ifstream SIDAR_calib_params ("calib_params_SIDAR.txt");

	if(!SIDAR_calib_params)
	{
		cout << "Cannot open SIDAR calibration file, you doughnut..." << endl;
		return 1;
	}


	//=====================READING IN THE CALIBRATION FILE=========================
	while(getline(SIDAR_calib_params,line))
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


	SIDAR_calib_params.close();

	//============================================================================
	//**************************BB10 CALIBRATION FILE*****************************
	//============================================================================
	//this is the array that will hold all of the calibration info 
	//for the BB10s. The format is as follows:
	//
	// [Detector#] [Strip#] [0] = slope 
	// [Detector#] [Strip#] [1] = offset	
	double calib_BB10 [12][8][2];

	ifstream BB10_calib_params ("calib_params_BB10.txt");

	if(!BB10_calib_params)
	{
		cout << "Cannot open BB10 calibration file, you doughnut..." << endl;
		return 1;
	}

	//=====================READING IN THE CALIBRATION FILE=========================
	while(getline(BB10_calib_params,line))
	{
		temp_string1 = line.at(0);

		//For BB10 detectors
		if(temp_string1 != "B") //ie, if line doesnt contain strings
		{

			istringstream in(line);
			in >> strip_no >> slope >> offset;

			calib_BB10 [C_det][strip_no][0] = slope;
			calib_BB10 [C_det][strip_no][1] = offset;
		}

		//checks if the detector is BB10
		if(line.compare(0,4,"BB10") == 0)
		{
			//finds the corresponding detector number
			temp_string = line.at(6);
			istringstream(temp_string) >> C_det;
		}

	}
	BB10_calib_params.close();

	//============================================================================
	//***************************X3 CALIBRATION FILE******************************
	//============================================================================
	//this is the array that will hold all of the calibration info 
	//for the X3s. The format is as follows:
	//
	// [Detector#] [0] = slope 
	// [Detector#] [1] = offset

	double calib_X3 [12][2];

	ifstream X3_calib_params ("calib_params_X3.txt");

	if(!X3_calib_params)
	{
		cout << "Cannot open X3 calibration file, you doughnut..." << endl;
		return 1;
	}

	//=====================READING IN THE CALIBRATION FILE=========================
	while(getline(X3_calib_params,line))
	{

		if(flag=="X3 dE" && line.compare(0,5,"X3 dE")!=0)
		{
			istringstream in(line);
			in >> temp_int >> slope >> offset;	

			calib_X3 [C_det][0] = slope;
			calib_X3 [C_det][1] = offset;
		}

		if(line.compare(0,5,"X3 dE") == 0)
		{
			flag = "X3 dE";

			//finds the corresponding detector number
			temp_string = line.at(line.size()-1);
			istringstream(temp_string) >> C_det;

			if(line.size()==7)
			{
				C_det = C_det+10;
			}
		}
	}


	//============================================================================
	//**********************SX3 front side CALIBRATION FILE***********************
	//============================================================================
	//this is the array that will hold all of the front side SX3 calibration info 
	//The format is as follows:
	//
	// [Upstream (0)/Downstream (1)] [Detector#] [Strip#] [0] = Offset X 
	// [Upstream (0)/Downstream (1)] [Detector#] [Strip#] [1] = Offset Y
	// [Upstream (0)/Downstream (1)] [Detector#] [Strip#] [2] = Slope (gain match)
	// [Upstream (0)/Downstream (1)] [Detector#] [Strip#] [3] = Slope (energy calibration)
	// [Upstream (0)/Downstream (1)] [Detector#] [Strip#] [4] = Left edge relative pos (LERP) 
	// [Upstream (0)/Downstream (1)] [Detector#] [Strip#] [5] = Right edge relative pos (RERP)

	double calib_SX3 [2][12][4][6];

	ifstream SX3_calib_params;
	SX3_calib_params.open(gmfile.c_str());

	if(!SX3_calib_params)
	{
		cout << "Cannot open Super X3 calibration file, you doughnut..." << endl;
		return 1;
	}

	//=====================READING IN THE CALIBRATION FILE=========================
	while(getline(SX3_calib_params,line))
	{
		//temp_string2 = line.at(0);

		//If the barrel detector is Downstream
		if(!line.empty() && flag == "SuperX3 D")
		{	
			if(line.compare(0,3,"Res") == 0)
			{			
				temp_string2 = line.at(12);
				istringstream(temp_string2) >> strip_no;			

				istringstream in(line);
				in >> temp_string3 >> temp_string3 >> temp_string3 >> offX >> offY >> slopeGM >> slopeEC >> LERP >> RERP;				

				calib_SX3 [1][C_det][strip_no][0] = offX;
				calib_SX3 [1][C_det][strip_no][1] = offY;
				calib_SX3 [1][C_det][strip_no][2] = slopeGM;
				calib_SX3 [1][C_det][strip_no][3] = slopeEC;
				calib_SX3 [1][C_det][strip_no][4] = LERP;
				calib_SX3 [1][C_det][strip_no][5] = RERP;

				//cout << flag << " " << C_det << " " << strip_no << " " << calib_SX3 [1][C_det][strip_no][0] << " " << calib_SX3 [1][C_det][strip_no][1] << " " << calib_SX3 [1][C_det][strip_no][2] << " " << calib_SX3 [1][C_det][strip_no][3] << " " << calib_SX3 [1][C_det][strip_no][4] << " " << calib_SX3 [1][C_det][strip_no][5] << endl;
			}
		}

		//If the barrel detector is Upstream
		if(!line.empty() && flag == "SuperX3 U")
		{	
			if(line.compare(0,3,"Res") == 0)
			{			
				temp_string2 = line.at(12);
				istringstream(temp_string2) >> strip_no;			

				istringstream in(line);
				in >> temp_string3 >> temp_string3 >> temp_string3 >> offX >> offY >> slopeGM >> slopeEC >> LERP >> RERP;				

				calib_SX3 [0][C_det][strip_no][0] = offX;
				calib_SX3 [0][C_det][strip_no][1] = offY;
				calib_SX3 [0][C_det][strip_no][2] = slopeGM;
				calib_SX3 [0][C_det][strip_no][3] = slopeEC;
				calib_SX3 [0][C_det][strip_no][4] = LERP;
				calib_SX3 [0][C_det][strip_no][5] = RERP;

				//cout << flag << " " << C_det << " " << strip_no << " " << calib_SX3 [0][C_det][strip_no][0] << " " << calib_SX3 [0][C_det][strip_no][1] << " " << calib_SX3 [0][C_det][strip_no][2] << " " << calib_SX3 [0][C_det][strip_no][3] << " " << calib_SX3 [0][C_det][strip_no][4] << " " << calib_SX3 [0][C_det][strip_no][5] << endl;

			}
		}

		if(!line.empty())
		{
			if(line.compare(0,9,"SuperX3 D") == 0)
			{
				flag = "SuperX3 D";

				//cout << line.size() << endl;

				//finds the corresponding detector number
				temp_string = line.at(line.size()-1);
				istringstream(temp_string) >> C_det;

				if(line.size()==11)
				{
					C_det = C_det+10;
				}
			}

			if(line.compare(0,9,"SuperX3 U") == 0)
			{
				flag = "SuperX3 U";

				//cout << line.size() << endl;

				//finds the corresponding detector number
				temp_string = line.at(line.size()-1);
				istringstream(temp_string) >> C_det;

				if(line.size()==11)
				{
					C_det = C_det+10;
				}
			}
		}
	}


	//============================================================================
	//**********************SX3 bask side CALIBRATION FILE***********************
	//============================================================================
	//this is the array that will hold all of the back side SX3 calibration info 
	//The format is as follows:
	//
	// [Upstream (0)/Downstream (1)] [Detector#] [back_Strip#] [front_strip#] [0] = Slope 
	// [Upstream (0)/Downstream (1)] [Detector#] [back_Strip#] [front_strip#] [1] = Offset

	double calib_back_SX3 [2][12][4][4][2];

	ifstream back_SX3_calib_params ("cal_params_alpha_cal_E_01.txt");

	if(!back_SX3_calib_params)
	{
		cout << "Cannot open backside Super X3 calibration file, you doughnut..." << endl;
		return 1;
	}

	//=====================READING IN THE CALIBRATION FILE=========================
	while(getline(back_SX3_calib_params,line))
	{

		if(!line.empty())
		{
			
			if(flag == "SuperX3 U backside")
			{

				istringstream in(line);
				in >> back_strip >> temp_string3 >> front_strip >> slope >> offset;

				calib_back_SX3 [0][C_det][back_strip][front_strip/2][0] = slope;
				calib_back_SX3 [0][C_det][back_strip][front_strip/2][1] = offset;

			}
			if(flag == "SuperX3 D backside")
			{

				istringstream in(line);
				in >> back_strip >> temp_string3 >> front_strip >> slope >> offset;

				calib_back_SX3 [1][C_det][back_strip][front_strip/2][0] = slope;
				calib_back_SX3 [1][C_det][back_strip][front_strip/2][1] = offset;

			}



			if(line.find("back side") != std::string::npos && line.find("SuperX3 U") != std::string::npos)
			{
				flag = "SuperX3 U backside";
				//finds the corresponding detector number
				temp_string = line.at(line.size()-11);
				istringstream(temp_string) >> C_det;

				if(line.size()==21)
				{
					C_det = C_det+10;
				}
				
			}
			else if(line.find("back side") != std::string::npos && line.find("SuperX3 D") != std::string::npos)
			{
				flag = "SuperX3 D backside";
				//finds the corresponding detector number
				temp_string = line.at(line.size()-11);
				istringstream(temp_string) >> C_det;

				if(line.size()==21)
				{
					C_det = C_det+10;
				}	
				
			}
			else if(line.find("SIDAR") != std::string::npos)
			{
				flag = "SIDAR";
			}
		}
	}


	//============================================================================
	//**************************READING FROM RAW TTREE****************************
	//============================================================================

	TChain* Chain1= new TChain("se84");

	TCanvas *c1 = new TCanvas("c1","c1",500,500);

	//Calibration runs
	//Chain1->Add("/media/harry/GODDESS_ANA1/84Se/cal/alpha_1_sx3_up.root"); //Upstream SX3 calibration	
	//Chain1->Add("/media/harry/GODDESS_ANA1/84Se/cal/alpha_2_sx3_up.root"); //Upstream SX3 calibration	
	//Chain1->Add("/media/harry/GODDESS_ANA1/84Se/cal1/alpha_cal_E_1.root"); //E calibration
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/cal1/alpha_cal_dE_1.root"); //dE calibration run	

	TH2D* E_vs_strip = new TH2D("E_vs_strip", "E_vs_strip", 49, 0, 49,500,0,10);

	char hname[48];
	char hname2[48];
	TH1D *E_spec[7][17];
	TH1D *dE_spec[13][17];
	int s=1;
	for(int d=1;d<13;d++)
	{

			sprintf(hname,"E_D%dS%d",d,s);
			sprintf(hname2,"dE_D%dS%d",d,s);
			E_spec[d][s] = new TH1D(hname,hname,200,800,1600);
			dE_spec[d][s] = new TH1D(hname2,hname2,200,800,1600);
	}


	unsigned long long int nEntries = Chain1->GetEntries();
	//cout << "Entries:" << nEntries << endl;

	std::vector<Barrel_detclass>* BarrelUp;
	BarrelUp = new std::vector<Barrel_detclass>;

	std::vector<Barrel_detclass>* BarrelDown;
	BarrelDown = new std::vector<Barrel_detclass>;

	std::vector<SIDAR_detclass>* SIDAR;
	SIDAR = new std::vector<SIDAR_detclass>;

	Generic_detclass* EventScalers;
	EventScalers = new Generic_detclass;

	Generic_detclass* TDC;
	TDC = new Generic_detclass;

	std::vector<SIDAR_detclass>* cSIDAR;
	cSIDAR = new std::vector<SIDAR_detclass>;

	short trig;

	Chain1->SetBranchAddress("SIDAR", &SIDAR);
	Chain1->SetBranchAddress("BarrelUp", &BarrelUp);
	Chain1->SetBranchAddress("BarrelDown", &BarrelDown);
	Chain1->SetBranchAddress("EventScalers", &EventScalers);
	Chain1->SetBranchAddress("TDC", &TDC);
	Chain1->SetBranchAddress("trig", &trig);

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

	double cal_slope, cal_offset, energy;
	double prog_pos;
	double bar_width = 70.0;
	int min_strip_size, stripn;
	bool lflag = true;

	double E_n, E_f;

	TFile write("cal_testy.root", "recreate");

	TTree *cal_84Se = new TTree("cal_84Se", "Calibrated tree");
	cal_84Se->Branch("cSIDAR", &cSIDAR);

	SIDAR_detclass S_data; 

	for(unsigned long long int i=0;i<nEntries;i++)
	{
		Chain1->GetEntry(i);
		SIDAR_vect_size = SIDAR->size();
		BarrelUp_vect_size = BarrelUp->size();
		BarrelDown_vect_size = BarrelDown->size();

		//====================================================
		//*********************ORRUBA-UP**********************
		//====================================================
		for( int l=0;l<BarrelUp_vect_size;l++)
		{
			Barrel_detclass UpB = BarrelUp->at(l);

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
				cal_slope = calib_BB10[detID_UpB][dE_strip_UpB.at(k)][0];
				cal_offset = calib_BB10[detID_UpB][dE_strip_UpB.at(k)][1];

				energy = dE_energy_UpB.at(k)*cal_slope + cal_offset;

				cal_energy_BB10.push_back(energy);
			}

			//**********************Resistive strip detectors******************
			for( int j=0;j<E_front_cont_UpB.size();j++)
			{
				for( int h=j;h<E_front_cont_UpB.size();h++)
				{
					//************************STRIP 1*****************************
					if(E_front_cont_UpB.at(j)==1 && E_front_cont_UpB.at(h)==2)
					{
						//near side energy
						E_n = E_front_en_UpB.at(j)*calib_SX3 [0][detID_UpB][1][2] - calib_SX3 [0][detID_UpB][1][1];
						//far side energy
						E_f = E_front_en_UpB.at(h) - calib_SX3 [0][detID_UpB][1][0];
						//total strip energy
						E_en_UpB.push_back((E_n+E_f)*calib_SX3 [0][detID_UpB][1][3]);
						E_strip_UpB.push_back(1);
						//strip position
						E_pos_UpB.push_back(((E_n-E_f)/(E_n+E_f) - calib_SX3 [0][detID_UpB][1][4])*(1/(calib_SX3 [0][detID_UpB][1][5] - calib_SX3 [0][detID_UpB][1][4])));
						break;
					}
					if(E_front_cont_UpB.at(j)==2 && E_front_cont_UpB.at(h)==1)
					{
						//near side energy
						E_n = E_front_en_UpB.at(h)*calib_SX3 [0][detID_UpB][1][2] - calib_SX3 [0][detID_UpB][1][1];
						//far side energy
						E_f = E_front_en_UpB.at(j) - calib_SX3 [0][detID_UpB][1][0];
						//total strip energy
						E_en_UpB.push_back((E_n+E_f)*calib_SX3 [0][detID_UpB][1][3]);
						E_strip_UpB.push_back(1);
						//strip position
						E_pos_UpB.push_back(((E_n-E_f)/(E_n+E_f) - calib_SX3 [0][detID_UpB][1][4])*(1/(calib_SX3 [0][detID_UpB][1][5] - calib_SX3 [0][detID_UpB][1][4])));
						break;
					}
					//************************STRIP 2*****************************
					if(E_front_cont_UpB.at(j)==3 && E_front_cont_UpB.at(h)==4)
					{
						//near side energy
						E_n = E_front_en_UpB.at(j)*calib_SX3 [0][detID_UpB][2][2] - calib_SX3 [0][detID_UpB][2][1];
						//far side energy
						E_f = E_front_en_UpB.at(h) - calib_SX3 [0][detID_UpB][2][0];
						//total strip energy
						E_en_UpB.push_back((E_n+E_f)*calib_SX3 [0][detID_UpB][2][3]);
						E_strip_UpB.push_back(2);
						//strip position
						E_pos_UpB.push_back(((E_n-E_f)/(E_n+E_f) - calib_SX3 [0][detID_UpB][2][4])*(1/(calib_SX3 [0][detID_UpB][2][5] - calib_SX3 [0][detID_UpB][2][4])));
						break;
					}
					if(E_front_cont_UpB.at(j)==4 && E_front_cont_UpB.at(h)==3)
					{
						//near side energy
						E_n = E_front_en_UpB.at(h)*calib_SX3 [0][detID_UpB][2][2] - calib_SX3 [0][detID_UpB][2][1];
						//far side energy
						E_f = E_front_en_UpB.at(j) - calib_SX3 [0][detID_UpB][2][0];
						//total strip energy
						E_en_UpB.push_back((E_n+E_f)*calib_SX3 [0][detID_UpB][2][3]);
						E_strip_UpB.push_back(2);
						//strip position
						E_pos_UpB.push_back(((E_n-E_f)/(E_n+E_f) - calib_SX3 [0][detID_UpB][2][4])*(1/(calib_SX3 [0][detID_UpB][2][5] - calib_SX3 [0][detID_UpB][2][4])));
						break;
					}
					//************************STRIP 3*****************************
					if(E_front_cont_UpB.at(j)==6 && E_front_cont_UpB.at(h)==5)
					{
						//near side energy
						E_n = E_front_en_UpB.at(j)*calib_SX3 [0][detID_UpB][3][2] - calib_SX3 [0][detID_UpB][3][1];
						//far side energy
						E_f = E_front_en_UpB.at(h) - calib_SX3 [0][detID_UpB][3][0];
						//total strip energy
						E_en_UpB.push_back((E_n+E_f)*calib_SX3 [0][detID_UpB][3][3]);
						E_strip_UpB.push_back(3);
						//strip position
						E_pos_UpB.push_back(((E_n-E_f)/(E_n+E_f) - calib_SX3 [0][detID_UpB][3][4])*(1/(calib_SX3 [0][detID_UpB][3][5] - calib_SX3 [0][detID_UpB][3][4])));
						break;
					}
					if(E_front_cont_UpB.at(j)==5 && E_front_cont_UpB.at(h)==6)
					{
						//near side energy
						E_n = E_front_en_UpB.at(h)*calib_SX3 [0][detID_UpB][3][2] - calib_SX3 [0][detID_UpB][3][1];
						//far side energy
						E_f = E_front_en_UpB.at(j) - calib_SX3 [0][detID_UpB][3][0];
						//total strip energy
						E_en_UpB.push_back((E_n+E_f)*calib_SX3 [0][detID_UpB][3][3]);
						E_strip_UpB.push_back(3);
						//strip position
						E_pos_UpB.push_back(((E_n-E_f)/(E_n+E_f) - calib_SX3 [0][detID_UpB][3][4])*(1/(calib_SX3 [0][detID_UpB][3][5] - calib_SX3 [0][detID_UpB][3][4])));
						break;
					}
					//************************STRIP 4*****************************
					if(E_front_cont_UpB.at(j)==8 && E_front_cont_UpB.at(h)==7)
					{
						//near side energy
						E_n = E_front_en_UpB.at(j)*calib_SX3 [0][detID_UpB][4][2] - calib_SX3 [0][detID_UpB][4][1];
						//far side energy
						E_f = E_front_en_UpB.at(h) - calib_SX3 [0][detID_UpB][4][0];
						//total strip energy
						E_en_UpB.push_back((E_n+E_f)*calib_SX3 [0][detID_UpB][4][3]);
						E_strip_UpB.push_back(4);
						//strip position
						E_pos_UpB.push_back(((E_n-E_f)/(E_n+E_f) - calib_SX3 [0][detID_UpB][4][4])*(1/(calib_SX3 [0][detID_UpB][4][5] - calib_SX3 [0][detID_UpB][4][4])));
						break;
					}
					if(E_front_cont_UpB.at(j)==7 && E_front_cont_UpB.at(h)==8)
					{
						//near side energy
						E_n = E_front_en_UpB.at(h)*calib_SX3 [0][detID_UpB][4][2] - calib_SX3 [0][detID_UpB][4][1];
						//far side energy
						E_f = E_front_en_UpB.at(j) - calib_SX3 [0][detID_UpB][4][0];
						//total strip energy
						E_en_UpB.push_back((E_n+E_f)*calib_SX3 [0][detID_UpB][4][3]);
						E_strip_UpB.push_back(4);
						//strip position
						E_pos_UpB.push_back(((E_n-E_f)/(E_n+E_f) - calib_SX3 [0][detID_UpB][4][4])*(1/(calib_SX3 [0][detID_UpB][4][5] - calib_SX3 [0][detID_UpB][4][4])));
						break;
					}
				}
			}

			//**********************Backside Super X3s******************
			for(int g=0;g<E_back_strip_UpB.size();g++)
			{
				for(int f=0;f<E_strip_UpB.size();f++)
				{
					//if the position from the resistive front side is consistant with the backside strip hit
					if((E_back_strip_UpB.at(g)==1 && (E_pos_UpB.at(f) >= 0 && E_pos_UpB.at(f) <= 0.3)) || (E_back_strip_UpB.at(g)==2 && (E_pos_UpB.at(f) >= 0.70 && E_pos_UpB.at(f) <= 1)) || (E_back_strip_UpB.at(g)==3 && (E_pos_UpB.at(f) >= 0.45 && E_pos_UpB.at(f) <= 0.75)) || (E_back_strip_UpB.at(g)==4 && (E_pos_UpB.at(f) >= 0.15 && E_pos_UpB.at(f) <= 0.45)))
					{
						E_cal_back_en_UpB.push_back(E_back_en_UpB.at(g)*calib_back_SX3 [0][detID_UpB][E_back_strip_UpB.at(g)][E_strip_UpB.at(f)][0] + calib_back_SX3 [0][detID_UpB][E_back_strip_UpB.at(g)][E_strip_UpB.at(f)][1]);
						E_cal_back_strip_UpB.push_back(E_back_strip_UpB.at(g));

					}
					
				}
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

			Barrel_detclass DownB = BarrelDown->at(l);

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
				cal_slope = calib_X3[detID_DownB][0];
				cal_offset = calib_X3[detID_DownB][1];

				energy = dE_energy_DownB.at(k)*cal_slope + cal_offset;

				cal_energy_X3.push_back(energy);

				dE_spec[detID_DownB][1]->Fill(dE_energy_DownB.at(k));

			}

			//**********************Resistive strip detectors******************
			for( int j=0;j<E_front_cont_DownB.size();j++)
			{
				for( int h=j;h<E_front_cont_DownB.size();h++)
				{
					//************************STRIP 1*****************************
					if(E_front_cont_DownB.at(j)==1 && E_front_cont_DownB.at(h)==2)
					{
						//near side energy
						E_n = E_front_en_DownB.at(j)*calib_SX3 [1][detID_DownB][1][2] - calib_SX3 [1][detID_DownB][1][1];
						//far side energy
						E_f = E_front_en_DownB.at(h) - calib_SX3 [1][detID_DownB][1][0];
						//total strip energy
						E_en_DownB.push_back((E_n+E_f)*calib_SX3 [1][detID_DownB][1][3]);
						E_strip_DownB.push_back(1);
						//strip position
						E_pos_DownB.push_back(((E_n-E_f)/(E_n+E_f) - calib_SX3 [1][detID_DownB][1][4])*(1/(calib_SX3 [1][detID_DownB][1][5] - calib_SX3 [1][detID_DownB][1][4])));

						break;
					}
					if(E_front_cont_DownB.at(j)==2 && E_front_cont_DownB.at(h)==1)
					{
						//near side energy
						E_n = E_front_en_DownB.at(h)*calib_SX3 [1][detID_DownB][1][2] - calib_SX3 [1][detID_DownB][1][1];
						//far side energy
						E_f = E_front_en_DownB.at(j) - calib_SX3 [1][detID_DownB][1][0];
						//total strip energy
						E_en_DownB.push_back((E_n+E_f)*calib_SX3 [1][detID_DownB][1][3]);
						E_strip_DownB.push_back(1);
						//strip position
						E_pos_DownB.push_back(((E_n-E_f)/(E_n+E_f) - calib_SX3 [1][detID_DownB][1][4])*(1/(calib_SX3 [1][detID_DownB][1][5] - calib_SX3 [1][detID_DownB][1][4])));

						break;
					}
					//************************STRIP 2*****************************
					if(E_front_cont_DownB.at(j)==3 && E_front_cont_DownB.at(h)==4)
					{
						//near side energy
						E_n = E_front_en_DownB.at(j)*calib_SX3 [1][detID_DownB][2][2] - calib_SX3 [1][detID_DownB][2][1];
						//far side energy
						E_f = E_front_en_DownB.at(h) - calib_SX3 [1][detID_DownB][2][0];
						//total strip energy
						E_en_DownB.push_back((E_n+E_f)*calib_SX3 [1][detID_DownB][2][3]);
						E_strip_DownB.push_back(2);
						//strip position
						E_pos_DownB.push_back(((E_n-E_f)/(E_n+E_f) - calib_SX3 [1][detID_DownB][2][4])*(1/(calib_SX3 [1][detID_DownB][2][5] - calib_SX3 [1][detID_DownB][2][4])));

						break;
					}
					if(E_front_cont_DownB.at(j)==4 && E_front_cont_DownB.at(h)==3)
					{
						//near side energy
						E_n = E_front_en_DownB.at(h)*calib_SX3 [1][detID_DownB][2][2] - calib_SX3 [1][detID_DownB][2][1];
						//far side energy
						E_f = E_front_en_DownB.at(j) - calib_SX3 [1][detID_DownB][2][0];
						//total strip energy
						E_en_DownB.push_back((E_n+E_f)*calib_SX3 [1][detID_DownB][2][3]);
						E_strip_DownB.push_back(2);
						//strip position
						E_pos_DownB.push_back(((E_n-E_f)/(E_n+E_f) - calib_SX3 [1][detID_DownB][2][4])*(1/(calib_SX3 [1][detID_DownB][2][5] - calib_SX3 [1][detID_DownB][2][4])));

						break;
					}
					//************************STRIP 3*****************************
					if(E_front_cont_DownB.at(j)==6 && E_front_cont_DownB.at(h)==5)
					{
						//near side energy
						E_n = E_front_en_DownB.at(j)*calib_SX3 [1][detID_DownB][3][2] - calib_SX3 [1][detID_DownB][3][1];
						//far side energy
						E_f = E_front_en_DownB.at(h) - calib_SX3 [1][detID_DownB][3][0];
						//total strip energy
						E_en_DownB.push_back((E_n+E_f)*calib_SX3 [1][detID_DownB][3][3]);
						E_strip_DownB.push_back(3);
						//strip position
						E_pos_DownB.push_back(((E_n-E_f)/(E_n+E_f) - calib_SX3 [1][detID_DownB][3][4])*(1/(calib_SX3 [1][detID_DownB][3][5] - calib_SX3 [1][detID_DownB][3][4])));

						break;
					}
					if(E_front_cont_DownB.at(j)==5 && E_front_cont_DownB.at(h)==6)
					{
						//near side energy
						E_n = E_front_en_DownB.at(h)*calib_SX3 [1][detID_DownB][3][2] - calib_SX3 [1][detID_DownB][3][1];
						//far side energy
						E_f = E_front_en_DownB.at(j) - calib_SX3 [1][detID_DownB][3][0];
						//total strip energy
						E_en_DownB.push_back((E_n+E_f)*calib_SX3 [1][detID_DownB][3][3]);
						E_strip_DownB.push_back(3);
						//strip position
						E_pos_DownB.push_back(((E_n-E_f)/(E_n+E_f) - calib_SX3 [1][detID_DownB][3][4])*(1/(calib_SX3 [1][detID_DownB][3][5] - calib_SX3 [1][detID_DownB][3][4])));

						break;
					}
					//************************STRIP 4*****************************
					if(E_front_cont_DownB.at(j)==8 && E_front_cont_DownB.at(h)==7)
					{
						//near side energy
						E_n = E_front_en_DownB.at(j)*calib_SX3 [1][detID_DownB][4][2] - calib_SX3 [1][detID_DownB][4][1];
						//far side energy
						E_f = E_front_en_DownB.at(h) - calib_SX3 [1][detID_DownB][4][0];
						//total strip energy
						E_en_DownB.push_back((E_n+E_f)*calib_SX3 [1][detID_DownB][4][3]);
						E_strip_DownB.push_back(4);
						//strip position
						E_pos_DownB.push_back(((E_n-E_f)/(E_n+E_f) - calib_SX3 [1][detID_DownB][4][4])*(1/(calib_SX3 [1][detID_DownB][4][5] - calib_SX3 [1][detID_DownB][4][4])));

						break;
					}
					if(E_front_cont_DownB.at(j)==7 && E_front_cont_DownB.at(h)==8)
					{
						//near side energy
						E_n = E_front_en_DownB.at(h)*calib_SX3 [1][detID_DownB][4][2] - calib_SX3 [1][detID_DownB][4][1];
						//far side energy
						E_f = E_front_en_DownB.at(j) - calib_SX3 [1][detID_DownB][4][0];
						//total strip energy
						E_en_DownB.push_back((E_n+E_f)*calib_SX3 [1][detID_DownB][4][3]);
						E_strip_DownB.push_back(4);
						//strip position
						E_pos_DownB.push_back(((E_n-E_f)/(E_n+E_f) - calib_SX3 [1][detID_DownB][4][4])*(1/(calib_SX3 [1][detID_DownB][4][5] - calib_SX3 [1][detID_DownB][4][4])));

						break;
					}
				}
			}
/*
			for(int b =0;b<E_strip_DownB.size();b++)
			{
				E_spec[detID_DownB][E_strip_DownB.at(b)]->Fill(E_en_DownB.at(b));
			}
*/
			E_strip_DownB.clear();
			E_en_DownB.clear();
			E_pos_DownB.clear();
			cal_energy_X3.clear();
		}

		//====================================================
		//***********************SIDAR************************
		//====================================================
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

				//E_spec[detID_SIDAR][E_strip_SIDAR.at(k)]->Fill(E_energy_SIDAR.at(k));


			}

			//dE strips
			for(int k=0;k<dE_strip_SIDAR.size();k++)
			{
				cal_slope = calib_dE_SIDAR[detID_SIDAR][dE_strip_SIDAR.at(k)][0];
				cal_offset = calib_dE_SIDAR[detID_SIDAR][dE_strip_SIDAR.at(k)][1];

				energy = dE_energy_SIDAR.at(k)*cal_slope + cal_offset;

				cal_dE_energy_SIDAR.push_back(energy);		


				//dE_spec[detID_SIDAR][dE_strip_SIDAR.at(k)]->Fill(dE_energy_SIDAR.at(k));

	
			}
/*
			for(int b =0;b<E_strip_UpB.size();b++)
			{
				E_spec[detID_UpB][E_strip_UpB.at(b)]->Fill(E_en_UpB.at(b));
			}
*/
			S_data.E_energies = cal_E_energy_SIDAR;
			S_data.dE_energies = cal_dE_energy_SIDAR;
			S_data.E_strips = E_strip_SIDAR;
			S_data.dE_strips = dE_strip_SIDAR;
			S_data.detID = detID_SIDAR;

			cSIDAR->push_back(S_data);

			cal_E_energy_SIDAR.clear();			
			cal_dE_energy_SIDAR.clear();


		}

		cal_84Se->Fill();
		cSIDAR->clear();


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


	double cal_peak_pos[3] = {5.157, 5.486, 5.805};
	double new_SX3_cal_params[14][5][2];
	double slope2, intercept2;	
	double offX_new, offY_new;
	double X, Y, XX, YY, XY;
	double denominator, numerator;
	double avg_c_pos = 0;
	double avg_r_pos = 0;
	double reg, Sx, Sy;
	flag = "off";
	string hist;

	ofstream outfile;
	outfile.open(fcfile.c_str());

	outfile << "FINE CALIBRATION, CREATED:" << endl; 
	outfile << endl;
	outfile << "Columns are: Strip #     Offset X     Offset Y     Slope (gain match)    Slope (energy calibration)     Left Edge Relative Pos.     Right edge Relative Pos." << endl;
	outfile << endl;

	c1->cd();
	E_test->Draw();

/*
	for(int z=1;z<7;z++)
	{

		outfile << "SIDAR E" << z <<endl;

		for(int q=1;q<17;q++)
		{
			ostringstream inf;
			inf << "D" << z << "S" << q;
			string h = inf.str();
			const char* hist = h.c_str();
		
			E_spec[z][q]->Draw();

			TSpectrum* spec = new TSpectrum(3);

			Int_t nPeaks = spec->Search(E_spec[z][q],1, "", 0.1);
			Double_t *raw_peak_pos = spec->GetPositionX();
			cout << " " << endl;
			cout << cal_peak_pos[0] <<" "<< cal_peak_pos[1] <<" "<< cal_peak_pos[2] << endl;
			cout << raw_peak_pos[0] <<" "<< raw_peak_pos[1] <<" "<< raw_peak_pos[2] << endl;
			
			avg_r_pos = (raw_peak_pos[0] + raw_peak_pos[1] + raw_peak_pos[2])/3;
			avg_c_pos = (cal_peak_pos[0] + cal_peak_pos[1] + cal_peak_pos[2])/3;
			cout << "avgR " << avg_r_pos << " avgC " << avg_c_pos << endl; 

			X=0;
			Y=0;
			XX=0;
			YY=0;
			XY=0;
			numerator = 0;
			denominator = 0;


			for(int p=0;p<3;p++)
			{
				X += raw_peak_pos[p] - avg_r_pos;
				Y += cal_peak_pos[p] - avg_c_pos;
				XX += X*X;
				YY += Y*Y;
				XY += X*Y;

				numerator = numerator + (raw_peak_pos[p] - avg_r_pos)*(cal_peak_pos[p] - avg_c_pos);
				denominator = denominator + (raw_peak_pos[p] - avg_r_pos)*(raw_peak_pos[p] - avg_r_pos);
			}

			reg = XY/sqrt(XX*YY);
			Sx = sqrt(XX/3);
			Sy = sqrt(YY/3);


			cout << "reg " << reg << " Sx " << Sx << " Sy " << Sy << endl;

			slope2 = reg*(Sy/Sx);
			intercept2 = (avg_c_pos - slope2*avg_r_pos);
	
			new_SX3_cal_params[z][q][0] = slope2;
			new_SX3_cal_params[z][q][1] = intercept2;

			for(int r=0; r<3; r++)
			{
				cout << (raw_peak_pos[r]*slope2 + intercept2) <<"  ";
			}
			cout << endl;
			cout << " " << endl;

			outfile << q << "      " << slope2 << "      " << intercept2 << endl;
		}
	}
*/
	outfile << "HARRY's X3 CALIBRATION" << endl;
	int q=1;

	for(int z=1;z<13;z++)
	{

		outfile << "X3 dE" << z <<endl;


			ostringstream inf;
			inf << "D" << z << "S" << q;
			string h = inf.str();
			const char* hist = h.c_str();
		
			dE_spec[z][q]->Draw();

			TSpectrum* spec = new TSpectrum(3);

			Int_t nPeaks = spec->Search(dE_spec[z][q],1, "", 0.1);
			Double_t *raw_peak_pos = spec->GetPositionX();
			cout << " " << endl;
			cout << cal_peak_pos[0] <<" "<< cal_peak_pos[1] <<" "<< cal_peak_pos[2] << endl;
			cout << raw_peak_pos[0] <<" "<< raw_peak_pos[1] <<" "<< raw_peak_pos[2] << endl;
			
			avg_r_pos = (raw_peak_pos[0] + raw_peak_pos[1] + raw_peak_pos[2])/3;
			avg_c_pos = (cal_peak_pos[0] + cal_peak_pos[1] + cal_peak_pos[2])/3;
			cout << "avgR " << avg_r_pos << " avgC " << avg_c_pos << endl; 

			X=0;
			Y=0;
			XX=0;
			YY=0;
			XY=0;
			numerator = 0;
			denominator = 0;


			for(int p=0;p<3;p++)
			{
				X += raw_peak_pos[p] - avg_r_pos;
				Y += cal_peak_pos[p] - avg_c_pos;
				XX += X*X;
				YY += Y*Y;
				XY += X*Y;

				numerator = numerator + (raw_peak_pos[p] - avg_r_pos)*(cal_peak_pos[p] - avg_c_pos);
				denominator = denominator + (raw_peak_pos[p] - avg_r_pos)*(raw_peak_pos[p] - avg_r_pos);
			}

			reg = XY/sqrt(XX*YY);
			Sx = sqrt(XX/3);
			Sy = sqrt(YY/3);


			cout << "reg " << reg << " Sx " << Sx << " Sy " << Sy << endl;

			slope2 = reg*(Sy/Sx);
			intercept2 = (avg_c_pos - slope2*avg_r_pos);
	
			new_SX3_cal_params[z][q][0] = slope2;
			new_SX3_cal_params[z][q][1] = intercept2;

			for(int r=0; r<3; r++)
			{
				cout << (raw_peak_pos[r]*slope2 + intercept2) <<"  ";
			}
			cout << endl;
			cout << " " << endl;

			outfile << q << "      " << slope2 << "      " << intercept2 << endl;
	}



	outfile.close();




	std::cout << endl;
	std::cout << "~~fin~~" << endl;

	return;

}




























