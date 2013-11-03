#include "Processor.h"

namespace CS4223{
	namespace Processor{
		Core::Core(vector<CS4223::Processor::Instruction*> *instructions,const unsigned int instruction_count,Bus * const sharedBus,CS4223::Protocol::Type protocol_type,unsigned int cache_size,unsigned short assoc, unsigned int blk_size)
			:_instructions(instructions),_instruction_count(instruction_count),_sharedBus(sharedBus)
		{
			// Create a new processor with cache
			this->_cache_miss_ratio = 0;
			this->_data_ref_count = instructions->size();
			this->_instruction_ref_count = _instruction_count - this->_data_ref_count;
			this->_cur_instruction_idx = 0;
			this->_processor_cycle = 0;
			this->_wait_cycle = 0;
			this->_L1_cache = new Cache(this->_sharedBus,cache_size,assoc,blk_size);

			if(protocol_type==Protocol::MESI){
				this->_protocol = new CS4223::Protocols::MESI();
			}else if(protocol_type==Protocol::DRAGON){
				this->_protocol = new CS4223::Protocols::DRAGON();
			}else if(protocol_type==Protocol::NONE){
				this->_protocol =new CS4223::Protocols::BASIC();
			}
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

		void Core::next_instr(){
			
			unsigned short label;
			string value;

			//In each clk, only one memory reference
			if(this->_state!=Core::State::cleaned_up){

				this->_state = Core::State::executing;

				CS4223::Processor::Instruction *instr_ptr = this->fetch_instr(this->_cur_instruction_idx);

				this->decode_instr(instr_ptr,label,value);

				switch(label){
					case 0:
						//Execute instructions no waits
						this->_cur_instruction_idx+=1;
						break;
					//No memory reference if cache miss penalty
					case 2:
						if(!this->wait()){
							this->read_from_addr(value);
							this->_cur_instruction_idx+=1;
						}
						break;
					case 3:
						if(!this->wait()){
							this->write_to_addr(value);	
							this->_cur_instruction_idx+=1;
						}
						break;
				}

				//Always inc proc cycle
				this->_processor_cycle+=1;

				if(this->last_instruction()){
					this->_state = Core::State::complete;
				}
			}
		}

		Instruction* Core::fetch_instr(unsigned int instruction_idx){

			Instruction *instr_ptr = this->_instructions->at(instruction_idx);

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

		bool Core::wait(){
			if(this->_wait_cycle>0){
				this->_wait_cycle-=1;
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

			this->_cache_miss_ratio = this->_L1_cache->get_miss_ratio();

			this->_state = Core::State::cleaned_up;
		}
	}
}