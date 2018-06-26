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

#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TChain.h"


void calib_auto()
{
	ofstream outfile;
	outfile.open("UD_res_strip_2nd_slope_offset.txt");



	TFile *f = new TFile("fine_cal_hists_UD_res_strips_E_vs_strip.root", "READ");
	double cal_peak_pos[3] = {5.157, 5.486, 5.805};
	double cal_params[14][5][2];
	double slope, intercept;	
	std::string temp_string, temp_string2, temp_string3, flag, line;
	double offX, offY, slopeGM, slopeEC, LERP, RERP, offX_new, offY_new;
	int strip_no, C_det;


	TH1D *h1[48];
	int j=0;

	for(int det=1;det<13;det++)
	{
		for(int strip=1;strip<5;strip++)
		{

			ostringstream in;
			in << "D" << det << "S" << strip;
			string h = in.str();
			const char* hist = h.c_str();
		
			h1[j] = (TH1D*)f->Get(hist);

			TSpectrum* spec = new TSpectrum(3);

			Int_t nPeaks = spec->Search(h1[j],1, "", 0.1);
			Double_t *raw_peak_pos = spec->GetPositionX();

			double avg_r_pos = (raw_peak_pos[0] + raw_peak_pos[1] + raw_peak_pos[2])/nPeaks;
			double avg_c_pos = (cal_peak_pos[0] + cal_peak_pos[1] + cal_peak_pos[2])/nPeaks;
			double numerator=0, denominator=0; 


			for(int i=0;i<nPeaks;i++)
			{
				numerator += (raw_peak_pos[i] - avg_r_pos)*(cal_peak_pos[i] - avg_c_pos);
				denominator += (raw_peak_pos[i] - avg_r_pos)*(raw_peak_pos[i] - avg_r_pos);
			}

			slope = numerator/denominator;
			intercept = (avg_c_pos - slope*avg_r_pos);
	
			cal_params[det][strip][0] = slope;
			cal_params[det][strip][1] = intercept;


			outfile << hist << " " << slope << " " << intercept << endl;
			j++;
		}
	}

	outfile << " " << endl;
	outfile << " " << endl;

	ifstream SX3_calib_params ("SX3_res_strip_cal_params.txt");

	if(!SX3_calib_params)
	{
		cout << "Cannot open Super X3 calibration file, you doughnut..." << endl;
		return 1;
	}

	//=====================READING IN THE CALIBRATION FILE=========================
	while(getline(SX3_calib_params,line))
	{

		//If the barrel detector is Downstream
		if(!line.empty() && flag == "SuperX3 D")
		{	
			if(line.compare(0,3,"Res") == 0)
			{			
				temp_string2 = line.at(12);
				istringstream(temp_string2) >> strip_no;			

				istringstream in(line);
				in >> temp_string3 >> temp_string3 >> temp_string3 >> offX >> offY >> slopeGM >> slopeEC >> LERP >> RERP;				

				offX_new = offX - (cal_params[C_det][strip_no][1] / (2 * cal_params[C_det][strip_no][0]));
				offY_new = offY - (cal_params[C_det][strip_no][1] / (2 * cal_params[C_det][strip_no][0]));

				outfile <<"Res. Strip #"<< strip_no <<"   "<< offX_new <<"   "<< offY_new <<"   "<< slopeGM <<"   "<< cal_params[C_det][strip_no][0] <<"   "<< LERP <<"   "<< RERP << endl;;

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


















	outfile.close();











	return 0;

}


















