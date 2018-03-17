///////////////////////////////////////////////////////////////////////////////
//
//	main.cpp
//
///////////////////////////////////////////////////////////////////////////////
#include <systemc.h>
#include "Sobel.h"
#include "Sobelv2.h"
#include "Reader.h"
#include "DataRAM.h"
#include "CacheMem.h"

#define RAMSIZE 0x200000

///////////////////////////////////////////////////////////////////////////////
//
//	Main
//
///////////////////////////////////////////////////////////////////////////////
int sc_main(int arg_count, char **arg_value)
{
	// Variables
	int sim_units = 2; //SC_NS

	// Clock
	const sc_core::sc_time_unit simTimeUnit = SC_NS;
	const int clk_freq = 4000;
	sc_clock clk("SysClock", clk_freq, simTimeUnit, 0.5);

	// Components
	//TODO : D�claration des modules

	// Signals
	sc_signal<unsigned int, SC_MANY_WRITERS> data;
	sc_signal<unsigned int, SC_MANY_WRITERS> address;
	sc_signal<unsigned int*> addressData;
	sc_signal<unsigned int> length;
	sc_signal<bool, SC_MANY_WRITERS> reqRead;
	sc_signal<bool, SC_MANY_WRITERS> ackReaderWriter;
	// ...

	// Connexions
	// TODO: Ajouter connexions 

	const bool utiliseCacheMem = false;

	if (!utiliseCacheMem) {
		Sobel sobel("Sobel");

		/* � compl�ter (connexion ports)*/

		// D�marrage de l'application
		cout << "D�marrage de la simulation." << endl;
		sc_start(-1, sc_core::sc_time_unit(sim_units));
		cout << endl << "Simulation s'est termin�e � " << sc_time_stamp();
	} else {
		Sobelv2 sobel("Sobel");
		CacheMem cacheMem("CacheMem");

		/* � compl�ter (connexion ports)*/

		// D�marrage de l'application
		cout << "D�marrage de la simulation." << endl;
		sc_start(-1, sc_core::sc_time_unit(sim_units));
		cout << endl << "Simulation s'est termin�e � " << sc_time_stamp();

	}

	return 0;
}
