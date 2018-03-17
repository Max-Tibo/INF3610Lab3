///////////////////////////////////////////////////////////////////////////////
//
//	Reader.h
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <systemc.h>
#include "LMBIF.h"

///////////////////////////////////////////////////////////////////////////////
//
//	Class Reader
//
///////////////////////////////////////////////////////////////////////////////
class Reader /* └ complÚter */
{
	public:
		// Ports
		sc_in_clk				clk;
		sc_in<unsigned int>  	address;
		sc_out<unsigned int>  	data;
		sc_in<bool>				request;
		sc_out<bool>			ack;
		sc_port<LMBIF>			dataPortRAM;

		// Constructor
		Reader( sc_module_name name );

		// Destructor
		~Reader();

	private:
		// Process SystemC
		SC_HAS_PROCESS(Reader);
		
		void thread(void);
};

