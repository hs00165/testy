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


void calib_sx3()
{

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




	ifstream SX3_calib_params ("Resistive_Strips_Calib_Params.txt");

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

	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/run109.root");
*/
	//Calibration runs
	//Chain1->Add("/media/harry/GODDESS_ANA1/84Se/cal/alpha_1_sx3_up.root"); //Upstream SX3 calibration	
	//Chain1->Add("/media/harry/GODDESS_ANA1/84Se/cal/alpha_2_sx3_up.root"); //Upstream SX3 calibration	
	Chain1->Add("/media/harry/GODDESS_ANA1/84Se/cal/E_cal_test1.root"); //all SX3 calibration	

	TH2D* calib_Det1_Strip1 = new TH2D("calib_Det1_Strip1", "calib_Det1_Strip1", 500, 0, 4000,500,0,4000);
	TH2D* calib_Det1_Strip2 = new TH2D("calib_Det1_Strip2", "calib_Det1_Strip2", 500, 0, 4000,500,0,4000);
	TH2D* calib_Det1_Strip3 = new TH2D("calib_Det1_Strip3", "calib_Det1_Strip3", 500, 0, 4000,500,0,4000);
	TH2D* calib_Det1_Strip4 = new TH2D("calib_Det1_Strip4", "calib_Det1_Strip4", 500, 0, 4000,500,0,4000);

	TH2D* calib_Det2_Strip1 = new TH2D("calib_Det2_Strip1", "calib_Det2_Strip1", 500, 0, 4000,500,0,4000);
	TH2D* calib_Det2_Strip2 = new TH2D("calib_Det2_Strip2", "calib_Det2_Strip2", 500, 0, 4000,500,0,4000);
	TH2D* calib_Det2_Strip3 = new TH2D("calib_Det2_Strip3", "calib_Det2_Strip3", 500, 0, 4000,500,0,4000);
	TH2D* calib_Det2_Strip4 = new TH2D("calib_Det2_Strip4", "calib_Det2_Strip4", 500, 0, 4000,500,0,4000);

	TH2D* calib_Det3_Strip1 = new TH2D("calib_Det3_Strip1", "calib_Det3_Strip1", 500, 0, 4000,500,0,4000);
	TH2D* calib_Det3_Strip2 = new TH2D("calib_Det3_Strip2", "calib_Det3_Strip2", 500, 0, 4000,500,0,4000);
	TH2D* calib_Det3_Strip3 = new TH2D("calib_Det3_Strip3", "calib_Det3_Strip3", 500, 0, 4000,500,0,4000);
	TH2D* calib_Det3_Strip4 = new TH2D("calib_Det3_Strip4", "calib_Det3_Strip4", 500, 0, 4000,500,0,4000);

	TH2D* calib_Det4_Strip1 = new TH2D("calib_Det4_Strip1", "calib_Det4_Strip1", 500, 0, 4000,500,0,4000);
	TH2D* calib_Det4_Strip2 = new TH2D("calib_Det4_Strip2", "calib_Det4_Strip2", 500, 0, 4000,500,0,4000);
	TH2D* calib_Det4_Strip3 = new TH2D("calib_Det4_Strip3", "calib_Det4_Strip3", 500, 0, 4000,500,0,4000);
	TH2D* calib_Det4_Strip4 = new TH2D("calib_Det4_Strip4", "calib_Det4_Strip4", 500, 0, 4000,500,0,4000);

	TH2D* calib_Det5_Strip1 = new TH2D("calib_Det5_Strip1", "calib_Det5_Strip1", 500, 0, 4000,500,0,4000);
	TH2D* calib_Det5_Strip2 = new TH2D("calib_Det5_Strip2", "calib_Det5_Strip2", 500, 0, 4000,500,0,4000);
	TH2D* calib_Det5_Strip3 = new TH2D("calib_Det5_Strip3", "calib_Det5_Strip3", 500, 0, 4000,500,0,4000);
	TH2D* calib_Det5_Strip4 = new TH2D("calib_Det5_Strip4", "calib_Det5_Strip4", 500, 0, 4000,500,0,4000);

	TH2D* calib_Det6_Strip1 = new TH2D("calib_Det6_Strip1", "calib_Det6_Strip1", 500, 0, 4000,500,0,4000);
	TH2D* calib_Det6_Strip2 = new TH2D("calib_Det6_Strip2", "calib_Det6_Strip2", 500, 0, 4000,500,0,4000);
	TH2D* calib_Det6_Strip3 = new TH2D("calib_Det6_Strip3", "calib_Det6_Strip3", 500, 0, 4000,500,0,4000);
	TH2D* calib_Det6_Strip4 = new TH2D("calib_Det6_Strip4", "calib_Det6_Strip4", 500, 0, 4000,500,0,4000);

	TH2D* calib_Det7_Strip1 = new TH2D("calib_Det7_Strip1", "calib_Det7_Strip1", 500, 0, 4000,500,0,4000);
	TH2D* calib_Det7_Strip2 = new TH2D("calib_Det7_Strip2", "calib_Det7_Strip2", 500, 0, 4000,500,0,4000);
	TH2D* calib_Det7_Strip3 = new TH2D("calib_Det7_Strip3", "calib_Det7_Strip3", 500, 0, 4000,500,0,4000);
	TH2D* calib_Det7_Strip4 = new TH2D("calib_Det7_Strip4", "calib_Det7_Strip4", 500, 0, 4000,500,0,4000);

	TH2D* calib_Det8_Strip1 = new TH2D("calib_Det8_Strip1", "calib_Det8_Strip1", 500, 0, 4000,500,0,4000);
	TH2D* calib_Det8_Strip2 = new TH2D("calib_Det8_Strip2", "calib_Det8_Strip2", 500, 0, 4000,500,0,4000);
	TH2D* calib_Det8_Strip3 = new TH2D("calib_Det8_Strip3", "calib_Det8_Strip3", 500, 0, 4000,500,0,4000);
	TH2D* calib_Det8_Strip4 = new TH2D("calib_Det8_Strip4", "calib_Det8_Strip4", 500, 0, 4000,500,0,4000);

	TH2D* calib_Det9_Strip1 = new TH2D("calib_Det9_Strip1", "calib_Det9_Strip1", 500, 0, 4000,500,0,4000);
	TH2D* calib_Det9_Strip2 = new TH2D("calib_Det9_Strip2", "calib_Det9_Strip2", 500, 0, 4000,500,0,4000);
	TH2D* calib_Det9_Strip3 = new TH2D("calib_Det9_Strip3", "calib_Det9_Strip3", 500, 0, 4000,500,0,4000);
	TH2D* calib_Det9_Strip4 = new TH2D("calib_Det9_Strip4", "calib_Det9_Strip4", 500, 0, 4000,500,0,4000);

	TH2D* calib_Det10_Strip1 = new TH2D("calib_Det10_Strip1", "calib_Det10_Strip1", 500, 0, 4000,500,0,4000);
	TH2D* calib_Det10_Strip2 = new TH2D("calib_Det10_Strip2", "calib_Det10_Strip2", 500, 0, 4000,500,0,4000);
	TH2D* calib_Det10_Strip3 = new TH2D("calib_Det10_Strip3", "calib_Det10_Strip3", 500, 0, 4000,500,0,4000);
	TH2D* calib_Det10_Strip4 = new TH2D("calib_Det10_Strip4", "calib_Det10_Strip4", 500, 0, 4000,500,0,4000);

	TH2D* calib_Det11_Strip1 = new TH2D("calib_Det11_Strip1", "calib_Det11_Strip1", 500, 0, 4000,500,0,4000);
	TH2D* calib_Det11_Strip2 = new TH2D("calib_Det11_Strip2", "calib_Det11_Strip2", 500, 0, 4000,500,0,4000);
	TH2D* calib_Det11_Strip3 = new TH2D("calib_Det11_Strip3", "calib_Det11_Strip3", 500, 0, 4000,500,0,4000);
	TH2D* calib_Det11_Strip4 = new TH2D("calib_Det11_Strip4", "calib_Det11_Strip4", 500, 0, 4000,500,0,4000);

	TH2D* calib_Det12_Strip1 = new TH2D("calib_Det12_Strip1", "calib_Det12_Strip1", 500, 0, 4000,500,0,4000);
	TH2D* calib_Det12_Strip2 = new TH2D("calib_Det12_Strip2", "calib_Det12_Strip2", 500, 0, 4000,500,0,4000);
	TH2D* calib_Det12_Strip3 = new TH2D("calib_Det12_Strip3", "calib_Det12_Strip3", 500, 0, 4000,500,0,4000);
	TH2D* calib_Det12_Strip4 = new TH2D("calib_Det12_Strip4", "calib_Det12_Strip4", 500, 0, 4000,500,0,4000);


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
   SX3_prots_down->SetPoint(0,0.859417,10.4571);
   SX3_prots_down->SetPoint(1,0.859417,8.87127);
   SX3_prots_down->SetPoint(2,4.44819,6.16604);
   SX3_prots_down->SetPoint(3,7.67269,4.48694);
   SX3_prots_down->SetPoint(4,10.425,3.66604);
   SX3_prots_down->SetPoint(5,11.9765,3.34888);
   SX3_prots_down->SetPoint(6,14.2431,3.42351);
   SX3_prots_down->SetPoint(7,14.0542,4.95336);
   SX3_prots_down->SetPoint(8,10.6543,5.08396);
   SX3_prots_down->SetPoint(9,7.76713,5.94216);
   SX3_prots_down->SetPoint(10,4.82596,7.73321);
   SX3_prots_down->SetPoint(11,2.3435,9.61754);
   SX3_prots_down->SetPoint(12,0.994333,10.4944);
   SX3_prots_down->SetPoint(13,0.859417,10.4571);





	TFile write("fine_cal_hists_UD_res_strips.root", "recreate");

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

						if(detID_DownB==1) calib_Det1_Strip1->Fill(E_front_en_DownB.at(j),E_front_en_DownB.at(h));
						if(detID_DownB==2) calib_Det2_Strip1->Fill(E_front_en_DownB.at(j),E_front_en_DownB.at(h));
						if(detID_DownB==3) calib_Det3_Strip1->Fill(E_front_en_DownB.at(j),E_front_en_DownB.at(h));
						if(detID_DownB==4) calib_Det4_Strip1->Fill(E_front_en_DownB.at(j),E_front_en_DownB.at(h));
						if(detID_DownB==5) calib_Det5_Strip1->Fill(E_front_en_DownB.at(j),E_front_en_DownB.at(h));
						if(detID_DownB==6) calib_Det6_Strip1->Fill(E_front_en_DownB.at(j),E_front_en_DownB.at(h));
						if(detID_DownB==7) calib_Det7_Strip1->Fill(E_front_en_DownB.at(j),E_front_en_DownB.at(h));
						if(detID_DownB==8) calib_Det8_Strip1->Fill(E_front_en_DownB.at(j),E_front_en_DownB.at(h));
						if(detID_DownB==9) calib_Det9_Strip1->Fill(E_front_en_DownB.at(j),E_front_en_DownB.at(h));
						if(detID_DownB==10) calib_Det10_Strip1->Fill(E_front_en_DownB.at(j),E_front_en_DownB.at(h));
						if(detID_DownB==11) calib_Det11_Strip1->Fill(E_front_en_DownB.at(j),E_front_en_DownB.at(h));
						if(detID_DownB==12) calib_Det12_Strip1->Fill(E_front_en_DownB.at(j),E_front_en_DownB.at(h));

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

						if(detID_DownB==1) calib_Det1_Strip1->Fill(E_front_en_DownB.at(h),E_front_en_DownB.at(j));
						if(detID_DownB==2) calib_Det2_Strip1->Fill(E_front_en_DownB.at(h),E_front_en_DownB.at(j));
						if(detID_DownB==3) calib_Det3_Strip1->Fill(E_front_en_DownB.at(h),E_front_en_DownB.at(j));
						if(detID_DownB==4) calib_Det4_Strip1->Fill(E_front_en_DownB.at(h),E_front_en_DownB.at(j));
						if(detID_DownB==5) calib_Det5_Strip1->Fill(E_front_en_DownB.at(h),E_front_en_DownB.at(j));
						if(detID_DownB==6) calib_Det6_Strip1->Fill(E_front_en_DownB.at(h),E_front_en_DownB.at(j));
						if(detID_DownB==7) calib_Det7_Strip1->Fill(E_front_en_DownB.at(h),E_front_en_DownB.at(j));
						if(detID_DownB==8) calib_Det8_Strip1->Fill(E_front_en_DownB.at(h),E_front_en_DownB.at(j));
						if(detID_DownB==9) calib_Det9_Strip1->Fill(E_front_en_DownB.at(h),E_front_en_DownB.at(j));
						if(detID_DownB==10) calib_Det10_Strip1->Fill(E_front_en_DownB.at(h),E_front_en_DownB.at(j));
						if(detID_DownB==11) calib_Det11_Strip1->Fill(E_front_en_DownB.at(h),E_front_en_DownB.at(j));
						if(detID_DownB==12) calib_Det12_Strip1->Fill(E_front_en_DownB.at(h),E_front_en_DownB.at(j));

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

						if(detID_DownB==1) calib_Det1_Strip2->Fill(E_front_en_DownB.at(j),E_front_en_DownB.at(h));
						if(detID_DownB==2) calib_Det2_Strip2->Fill(E_front_en_DownB.at(j),E_front_en_DownB.at(h));
						if(detID_DownB==3) calib_Det3_Strip2->Fill(E_front_en_DownB.at(j),E_front_en_DownB.at(h));
						if(detID_DownB==4) calib_Det4_Strip2->Fill(E_front_en_DownB.at(j),E_front_en_DownB.at(h));
						if(detID_DownB==5) calib_Det5_Strip2->Fill(E_front_en_DownB.at(j),E_front_en_DownB.at(h));
						if(detID_DownB==6) calib_Det6_Strip2->Fill(E_front_en_DownB.at(j),E_front_en_DownB.at(h));
						if(detID_DownB==7) calib_Det7_Strip2->Fill(E_front_en_DownB.at(j),E_front_en_DownB.at(h));
						if(detID_DownB==8) calib_Det8_Strip2->Fill(E_front_en_DownB.at(j),E_front_en_DownB.at(h));
						if(detID_DownB==9) calib_Det9_Strip2->Fill(E_front_en_DownB.at(j),E_front_en_DownB.at(h));
						if(detID_DownB==10) calib_Det10_Strip2->Fill(E_front_en_DownB.at(j),E_front_en_DownB.at(h));
						if(detID_DownB==11) calib_Det11_Strip2->Fill(E_front_en_DownB.at(j),E_front_en_DownB.at(h));
						if(detID_DownB==12) calib_Det12_Strip2->Fill(E_front_en_DownB.at(j),E_front_en_DownB.at(h));

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

						if(detID_DownB==1) calib_Det1_Strip2->Fill(E_front_en_DownB.at(h),E_front_en_DownB.at(j));
						if(detID_DownB==2) calib_Det2_Strip2->Fill(E_front_en_DownB.at(h),E_front_en_DownB.at(j));
						if(detID_DownB==3) calib_Det3_Strip2->Fill(E_front_en_DownB.at(h),E_front_en_DownB.at(j));
						if(detID_DownB==4) calib_Det4_Strip2->Fill(E_front_en_DownB.at(h),E_front_en_DownB.at(j));
						if(detID_DownB==5) calib_Det5_Strip2->Fill(E_front_en_DownB.at(h),E_front_en_DownB.at(j));
						if(detID_DownB==6) calib_Det6_Strip2->Fill(E_front_en_DownB.at(h),E_front_en_DownB.at(j));
						if(detID_DownB==7) calib_Det7_Strip2->Fill(E_front_en_DownB.at(h),E_front_en_DownB.at(j));
						if(detID_DownB==8) calib_Det8_Strip2->Fill(E_front_en_DownB.at(h),E_front_en_DownB.at(j));
						if(detID_DownB==9) calib_Det9_Strip2->Fill(E_front_en_DownB.at(h),E_front_en_DownB.at(j));
						if(detID_DownB==10) calib_Det10_Strip2->Fill(E_front_en_DownB.at(h),E_front_en_DownB.at(j));
						if(detID_DownB==11) calib_Det11_Strip2->Fill(E_front_en_DownB.at(h),E_front_en_DownB.at(j));
						if(detID_DownB==12) calib_Det12_Strip2->Fill(E_front_en_DownB.at(h),E_front_en_DownB.at(j));

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

						if(detID_DownB==1) calib_Det1_Strip3->Fill(E_front_en_DownB.at(j),E_front_en_DownB.at(h));
						if(detID_DownB==2) calib_Det2_Strip3->Fill(E_front_en_DownB.at(j),E_front_en_DownB.at(h));
						if(detID_DownB==3) calib_Det3_Strip3->Fill(E_front_en_DownB.at(j),E_front_en_DownB.at(h));
						if(detID_DownB==4) calib_Det4_Strip3->Fill(E_front_en_DownB.at(j),E_front_en_DownB.at(h));
						if(detID_DownB==5) calib_Det5_Strip3->Fill(E_front_en_DownB.at(j),E_front_en_DownB.at(h));
						if(detID_DownB==6) calib_Det6_Strip3->Fill(E_front_en_DownB.at(j),E_front_en_DownB.at(h));
						if(detID_DownB==7) calib_Det7_Strip3->Fill(E_front_en_DownB.at(j),E_front_en_DownB.at(h));
						if(detID_DownB==8) calib_Det8_Strip3->Fill(E_front_en_DownB.at(j),E_front_en_DownB.at(h));
						if(detID_DownB==9) calib_Det9_Strip3->Fill(E_front_en_DownB.at(j),E_front_en_DownB.at(h));
						if(detID_DownB==10) calib_Det10_Strip3->Fill(E_front_en_DownB.at(j),E_front_en_DownB.at(h));
						if(detID_DownB==11) calib_Det11_Strip3->Fill(E_front_en_DownB.at(j),E_front_en_DownB.at(h));
						if(detID_DownB==12) calib_Det12_Strip3->Fill(E_front_en_DownB.at(j),E_front_en_DownB.at(h));

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

						if(detID_DownB==1) calib_Det1_Strip3->Fill(E_front_en_DownB.at(h),E_front_en_DownB.at(j));
						if(detID_DownB==2) calib_Det2_Strip3->Fill(E_front_en_DownB.at(h),E_front_en_DownB.at(j));
						if(detID_DownB==3) calib_Det3_Strip3->Fill(E_front_en_DownB.at(h),E_front_en_DownB.at(j));
						if(detID_DownB==4) calib_Det4_Strip3->Fill(E_front_en_DownB.at(h),E_front_en_DownB.at(j));
						if(detID_DownB==5) calib_Det5_Strip3->Fill(E_front_en_DownB.at(h),E_front_en_DownB.at(j));
						if(detID_DownB==6) calib_Det6_Strip3->Fill(E_front_en_DownB.at(h),E_front_en_DownB.at(j));
						if(detID_DownB==7) calib_Det7_Strip3->Fill(E_front_en_DownB.at(h),E_front_en_DownB.at(j));
						if(detID_DownB==8) calib_Det8_Strip3->Fill(E_front_en_DownB.at(h),E_front_en_DownB.at(j));
						if(detID_DownB==9) calib_Det9_Strip3->Fill(E_front_en_DownB.at(h),E_front_en_DownB.at(j));
						if(detID_DownB==10) calib_Det10_Strip3->Fill(E_front_en_DownB.at(h),E_front_en_DownB.at(j));
						if(detID_DownB==11) calib_Det11_Strip3->Fill(E_front_en_DownB.at(h),E_front_en_DownB.at(j));
						if(detID_DownB==12) calib_Det12_Strip3->Fill(E_front_en_DownB.at(h),E_front_en_DownB.at(j));

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

						if(detID_DownB==1) calib_Det1_Strip4->Fill(E_front_en_DownB.at(j),E_front_en_DownB.at(h));
						if(detID_DownB==2) calib_Det2_Strip4->Fill(E_front_en_DownB.at(j),E_front_en_DownB.at(h));
						if(detID_DownB==3) calib_Det3_Strip4->Fill(E_front_en_DownB.at(j),E_front_en_DownB.at(h));
						if(detID_DownB==4) calib_Det4_Strip4->Fill(E_front_en_DownB.at(j),E_front_en_DownB.at(h));
						if(detID_DownB==5) calib_Det5_Strip4->Fill(E_front_en_DownB.at(j),E_front_en_DownB.at(h));
						if(detID_DownB==6) calib_Det6_Strip4->Fill(E_front_en_DownB.at(j),E_front_en_DownB.at(h));
						if(detID_DownB==7) calib_Det7_Strip4->Fill(E_front_en_DownB.at(j),E_front_en_DownB.at(h));
						if(detID_DownB==8) calib_Det8_Strip4->Fill(E_front_en_DownB.at(j),E_front_en_DownB.at(h));
						if(detID_DownB==9) calib_Det9_Strip4->Fill(E_front_en_DownB.at(j),E_front_en_DownB.at(h));
						if(detID_DownB==10) calib_Det10_Strip4->Fill(E_front_en_DownB.at(j),E_front_en_DownB.at(h));
						if(detID_DownB==11) calib_Det11_Strip4->Fill(E_front_en_DownB.at(j),E_front_en_DownB.at(h));
						if(detID_DownB==12) calib_Det12_Strip4->Fill(E_front_en_DownB.at(j),E_front_en_DownB.at(h));

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

						if(detID_DownB==1) calib_Det1_Strip4->Fill(E_front_en_DownB.at(h),E_front_en_DownB.at(j));
						if(detID_DownB==2) calib_Det2_Strip4->Fill(E_front_en_DownB.at(h),E_front_en_DownB.at(j));
						if(detID_DownB==3) calib_Det3_Strip4->Fill(E_front_en_DownB.at(h),E_front_en_DownB.at(j));
						if(detID_DownB==4) calib_Det4_Strip4->Fill(E_front_en_DownB.at(h),E_front_en_DownB.at(j));
						if(detID_DownB==5) calib_Det5_Strip4->Fill(E_front_en_DownB.at(h),E_front_en_DownB.at(j));
						if(detID_DownB==6) calib_Det6_Strip4->Fill(E_front_en_DownB.at(h),E_front_en_DownB.at(j));
						if(detID_DownB==7) calib_Det7_Strip4->Fill(E_front_en_DownB.at(h),E_front_en_DownB.at(j));
						if(detID_DownB==8) calib_Det8_Strip4->Fill(E_front_en_DownB.at(h),E_front_en_DownB.at(j));
						if(detID_DownB==9) calib_Det9_Strip4->Fill(E_front_en_DownB.at(h),E_front_en_DownB.at(j));
						if(detID_DownB==10) calib_Det10_Strip4->Fill(E_front_en_DownB.at(h),E_front_en_DownB.at(j));
						if(detID_DownB==11) calib_Det11_Strip4->Fill(E_front_en_DownB.at(h),E_front_en_DownB.at(j));
						if(detID_DownB==12) calib_Det12_Strip4->Fill(E_front_en_DownB.at(h),E_front_en_DownB.at(j));

						break;
					}
				}
			}
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
			}

			//dE strips
			for(int k=0;k<dE_strip_SIDAR.size();k++)
			{
				cal_slope = calib_dE_SIDAR[detID_SIDAR][dE_strip_SIDAR.at(k)][0];
				cal_offset = calib_dE_SIDAR[detID_SIDAR][dE_strip_SIDAR.at(k)][1];

				energy = dE_energy_SIDAR.at(k)*cal_slope + cal_offset;

				cal_dE_energy_SIDAR.push_back(energy);			
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



	calib_Det1_Strip1->Write();
	calib_Det1_Strip2->Write();
	calib_Det1_Strip3->Write();
	calib_Det1_Strip4->Write();
	calib_Det2_Strip1->Write();
	calib_Det2_Strip2->Write();
	calib_Det2_Strip3->Write();
	calib_Det2_Strip4->Write();
	calib_Det3_Strip1->Write();
	calib_Det3_Strip2->Write();
	calib_Det3_Strip3->Write();
	calib_Det3_Strip4->Write();
	calib_Det4_Strip1->Write();
	calib_Det4_Strip2->Write();
	calib_Det4_Strip3->Write();
	calib_Det4_Strip4->Write();
	calib_Det5_Strip1->Write();
	calib_Det5_Strip2->Write();
	calib_Det5_Strip3->Write();
	calib_Det5_Strip4->Write();
	calib_Det6_Strip1->Write();
	calib_Det6_Strip2->Write();
	calib_Det6_Strip3->Write();
	calib_Det6_Strip4->Write();
	calib_Det7_Strip1->Write();
	calib_Det7_Strip2->Write();
	calib_Det7_Strip3->Write();
	calib_Det7_Strip4->Write();
	calib_Det8_Strip1->Write();
	calib_Det8_Strip2->Write();
	calib_Det8_Strip3->Write();
	calib_Det8_Strip4->Write();
	calib_Det9_Strip1->Write();
	calib_Det9_Strip2->Write();
	calib_Det9_Strip3->Write();
	calib_Det9_Strip4->Write();
	calib_Det10_Strip1->Write();
	calib_Det10_Strip2->Write();
	calib_Det10_Strip3->Write();
	calib_Det10_Strip4->Write();
	calib_Det11_Strip1->Write();
	calib_Det11_Strip2->Write();
	calib_Det11_Strip3->Write();
	calib_Det11_Strip4->Write();
	calib_Det12_Strip1->Write();
	calib_Det12_Strip2->Write();
	calib_Det12_Strip3->Write();
	calib_Det12_Strip4->Write();


	std::cout << endl;
	std::cout << "~~fin~~" << endl;

	return;

}




























