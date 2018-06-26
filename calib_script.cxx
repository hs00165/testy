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


void calib_script()
{
	string outname;

	//standard spiel
	cout << "=========================================================================" << endl;
	cout << "                        84Se(d,p) Calibration script                     " << endl;
	cout << "=========================================================================" << endl;
	cout << " " << endl;
	cout << "|=======================================================================|" << endl;
	cout << "|                                                                       |" << endl;
	cout << "|  This script will calibrate all dE and E detectors with               |" << endl;
	cout << "|  paramteres from the 'cal_params' folder.                             |" << endl;
	cout << "|                                                      Author: H. Sims  |" << endl;
	cout << "|=======================================================================|" << endl;
	cout << "Please enter the name of the output root file:  ";
	cin >> outname;
	cout << " " << endl;	                






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

	ifstream SIDAR_calib_params ("cal_params/SIDAR_fineEcal.txt");

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

	ifstream BB10_calib_params ("cal_params/BB10_fineEcal.txt");

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

	ifstream X3_calib_params ("cal_params/X3_fineEcal.txt");

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

	ifstream SX3_calib_params ("cal_params/SX3rs_fineEcal_orig_poscal.txt");

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

	ifstream back_SX3_calib_params ("cal_params/SX3_back_fineEcal.txt");

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
				in >> back_strip >> front_strip >> slope >> offset;

				calib_back_SX3 [0][C_det][back_strip][front_strip][0] = slope;
				calib_back_SX3 [0][C_det][back_strip][front_strip][1] = offset;

			}
			if(flag == "SuperX3 D backside")
			{

				istringstream in(line);
				in >> back_strip >> front_strip >> slope >> offset;

				calib_back_SX3 [1][C_det][back_strip][front_strip][0] = slope;
				calib_back_SX3 [1][C_det][back_strip][front_strip][1] = offset;

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
/*
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run69.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run77.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run78.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run79.root");

	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run80.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run81.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run82.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run83.root");

	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run87.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run88.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run89.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run90.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run91.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run92.root");
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run95.root");

	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run100.root"); //
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run101.root"); //
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run104.root"); //
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run105.root"); //
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run106.root"); //

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
*/
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run109.root");

	//Calibration runs
	//Chain1->Add("/media/harry/GODDESS_ANA1/84Se/cal/alpha_1_sx3_up.root"); //Upstream SX3 calibration	
	//Chain1->Add("/media/harry/GODDESS_ANA1/84Se/cal/alpha_2_sx3_up.root"); //Upstream SX3 calibration	
	//Chain1->Add("/media/harry/GODDESS_ANA1/84Se/cal1/alpha_cal_E_1.root"); //Downstream? SX3 calibration


	unsigned long long int nEntries = Chain1->GetEntries();
	cout << "Entries:" << nEntries << endl;

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

	CRDC_detclass* CRDC1;
	CRDC1 = new CRDC_detclass;

	CRDC_detclass* CRDC2;
	CRDC2 = new CRDC_detclass;


	//Output branches
	std::vector<SIDAR_detclass>* cSIDAR;
	cSIDAR = new std::vector<SIDAR_detclass>;

	std::vector<Barrel_detclass>* cBarrelUp;
	cBarrelUp = new std::vector<Barrel_detclass>;

	std::vector<Barrel_detclass>* cBarrelDown;
	cBarrelDown = new std::vector<Barrel_detclass>;

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

	int inv_strip;
	double dx, dy, x_hit, y_hit, angle_SIDAR, angle_UpB, angle_DownB;
	double qValue;

	double cal_slope, cal_offset, energy;
	double prog_pos;
	double bar_width = 70.0;
	int min_strip_size;
	bool lflag = true;

	double E_n, E_f;

	TCutG *SX3_prots = new TCutG("SX3_prots",14);
	SX3_prots->SetVarX("E");
	SX3_prots->SetVarY("dE");
   SX3_prots->SetPoint(0,0.42324,3.46939);
   SX3_prots->SetPoint(1,0.46875,2.90618);
   SX3_prots->SetPoint(2,2.97178,1.82199);
   SX3_prots->SetPoint(3,5.58859,1.37142);
   SX3_prots->SetPoint(4,9.07008,0.949014);
   SX3_prots->SetPoint(5,13.3935,0.597006);
   SX3_prots->SetPoint(6,14.8498,0.639247);
   SX3_prots->SetPoint(7,14.8271,1.16022);
   SX3_prots->SetPoint(8,12.6198,1.21654);
   SX3_prots->SetPoint(9,9.47967,1.39958);
   SX3_prots->SetPoint(10,5.77063,1.90648);
   SX3_prots->SetPoint(11,3.19933,2.41337);
   SX3_prots->SetPoint(12,0.673544,3.46939);
   SX3_prots->SetPoint(13,0.42324,3.46939);

   TCutG *SX3_prots_down = new TCutG("SX3_prots_down",14);
   SX3_prots_down->SetVarX("E");
   SX3_prots_down->SetVarY("dE");
   SX3_prots_down->SetPoint(0,0.845436,9.86317);
   SX3_prots_down->SetPoint(1,0.871369,8.97559);
   SX3_prots_down->SetPoint(2,4.55394,6.20192);
   SX3_prots_down->SetPoint(3,7.56224,4.67086);
   SX3_prots_down->SetPoint(4,10.9336,3.76109);
   SX3_prots_down->SetPoint(5,13.9678,3.73891);
   SX3_prots_down->SetPoint(6,13.7344,4.62648);
   SX3_prots_down->SetPoint(7,10.3112,4.71524);
   SX3_prots_down->SetPoint(8,8.46992,5.11464);
   SX3_prots_down->SetPoint(9,6.42116,6.15754);
   SX3_prots_down->SetPoint(10,3.6722,7.88831);
   SX3_prots_down->SetPoint(11,1.02697,9.86317);
   SX3_prots_down->SetPoint(12,1.02697,9.86317);
   SX3_prots_down->SetPoint(13,0.845436,9.86317);

   TCutG *SIDAR_prots = new TCutG("SIDAR_prots",13);
   SIDAR_prots->SetVarX("E");
   SIDAR_prots->SetVarY("dE");
   SIDAR_prots->SetPoint(0,0.723695,2.18029);
   SIDAR_prots->SetPoint(1,0.723695,1.80971);
   SIDAR_prots->SetPoint(2,2.25984,1.16118);
   SIDAR_prots->SetPoint(3,4.75181,0.751985);
   SIDAR_prots->SetPoint(4,9.12129,0.474044);
   SIDAR_prots->SetPoint(5,12.5349,0.412279);
   SIDAR_prots->SetPoint(6,12.5691,0.759706);
   SIDAR_prots->SetPoint(7,9.25783,0.744265);
   SIDAR_prots->SetPoint(8,6.32209,0.898676);
   SIDAR_prots->SetPoint(9,3.18153,1.30015);
   SIDAR_prots->SetPoint(10,1.71365,1.86375);
   SIDAR_prots->SetPoint(11,1.23574,2.17257);
   SIDAR_prots->SetPoint(12,0.723695,2.18029);





	TFile write(outname.c_str(), "recreate");

	TTree *cal_84Se = new TTree("cal_84Se", "Calibrated tree");
	cal_84Se->Branch("cSIDAR", &cSIDAR);
	cal_84Se->Branch("cBarrelUp", &cBarrelUp);
	cal_84Se->Branch("cBarrelDown", &cBarrelDown);

	SIDAR_detclass S_data;
	Barrel_detclass U_data;
	Barrel_detclass D_data; 


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


			//Backside stuffs
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
			
			for(int a =0;a<dE_strip_UpB.size();a++)
			{
				for(int b =0;b<E_strip_UpB.size();b++)
				{
					for(int c =0;c<E_cal_back_strip_UpB.size();c++)
					{
						//if the BB10 strips are consistant with the SX3 front side strips.
						if(((dE_strip_UpB.at(a) == 2*(5-E_strip_UpB.at(b)) || dE_strip_UpB.at(a) == 2*(5-E_strip_UpB.at(b))-1) && cal_energy_BB10.at(a)>=0.4) && SX3_prots->IsInside(E_cal_back_en_UpB.at(c),cal_energy_BB10.at(a)))
						{
							//angle calculations
							angle_UpB = 180.0 - (180.0/3.1415)*atan(101.6/(239.35-((1-E_pos_UpB.at(b))*75)));

							qValue = (86.0/85.0)*(E_cal_back_en_UpB.at(c)+cal_energy_BB10.at(a)) - 44.47 - 13.259*sqrt(E_cal_back_en_UpB.at(c)+cal_energy_BB10.at(a))*cos(angle_UpB*(3.1415/180.0));

						}
					}
				}
			}

			U_data.E_front_energies = E_en_UpB;
			U_data.dE_energies = cal_energy_BB10;
			U_data.E_front_contacts = E_strip_UpB;
			U_data.dE_strips = dE_strip_UpB;
			U_data.detID = detID_UpB;
			U_data.E_back_strips = E_cal_back_strip_UpB;
			U_data.E_back_energies = E_cal_back_en_UpB;

			cBarrelUp->push_back(U_data);

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

			//Backside
			for(int g=0;g<E_back_strip_DownB.size();g++)
			{
				for(int f=0;f<E_strip_DownB.size();f++)
				{
					//if the position from the resistive front side is consistant with the backside strip hit
					if((E_back_strip_DownB.at(g)==1 && (E_pos_DownB.at(f) >= 0 && E_pos_DownB.at(f) <= 0.3)) || (E_back_strip_DownB.at(g)==2 && (E_pos_DownB.at(f) >= 0.70 && E_pos_DownB.at(f) <= 1)) || (E_back_strip_DownB.at(g)==3 && (E_pos_DownB.at(f) >= 0.45 && E_pos_DownB.at(f) <= 0.75)) || (E_back_strip_DownB.at(g)==4 && (E_pos_DownB.at(f) >= 0.15 && E_pos_DownB.at(f) <= 0.45)))
					{
						E_cal_back_en_DownB.push_back(E_back_en_DownB.at(g)*calib_back_SX3 [1][detID_DownB][E_back_strip_DownB.at(g)][E_strip_DownB.at(f)][0] + calib_back_SX3 [1][detID_DownB][E_back_strip_DownB.at(g)][E_strip_DownB.at(f)][1]);
							
						E_cal_back_strip_DownB.push_back(E_back_strip_DownB.at(g));

					}
					
				}
			}

			for(int a =0;a<dE_strip_DownB.size();a++)
			{
				for(int b =0;b<E_strip_DownB.size();b++)
				{
					for(int c =0;c<E_cal_back_strip_DownB.size();c++)
					{
						//if the BB10 strips are consistant with the SX3 front side strips.
						if(SX3_prots_down->IsInside(E_cal_back_en_DownB.at(c),cal_energy_X3.at(a)))
						{
							//angle calculations
							angle_DownB = 180.0 - (180.0/3.1415)*atan(86.36/(138.27-((1-E_pos_DownB.at(b))*75)));

							qValue = (86.0/85.0)*(E_cal_back_en_DownB.at(c)+cal_energy_X3.at(a)) - 44.47 - 13.259*sqrt(E_cal_back_en_DownB.at(c)+cal_energy_X3.at(a))*cos(angle_DownB*(3.1415/180.0));

						}
					}
				}
			}

			D_data.E_front_energies = E_en_DownB;
			D_data.dE_energies = cal_energy_X3;
			D_data.E_front_contacts = E_strip_DownB;
			D_data.dE_strips = dE_strip_DownB;
			D_data.detID = detID_DownB;
			D_data.E_back_strips = E_cal_back_strip_DownB;
			D_data.E_back_energies = E_cal_back_en_DownB;

			cBarrelDown->push_back(D_data);

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

			for(int k=0;k<dE_strip_SIDAR.size();k++)
			{
				for(int l=0;l<E_strip_SIDAR.size();l++)
				{
					if(fabs(dE_strip_SIDAR.at(k)-E_strip_SIDAR.at(l))<=1 && cal_dE_energy_SIDAR.at(k) >= 0.4 && cal_E_energy_SIDAR.at(l) >= 0.8 && SIDAR_prots->IsInside(cal_E_energy_SIDAR.at(l),cal_dE_energy_SIDAR.at(k)))
					{
						//Angle calculations
						inv_strip = dE_strip_SIDAR.at(k);
						dx = 3.41*inv_strip;
						dy = 3.66*inv_strip;
						x_hit = 268.2 + dx;
						y_hit = 97.62 - dy;
						angle_SIDAR = 180.0 - atan(y_hit/x_hit)*180.0/3.1415;

						qValue = (86.0/85.0)*(cal_dE_energy_SIDAR.at(k)+cal_E_energy_SIDAR.at(l)) - 44.47 - 13.259*sqrt(cal_dE_energy_SIDAR.at(k)+cal_E_energy_SIDAR.at(l))*cos(angle_SIDAR*(3.1415/180.0));
					}
				}
			}
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
		cBarrelUp->clear();
		cBarrelDown->clear();

		//====================================================
		//**********************SCALERS***********************
		//====================================================
		Scaler_Ch = EventScalers->channels;
		Scaler_Val = EventScalers->values;

		TDC_Ch = TDC->channels;
		TDC_Val = TDC->values;

		for( int j=0;j<Scaler_Ch.size();j++)
		{
			//filling scalars
			//timing_scalers->Fill(Scaler_Val.at(j),Scaler_Ch.at(j));

			//10MHz ORRUBA timestamp
			if(Scaler_Ch.at(j) == 901) val_901 = Scaler_Val.at(j);
			if(Scaler_Ch.at(j) == 902) val_902 = Scaler_Val.at(j);
			if(Scaler_Ch.at(j) == 903) val_903 = Scaler_Val.at(j);

			//100Hz clock 
			if(Scaler_Ch.at(j) == 905) val_905 = Scaler_Val.at(j);
			if(Scaler_Ch.at(j) == 906) val_906 = Scaler_Val.at(j);

			//vetoed 100 Hz clock
			if(Scaler_Ch.at(j) == 908) val_908 = Scaler_Val.at(j);
			if(Scaler_Ch.at(j) == 909) val_909 = Scaler_Val.at(j);

			//VME clock
			if(Scaler_Ch.at(j) == 998) val_998 = Scaler_Val.at(j);
			if(Scaler_Ch.at(j) == 999) val_999 = Scaler_Val.at(j);

		}

		ORRUBA_10MHz_clock = (val_901 << 32) + (val_902 << 16) + val_903;
		other_100Hz_clock = (val_905 << 16) + val_906;
		vetoed_100Hz_clock = (val_908 << 16) + val_909;
		VME_clock = (val_998 << 16) + val_999;


		//====================================================
		//************************TDC*************************
		//====================================================
		for( int j=0;j<TDC_Ch.size();j++)
		{
			if(TDC_Ch.at(j) == 806) XF_scint = TDC_Val.at(j);
			if(TDC_Ch.at(j) == 807) RF_vetoed = TDC_Val.at(j);
			if(TDC_Ch.at(j) == 808) RF = TDC_Val.at(j);
			if(TDC_Ch.at(j) == 809) UpMCP = TDC_Val.at(j);
			if(TDC_Ch.at(j) == 810) DownMCP = TDC_Val.at(j);
			if(TDC_Ch.at(j) == 811) UpMCP_viaMPD4 = TDC_Val.at(j);
			if(TDC_Ch.at(j) == 812) DownMCP_viaMPD4 = TDC_Val.at(j);
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


/*
	//Kinematic line calculations

	double T_p;

	for(double theta=1.5708;theta<3.1415;theta=theta+0.01)
	{
		T_p = 1.6901E-9*(5.08065E+10*pow((cos(theta),2) + 5.84885E+8*46.782 - sqrt(2.5813E+21*pow(cos(theta),4) + 5.94233E+19*pow(cos(theta),2)*46.782));	
	}
*/

	//expected ground state kinematics
	ifstream gState_kinematics ("gState_kinematics_84Se_dp.txt");
	if(!gState_kinematics)
	{
		cout << "Cannot open SIDAR calibration file, you doughnut..." << endl;
		return 1;
	}

	Int_t u=0;
	Double_t xK[100];
	Double_t yK[100];


	while(getline(gState_kinematics,line))
	{	

		istringstream in(line);
		in >> xK[u] >> yK[u];
		u++;

	}

	TGraph* gStateKin = new TGraph(u,xK,yK);




	std::cout << endl;
	std::cout << "~~fin~~" << endl;

	return;

}




























