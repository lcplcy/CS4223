#ifndef PROCESSOR_H
#define PROCESSOR_H

using namespace std;

#include "Processor\Cache.h"
#include "Processor\Instruction.h"
#include "Protocol.h"
#include "Bus.h"

#include <string>
#include <vector>
#include <list>

namespace CS4223{
	namespace Processor{
			class Core{
				private:
					vector<Instruction*> *_instructions;
					unsigned int _instruction_count;
					unsigned int _instruction_ref_count;
					unsigned int _data_ref_count;
					unsigned int _cur_instruction_idx;
					Bus *const _sharedBus;
					Cache *_cache;
					Protocol::Type _protocol_type;
					unsigned int _processor_cycle;
					bool last_instruction();
				public:
					//Constructor & Destructor
					Core(vector<Instruction*> *instructions,
						 const unsigned int instruction_count,
						 Bus * const sharedBus,
						 Protocol::Type protocol_type,
						 unsigned int cache_size,
						 unsigned short assoc, 
						 unsigned int blk_size);
					~Core();
					//Public Members
					enum State{
						ready,
						executing,
						complete,
						cleaned_up
					} _state;
					//Getter & Setter 
					unsigned int get_instr_count();
					unsigned int get_data_count();
					Core::State get_state();
					//Operations
					bool initialise();
					void next_instr(const unsigned int clk);
					void clean_up();
					Instruction* fetch_next_instr();
					void decode_instr(Instruction *instr,
						              unsigned short &label,
									  string &value);
					void read_from_addr(string address);
					void write_to_addr(string address);
					bool wait(const unsigned int clk);
					
			};
	}
}
#endif