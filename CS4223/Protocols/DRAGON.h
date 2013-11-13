#ifndef DRAGON_H
#define DRAGON_H

#include "../Protocol.h"
#include "../Processor/Cache.h"
#include "../Processor/Transaction.h"
#include "../Bus.h"

using namespace std;

namespace CS4223{
	namespace Protocols{

		class DRAGON : public CS4223::Protocol {
			private:
			public:
				DRAGON(CS4223::Processor::Cache *cache, CS4223::Bus *sharedBus);
				~DRAGON();
				void ProRd(string address,unsigned int *wait_cycle);
				void ProWr(string address,unsigned int *wait_cycle);
		};

	}
}
#endif