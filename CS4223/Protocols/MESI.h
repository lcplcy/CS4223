#ifndef MESI_H
#define MESI_H

#include "..\Protocol.h"

using namespace std;

namespace CS4223{
	namespace Protocols{

		class MESI : public CS4223::Protocol {
			private:
			public:
				MESI();
				~MESI();
				bool ProRd(string address,unsigned int *wait_cycle);
				bool ProWr(string address,unsigned int *wait_cycle);
				/*
					bool prord(Cache *cache, Bus *bus, Cycle *proc_cycle);
				*/
		};

	}
}
#endif
