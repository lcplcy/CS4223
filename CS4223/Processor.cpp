#include "Processor.h"

namespace CS4223{
	namespace Processor{
		Core::Core(vector<CS4223::Processor::Instruction*> *instructions,const unsigned short proc_id,const unsigned int instruction_count,Bus * const sharedBus,CS4223::Protocol::Type protocol_type,unsigned int cache_size,unsigned short assoc, unsigned int blk_size)
			:_instructions(instructions),_proc_id(proc_id),_instruction_count(instruction_count),_sharedBus(sharedBus)
		{
			// Create a new processor with cache
			this->_data_ref_count = instructions->size();
			this->_instruction_ref_count = _instruction_count - this->_data_ref_count;

			unsigned short address_in_bytes_for_each_transaction = 4;
			unsigned short data_in_bytes_for_each_transaction;

			this->_L1_cache = new Cache(this->_sharedBus,cache_size,assoc,blk_size);

			if(protocol_type==Protocol::MESI){
				this->_protocol = new CS4223::Protocols::MESI(this->_proc_id,this->_L1_cache,this->_sharedBus);
				this->_sharedBus->set_bytes_per_transaction(4,blk_size,blk_size,blk_size,0);
			}else if(protocol_type==Protocol::MOESI){
				this->_protocol = new CS4223::Protocols::MOESI(this->_proc_id,this->_L1_cache,this->_sharedBus);
				this->_sharedBus->set_bytes_per_transaction(4,blk_size,blk_size,blk_size,2);
			}else if(protocol_type==Protocol::DRAGON){
				this->_protocol = new CS4223::Protocols::DRAGON(this->_proc_id,this->_L1_cache,this->_sharedBus);
				this->_sharedBus->set_bytes_per_transaction(4,blk_size,blk_size,blk_size,2);  //Only one word size per transaction
			}else if(protocol_type==Protocol::NONE){
				this->_protocol = new CS4223::Protocols::BASIC(this->_proc_id,this->_L1_cache,this->_sharedBus);
				this->_sharedBus->set_bytes_per_transaction(4,blk_size,blk_size,blk_size,0);
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
		
			this->_cache_miss_ratio = 0;
			this->_cur_instruction_idx = 0;
			this->_processor_cycle = 0;
			this->_wait_cycle = 0;
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
			//CS4223::Protocol::Type protocol_type;
			//if(protocol_type == Protocol::DRAGON){
			//	while(!this->_protocol->ProRd(address,&this->_wait_cycle)){
					//do_nothing
			//	}
			//}
			this->_protocol->ProRd(address,&this->_wait_cycle);
		}

		void Core::write_to_addr(string address){
			this->_protocol->ProWr(address,&this->_wait_cycle);
		}

		void Core::listen(Transaction incoming_transaction){

			if(incoming_transaction.get_address()==""){
				return;
			}

			this->_protocol->Snoop(incoming_transaction);
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

		double Core::get_cache_miss_ratio(){
			return this->_cache_miss_ratio;
		}

		double Core::get_total_cache_access(){
			return this->_L1_cache->get_total_cache_access();
		}

		double Core::get_total_cache_hit(){
			return this->_L1_cache->get_total_cache_hit();
		}

		unsigned int Core::get_processor_execution_cycles(){
			return this->_processor_cycle;
		}
	}
}