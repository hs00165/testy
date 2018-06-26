#ifndef __SE84dETCLASSES__
#define __SE84dETCLASSES__

#include <iostream>
#include <vector>

#include "UserClassBase.h"

using namespace std;

// ----------------- Generic "Detector" Class ------------------------- //

class Generic_detclass: public LuaUserClass {
	public:
		Generic_detclass()
		{
		}

		int detID = -1;

		vector<int> channels;
		vector<double> values;

		void Reset();
		void MakeAccessors(lua_State* L);
};

// ----------------- SIDAR Class ------------------------- //

class SIDAR_detclass: public LuaUserClass {
	public:
		SIDAR_detclass()
		{
		}

		int detID = -1;

		vector<short> dE_strips;
		vector<float> dE_energies;

		vector<short> E_strips;
		vector<float> E_energies;

		void Reset();

		string to_string() const
		{
			cout << "SIDAR_detclass::to_string => " << endl;

			cout << "dE_strips = ";
			for (unsigned int i = 0; i < dE_strips.size(); i++)
			{
				cout << dE_strips[i] << " ";
			}
			cout << endl;

			cout << "dE_energies = ";
			for (unsigned int i = 0; i < dE_energies.size(); i++)
			{
				cout << dE_energies[i] << " ";
			}
			cout << endl;

			cout << "E_strips = ";
			for (unsigned int i = 0; i < E_strips.size(); i++)
			{
				cout << E_strips[i] << " ";
			}
			cout << endl;

			cout << "E_energies = ";
			for (unsigned int i = 0; i < E_energies.size(); i++)
			{
				cout << E_energies[i] << " ";
			}
			cout << endl;

			char* dump = new char[512];

			sprintf(dump, "SIDAR_detclass %d %d %d %d", (int) dE_strips.size(), (int) dE_energies.size(), (int) E_strips.size(), (int) E_energies.size());

			return (string) dump;
		}

		void MakeAccessors(lua_State* L);
};

// ----------------- Barrel Class ------------------------- //

class Barrel_detclass: public LuaUserClass {
	public:
		Barrel_detclass()
		{
		}

		int detID = -1;

		vector<short> dE_strips;
		vector<float> dE_energies;

		vector<short> E_front_contacts;
		vector<float> E_front_energies;

		vector<short> E_back_strips;
		vector<float> E_back_energies;

		void Reset();
		void MakeAccessors(lua_State* L);
};

// ----------------- Ion Chamber Class ------------------------- //

class IonChamber_detclass: public LuaUserClass {
	public:
		IonChamber_detclass()
		{
		}

		vector<short> pads;
		vector<float> energies;

		float average_energy;

		void Reset();
		void MakeAccessors(lua_State* L);
};

// ----------------- CRDC Class ------------------------- //

class CRDC_detclass: public LuaUserClass {
	public:
		CRDC_detclass()
		{
		}

//		vector<short> pads;
//		vector<vector<short>> sample_nbr;
//		vector<vector<float>> raw;

		float time;
		float average_raw;
		float xgrav;

		void Reset();
		void MakeAccessors(lua_State* L);
};

// ----------------- MTDC Class ------------------------- //

class MTDC_detclass: public LuaUserClass {
	public:
		MTDC_detclass()
		{
		}

		vector<unsigned int> e1up_hits;
		vector<unsigned int> e1down_hits;
		vector<unsigned int> xf_hits;
		vector<unsigned int> rf_hits;

		void Reset();
		void MakeAccessors(lua_State* L);
};

// ----------------- Scintillators Class ------------------------- //

class Scintillators_detclass: public LuaUserClass {
	public:
		Scintillators_detclass()
		{
		}

		vector<float> up;
		vector<float> down;

		void Reset();
		void MakeAccessors(lua_State* L);
};

extern "C" int openlib_se84_detclasses(lua_State* L)
{
	MakeAccessFunctions<Generic_detclass>(L, "Generic_detclass");
	MakeAccessFunctions<SIDAR_detclass>(L, "SIDAR_detclass");
	MakeAccessFunctions<Barrel_detclass>(L, "Barrel_detclass");
	MakeAccessFunctions<IonChamber_detclass>(L, "IonChamber_detclass");
	MakeAccessFunctions<CRDC_detclass>(L, "CRDC_detclass");
	MakeAccessFunctions<MTDC_detclass>(L, "MTDC_detclass");
	MakeAccessFunctions<Scintillators_detclass>(L, "Scintillators_detclass");
	return 0;
}

#ifdef __CINT__

#pragma link C++ class SIDAR_detclass+;
#pragma link C++ class vector<SIDAR_detclass>+;
#pragma link C++ class Barrel_detclass+;
#pragma link C++ class vector<Barrel_detclass>+;
#pragma link C++ class IonChamber_detclass+;
#pragma link C++ class CRDC_detclass+;
#pragma link C++ class MTDC_detclass+;
#pragma link C++ class Scintillators_detclass+;

#endif

#endif
