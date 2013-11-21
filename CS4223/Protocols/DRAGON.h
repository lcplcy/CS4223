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
				CS4223::Processor::Cache *_cache;
				CS4223::Bus *_sharedBus;
				unsigned short _proc_id;
				class State{
					private:
						string Dragon_State;
					public:
						State(){
							this->Dragon_State;
						}
						void set_dragonstate(string state){
							this->Dragon_State=state;
						}
						string get_dragonstate(){
							return this->Dragon_State;
						}
				};

				vector<vector<State>> *_cache_state;
			public:
				string prev_step;
				DRAGON(const unsigned short proc_id,CS4223::Processor::Cache *cache, CS4223::Bus *sharedBus);
				~DRAGON();
				void ProRd(string address,unsigned int *wait_cycle);
				void ProWr(string address,unsigned int *wait_cycle);
				void listen(string address,unsigned int *wait_cycle);
		};

	}
}
#endif