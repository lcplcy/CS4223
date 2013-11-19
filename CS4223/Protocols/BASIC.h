#ifndef BASIC_H
#define BASIC_H

#include "../Protocol.h"
#include "../Processor/Cache.h"
#include "../Processor/Transaction.h"
#include "../Processor/Block.h"
#include "../Bus.h"

#include <vector>

using namespace std;

namespace CS4223{
	namespace Protocols{

		class BASIC : public CS4223::Protocol {
			private:
				CS4223::Processor::Cache *_cache;
				CS4223::Bus *_sharedBus;
				unsigned short _proc_id;
				class State{
					private:
						bool Valid;
						bool Dirty;
					public:
						State(){
							this->Valid = false;
							this->Dirty = false;
						}
						void set_valid(bool valid){
							this->Valid=valid;
						}
						bool get_valid(){
							return this->Valid;
						}
						void set_dirty(bool dirty){
							this->Dirty = dirty;
						}
						bool get_dirty(){
							return this->Dirty;
						}
				};

				vector<vector<State>> *_cache_state;
			public:
				BASIC(const unsigned short proc_id,CS4223::Processor::Cache *cache, CS4223::Bus *sharedBus);
				~BASIC();
				void ProRd(string address,unsigned int *wait_cycle);
				void ProWr(string address,unsigned int *wait_cycle);
				void Snoop(CS4223::Processor::Transaction incoming_transaction);
		};

	}
}
#endif