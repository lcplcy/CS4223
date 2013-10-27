#include "Processor.h"

namespace CS4223{
	namespace Processor{
		Core::Core(const vector<CS4223::Processor::Instruction*> instructions,const unsigned int instruction_count,const Bus * const sharedBus)
			:_instructions(instructions),_instruction_count(instruction_count),_sharedBus(sharedBus)
		{
			this->_data_ref_count = instructions.size();
			this->_instruction_ref_count = _instruction_count - this->_data_ref_count;
			this->_cur_instruction_idx = 0;
		}

		Core::~Core(){

		}

		bool Core::initialise(){
			bool initialisation_completed = false;
			
			return initialisation_completed;
		}

		void Core::next_instr(const unsigned int clk, unsigned int &cycle){

			unsigned short label;
			string value;

			CS4223::Processor::Instruction *next_instr = this->fetch_next_instr();

			this->decode_instr(next_instr,label,value);

			switch(label){
				case 2:
					this->read_from_addr(value);
					break;
				case 3:
					this->write_to_addr(value);
					break;
			}
		}

		CS4223::Processor::Instruction* Core::fetch_next_instr(){

			Instruction* instr_ptr = this->_instructions[this->_cur_instruction_idx];

			return instr_ptr;
		}

		void CS4223::Processor::Core::decode_instr(CS4223::Processor::Instruction *instr,unsigned short &label,string &value){
			label = instr->GetLabel();
			value = instr->GetValue();
		}

		void CS4223::Processor::Core::read_from_addr(string address){

		}

		void CS4223::Processor::Core::write_to_addr(string address){

		}

		bool Core::last_instruction(){
			return this->_cur_instruction_idx == this->_instruction_count;
		}
	}
}