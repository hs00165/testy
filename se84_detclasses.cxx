#include "se84_detclasses.h"

// ----------------- Generic "Detector" Class ------------------------- //

void Generic_detclass::Reset()
{
	channels.clear();
	values.clear();
}

void Generic_detclass::MakeAccessors(lua_State* L)
{
	AddAccessor(L, &detID, "detID", "int");
	AddAccessor(L, &channels, "channels", "vector<int>");
	AddAccessor(L, &values, "values", "vector<double>");
}

// ----------------- SIDAR Class ------------------------- //

void SIDAR_detclass::Reset()
{
	dE_strips.clear();
	dE_energies.clear();
	E_strips.clear();
	E_energies.clear();
}

void SIDAR_detclass::MakeAccessors(lua_State* L)
{
	AddAccessor(L, &detID, "detID", "int");
	AddAccessor(L, &dE_strips, "dE_strips", "vector<short>");
	AddAccessor(L, &dE_energies, "dE_energies", "vector<float>");
	AddAccessor(L, &E_strips, "E_strips", "vector<short>");
	AddAccessor(L, &E_energies, "E_energies", "vector<float>");
}

// ----------------- Barrel Class ------------------------- //

void Barrel_detclass::Reset()
{
	dE_strips.clear();
	dE_energies.clear();
	E_front_contacts.clear();
	E_front_energies.clear();
	E_back_strips.clear();
	E_back_energies.clear();
}

void Barrel_detclass::MakeAccessors(lua_State* L)
{
	AddAccessor(L, &detID, "detID", "int");
	AddAccessor(L, &dE_strips, "dE_strips", "vector<short>");
	AddAccessor(L, &dE_energies, "dE_energies", "vector<float>");
	AddAccessor(L, &E_front_contacts, "E_front_contacts", "vector<short>");
	AddAccessor(L, &E_front_energies, "E_front_energies", "vector<float>");
	AddAccessor(L, &E_back_strips, "E_back_strips", "vector<short>");
	AddAccessor(L, &E_back_energies, "E_back_energies", "vector<float>");
}

// ----------------- Ion Chamber Class ------------------------- //

void IonChamber_detclass::Reset()
{
	average_energy = 0;
	pads.clear();
	energies.clear();
}

void IonChamber_detclass::MakeAccessors(lua_State* L)
{
	AddAccessor(L, &pads, "pads", "vector<short>");
	AddAccessor(L, &energies, "energies", "vector<float>");
	AddAccessor(L, &average_energy, "average_energy", "float");
}

// ----------------- CRDC Class ------------------------- //

void CRDC_detclass::Reset()
{
//			pads.clear();
//			sample_nbr.clear();
//			raw.clear();
	time = 0;
	xgrav = 0;
	average_raw = 0;
}

void CRDC_detclass::MakeAccessors(lua_State* L)
{
//			AddAccessor(L, &pads, "pads", "vector<short>");
//			AddAccessor(L, &sample_nbr, "sample_nbr", "vector<vector<short>>");
//			AddAccessor(L, &raw, "raw", "vector<vector<float>>");
	AddAccessor(L, &time, "time", "float");
	AddAccessor(L, &average_raw, "average_raw", "float");
	AddAccessor(L, &xgrav, "xgrav", "float");
}

// ----------------- MTDC Class ------------------------- //

void MTDC_detclass::Reset()
{
	e1up_hits.clear();
	e1down_hits.clear();
	xf_hits.clear();
	rf_hits.clear();
}

void MTDC_detclass::MakeAccessors(lua_State* L)
{
	AddAccessor(L, &xf_hits, "xf_hits", "vector<unsigned int>");
	AddAccessor(L, &e1up_hits, "e1up_hits", "vector<unsigned int>");
	AddAccessor(L, &e1down_hits, "e1down_hits", "vector<unsigned int>");
	AddAccessor(L, &rf_hits, "rf_hits", "vector<unsigned int>");
}

// ----------------- Scintillators Class ------------------------- //

void Scintillators_detclass::Reset()
{
	up.clear();
	down.clear();
}

void Scintillators_detclass::MakeAccessors(lua_State* L)
{
	AddAccessor(L, &up, "up", "vector<float>");
	AddAccessor(L, &down, "down", "vector<float>");
}
