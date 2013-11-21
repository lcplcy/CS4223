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
				CS4223::Processor::Cache *_cache;
				CS4223::Bus *_sharedBus;
				unsigned short _proc_id;
				class State{
					public:
						enum state{
							M,
							E,
							S,
							I
						}_state;
						State(){
							this->_state = state::I;
						}
						void set_state(const State::state new_state){
							this->_state = new_state;
						}
						state get_state(){
							return this->_state;
						}
				};
				vector<vector<State>> *_cache_state;
			public:
				MESI(const unsigned short proc_id, CS4223::Processor::Cache *cache, CS4223::Bus *sharedBus);
				~MESI();
				void ProRd(string address,unsigned int *wait_cycle);
				void ProWr(string address,unsigned int *wait_cycle);
				void Snoop(CS4223::Processor::Transaction incoming_transaction);
		};

	}
}
#endif
