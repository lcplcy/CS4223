#ifndef MESI_H
#define MESI_H

#include "../Protocol.h"
#include "../Processor/Cache.h"
#include "../Processor/Transaction.h"
#include "../Bus.h"

using namespace std;

namespace CS4223{
	namespace Protocols{

		class MESI : public CS4223::Protocol {
			private:
				enum State{
					M,
					E,
					S,
					I
				};
				CS4223::Processor::Cache *_cache;
				CS4223::Bus *_sharedBus;
				unsigned short _proc_id;
			public:
				MESI(const unsigned short proc_id, CS4223::Processor::Cache *cache, CS4223::Bus *sharedBus);
				~MESI();
				void ProRd(string address,unsigned int *wait_cycle);
				void ProWr(string address,unsigned int *wait_cycle);
				void Snoop(const unsigned short proc_id, string address);
		};

	}
}
#endif
