#ifndef PROCESSOR_H
#define PROCESSOR_H

using namespace std;

#include "Processor/Cache.h"
#include "Processor/Instruction.h"
#include "Protocol.h"
#include "Bus.h"
#include "Protocols/MESI.h"
#include "Protocols/DRAGON.h"
#include "Protocols/BASIC.h"

#include <string>
#include <vector>
#include <list>

namespace CS4223{
	namespace Processor{
			class Core{
				private:
					unsigned short _proc_id;
					vector<Instruction*> *_instructions;
					unsigned int _instruction_count;
					unsigned int _instruction_ref_count;
					unsigned int _data_ref_count;
					unsigned int _cur_instruction_idx;
					double _cache_miss_ratio;
					Bus *const _sharedBus;
					Cache *_L1_cache;
					Protocol *_protocol;
					unsigned int _processor_cycle;
					unsigned int _wait_cycle;
					bool last_instruction();
				public:
					//Constructor & Destructor
					Core(vector<Instruction*> *instructions,
						 const unsigned short proc_id,
						 const unsigned int instruction_count,
						 Bus * const sharedBus,
						 Protocol::Type protocol_type,
						 unsigned int cache_size,
						 unsigned short assoc, 
						 unsigned int blk_size);
					~Core();
					//Public Members
					enum State{
						ready,			//Ready to execute instructions
						executing,		//Executing instructions
						complete,		//All instructions have been executed
						cleaned_up		//Clean up and calculate metric
					} _state;
					//Getter & Setter 
					unsigned int get_instr_count();
					unsigned int get_data_count();
					Core::State get_state();
					//Operations
					bool initialise();
					void next_instr();
					void clean_up();
					Instruction* fetch_instr(unsigned int instruction_idx);
					void decode_instr(Instruction *instr,
						              unsigned short &label,
									  string &value);
					void read_from_addr(string address);
					void write_to_addr(string address);
					void listen(const unsigned short proc_id, string address);
					bool wait();
					//Analytics
					double get_cache_miss_ratio();
					double get_total_cache_access();
					double Core::get_total_cache_hit();
					unsigned int get_processor_execution_cycles(); 
			};
	}
}
#endif