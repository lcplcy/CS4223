#ifndef BASIC_H
#define BASIC_H

#include "../Protocol.h"
#include "../Processor/Cache.h"
#include "../Processor/Transaction.h"
#include "../Bus.h"

using namespace std;

namespace CS4223{
	namespace Protocols{

		class BASIC : public CS4223::Protocol {
			private:
				CS4223::Processor::Cache *_cache;
				CS4223::Bus *_sharedBus;
			public:
				BASIC(CS4223::Processor::Cache *cache, CS4223::Bus *sharedBus);
				~BASIC();
				bool ProRd(string address,unsigned int *wait_cycle);
				bool ProWr(string address,unsigned int *wait_cycle);
		};

	}
}
#endif