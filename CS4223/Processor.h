#ifndef PROCESSOR_H
#define PROCESSOR_H

using namespace std;

#include "Processor\Instruction.h"
#include "Bus.h"

#include <string>
#include <vector>

namespace CS4223{
	namespace Processor{
			class Core{
				private:
					vector<CS4223::Processor::Instruction*> _instructions;
					unsigned int _instruction_count;
					unsigned int _instruction_ref_count;
					unsigned int _data_ref_count;
					unsigned int _cur_instruction_idx;
					const Bus * const _sharedBus;
					unsigned int instr_cycle;
				public:
					Core(const vector<CS4223::Processor::Instruction*> instructions,const unsigned int instruction_count,const Bus * const sharedBus);
					~Core();
					bool initialise();
					void next_instr(const unsigned int clk, unsigned int &cycle);
					CS4223::Processor::Instruction* fetch_next_instr();
					void decode_instr(CS4223::Processor::Instruction *instr,unsigned short &label,string &value);
					void read_from_addr(string address);
					void write_to_addr(string address);
					bool last_instruction();
			};
	}
}
#endif