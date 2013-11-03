#include "Processor.h"

namespace CS4223{
	namespace Processor{
		Core::Core(vector<CS4223::Processor::Instruction*> *instructions,const unsigned int instruction_count,Bus * const sharedBus,CS4223::Protocol::Type protocol_type,unsigned int cache_size,unsigned short assoc, unsigned int blk_size)
			:_instructions(instructions),_instruction_count(instruction_count),_sharedBus(sharedBus)
		{
			// Create a new processor with cache
			this->_data_ref_count = instructions->size();
			this->_instruction_ref_count = _instruction_count - this->_data_ref_count;
			this->_cur_instruction_idx = 0;
			this->_processor_cycle = 0;
			this->_cache = new Cache(this->_sharedBus,protocol_type,cache_size,assoc,blk_size);
			this->_protocol_type = protocol_type;
		}

		Core::~Core(){

		}

		unsigned int Core::get_instr_count(){

			return this->_instruction_ref_count;
		}

		unsigned int Core::get_data_count(){
			return this->_data_ref_count;
		}

		Core::State Core::get_state(){
			return this->_state;
		}

		bool Core::initialise(){
			bool initialisation_completed = false;
			
			this->_state = Core::State::ready;

			return initialisation_completed;
		}

		void Core::next_instr(const unsigned int clk){

			//In each clk, only one memory reference
			if(!this->wait(clk)&&this->_state!=Core::State::cleaned_up){

				unsigned short label;
				string value;

				this->_state = Core::State::executing;

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

				if(this->last_instruction()){
					this->_state = Core::State::complete;
				}
			}
		}

		Instruction* Core::fetch_next_instr(){
			unsigned int instr_to_fetch = this->_cur_instruction_idx;

			Instruction *instr_ptr = this->_instructions->at(instr_to_fetch);

			this->_cur_instruction_idx+=1;

			return instr_ptr;
		}

		void Core::decode_instr(Instruction *instr,unsigned short &label,string &value){
			label = instr->GetLabel();
			value = instr->GetValue();
		}

		void Core::read_from_addr(string address){
			//this->_protocol.ProRd(this->_cache,this->_sharedBus,this->_processor_cycle, address);
		}

		void Core::write_to_addr(string address){
			//this->_protocol.ProRd(this->_cache,this->_sharedBus,this->_processor_cycle, address);
		}

		bool Core::wait(const unsigned int clk){
			if(this->_processor_cycle>clk){
				return true;
			}
			else{
				return false;
			}
		}

		bool Core::last_instruction(){
			return this->_cur_instruction_idx == this->_data_ref_count;
		}

		void Core::clean_up(){
			//Add the instruction cycle back to the processor cycle [assume each instruction cycle takes one processor cycle]
			this->_processor_cycle+=this->_instruction_ref_count;

			this->_state = Core::State::cleaned_up;
		}
	}
}