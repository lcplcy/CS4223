#include "BASIC.h"

namespace CS4223{
	namespace Protocols{

		BASIC::BASIC(const unsigned short proc_id, CS4223::Processor::Cache *cache, CS4223::Bus *sharedBus)
			:_proc_id(proc_id),_cache(cache),_sharedBus(sharedBus)
		{
			vector<State> cache_set(this->_cache->get_associativity(),State());
			this->_cache_state = new vector<vector<State>>(this->_cache->get_num_of_cache_sets(),cache_set);
		}

		BASIC::~BASIC(){

		} 

		void BASIC::ProRd(string address,unsigned int *wait_cycle){

			bool miss = true;

			this->_cache->inc_cache_access();

			//Extract block identifier
			Processor::Cache::Address translated_address = this->_cache->translate_address(address);
			vector<Processor::Block> *cacheSet = this->_cache->get_cache_set((unsigned int)translated_address.cache_set_idx);
			vector<State> *cacheStateSet = &this->_cache_state->at((unsigned int)translated_address.cache_set_idx);

			//Cache will return hit or miss
			for(unsigned int sets=0;sets<cacheSet->size();sets++){
				
				if(translated_address.tag==cacheSet->at(sets).get_tag()&&cacheStateSet->at(sets).get_valid()==true){
					//A Hit
					miss=false;
					break;
				}
			}
			
			//Cache Hit
			if(!miss){
				this->_cache->inc_hit();
			}else{
				//Cache Miss
				//Miss => access memory add 10 to processor cycle
				*wait_cycle+=10;

				//Fetch Cache Block from Memory
				CS4223::Processor::Transaction new_transaction(this->_proc_id,CS4223::Processor::Transaction::BusRd,address);
				this->_sharedBus->add_transaction(Bus::Type::BusRd,new_transaction);

				//Random Replacement at the cache set
				unsigned int random_blk_idx = rand()%cacheSet->size()+0;
				Processor::Block *selectedBlk = &cacheSet->at(random_blk_idx);
				BASIC::State *selectedState = &cacheStateSet->at(random_blk_idx);

				if(selectedState->get_dirty()){
					Processor::Transaction new_transaction(this->_proc_id,CS4223::Processor::Transaction::BusWr,address);
					this->_sharedBus->add_transaction(Bus::Type::BusWr,new_transaction);
				}

				selectedBlk->set_tag(translated_address.tag);
				selectedState->set_dirty(false);
				selectedState->set_valid(true);
			}
		}

		void BASIC::ProWr(string address,unsigned int *wait_cycle){

			bool miss=true;

			this->_cache->inc_cache_access();

			//Extract block identifier
			Processor::Cache::Address translated_address = this->_cache->translate_address(address);
			vector<Processor::Block> *cacheSet = this->_cache->get_cache_set((unsigned int)translated_address.cache_set_idx);
			vector<State> *cacheStateSet = &this->_cache_state->at((unsigned int)translated_address.cache_set_idx);

			for(unsigned int sets=0;sets<cacheSet->size();sets++){

				Processor::Block *scanningBlk = &cacheSet->at(sets);
				State *state = &cacheStateSet->at(sets);

				//A Write Hit
				if(translated_address.tag==cacheSet->at(sets).get_tag()&&state->get_valid()==true){
					miss=false;

					//No Bus Transaction
					state->set_dirty(true);

					break;
				}
			}

			if(!miss){
				//Write Hit

			}else{
				//Write Miss
				//Miss => access memory add 10 to processor cycle
				*wait_cycle+=10;

				//Fetch Cache Block from Memory
				CS4223::Processor::Transaction new_transaction(this->_proc_id,CS4223::Processor::Transaction::BusRd,address);
				this->_sharedBus->add_transaction(Bus::Type::BusRd,new_transaction);

				//Random Replacement at the cache set
				unsigned int random_blk_idx = rand()%cacheSet->size()+0;
				Processor::Block *selectedBlk = &cacheSet->at(random_blk_idx);
				BASIC::State *selectedState = &cacheStateSet->at(random_blk_idx);

				if(selectedState->get_dirty()){
					Processor::Transaction new_transaction(this->_proc_id,CS4223::Processor::Transaction::BusWr,address);
					this->_sharedBus->add_transaction(Bus::Type::BusWr,new_transaction);
				}

				selectedBlk->set_tag(translated_address.tag);
				selectedState->set_dirty(true);
				selectedState->set_valid(true);
			}
		}

		void BASIC::Snoop(CS4223::Processor::Transaction incoming_transaction){

		}
	}
}