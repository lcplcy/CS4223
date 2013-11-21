#include "MESI.h"

namespace CS4223{
	namespace Protocols{
		
		MESI::MESI(const unsigned short proc_id,CS4223::Processor::Cache *cache, CS4223::Bus *sharedBus)			
		:_proc_id(proc_id),_cache(cache),_sharedBus(sharedBus)
		{
			vector<State> cache_set(this->_cache->get_associativity(),State());
			this->_cache_state = new vector<vector<State>>(this->_cache->get_num_of_cache_sets(),cache_set);
		}

		MESI::~MESI(){

		}

		void MESI::ProRd(string address,unsigned int *wait_cycle){
			
			bool miss = true;

			this->_cache->inc_cache_access();

			//Extract block identifier
			Processor::Cache::Address translated_address = this->_cache->translate_address(address);
			vector<Processor::Block> *cacheSet = this->_cache->get_cache_set((unsigned int)translated_address.cache_set_idx);
			vector<State> *cacheStateSet = &this->_cache_state->at((unsigned int)translated_address.cache_set_idx);
			State::state cacheState;

			//Cache will return hit or miss
			for(unsigned int sets=0;sets<cacheSet->size();sets++){
				
				cacheState = cacheStateSet->at(sets).get_state();

				//Compare each cache block within the cache set
				if(translated_address.tag==cacheSet->at(sets).get_tag()
				   &&
				   cacheState!=State::I){
					//A Hit
					miss=false;
					break;
				}
			}

			//Cache Hit - M, E, S
			if(!miss){	

				this->_cache->inc_hit();

				switch(cacheState){
					case State::M:
						{
							//Already exclusively yours don't do anything
						}
						break;
					case State::E:
						{
							//Already exclusively yours don't do anything
						}
						break;
					case State::S:
						{
							//Read only don't do anything
						}
						break;
					default:
						break;
				}
			}else{
			//Cache Miss - Invalid

				//Miss => access memory add 10 to processor cycle
				*wait_cycle+=10;

				//Send a new transaction
				CS4223::Processor::Transaction new_transaction(this->_proc_id,CS4223::Processor::Transaction::BusRd,address);
				this->_sharedBus->add_transaction(Bus::Type::BusRd,new_transaction);
			}

			//Okay stop here do not change state
		}

		void MESI::ProWr(string address,unsigned int *wait_cycle){
			
			bool miss = true;

			this->_cache->inc_cache_access();

			//Extract block identifier
			Processor::Cache::Address translated_address = this->_cache->translate_address(address);
			vector<Processor::Block> *cacheSet = this->_cache->get_cache_set((unsigned int)translated_address.cache_set_idx);
			vector<State> *cacheStateSet = &this->_cache_state->at((unsigned int)translated_address.cache_set_idx);
			State::state cacheState;

			//Cache will return hit or miss
			for(unsigned int sets=0;sets<cacheSet->size();sets++){
				
				cacheState = cacheStateSet->at(sets).get_state();

				if(translated_address.tag==cacheSet->at(sets).get_tag()
				   &&
				   cacheState!=State::state::I){
					//A Hit
					miss=false;
					break;
				}
			}

			//Cache Hit - M, E, S
			if(!miss){	
				switch(cacheState){
					case State::M:
						{
							//Already exclusively yours don't do anything
						}
						break;
					case State::E:
						{
							//Already exclusively yours don't do anything
						}
						break;
					case State::S:
						{
							CS4223::Processor::Transaction new_transaction(this->_proc_id,CS4223::Processor::Transaction::BusRdX,address);
							this->_sharedBus->add_transaction(Bus::Type::BusRdX,new_transaction);
						}
						break;
					default:
						break;
				}
			}else{
				//Cache Miss - Invalid

				//Miss => access memory add 10 to processor cycle
				*wait_cycle+=10;

				CS4223::Processor::Transaction new_transaction(this->_proc_id,CS4223::Processor::Transaction::BusRdX,address);
				this->_sharedBus->add_transaction(Bus::Type::BusRdX,new_transaction);

				//Okay stop here do not change state
			}
		}

		void MESI::Snoop(CS4223::Processor::Transaction incoming_transaction){
			
			//Extract Transaction Information
			unsigned short proc_id = incoming_transaction.get_proc_id();
			CS4223::Processor::Transaction::Type trans_type = incoming_transaction.get_type();
			string address = incoming_transaction.get_address();

			//Extract block identifier
			Processor::Cache::Address translated_address = this->_cache->translate_address(address);
			vector<Processor::Block> *cacheSet = this->_cache->get_cache_set((unsigned int)translated_address.cache_set_idx);
			vector<State> *cacheStateSet = &this->_cache_state->at((unsigned int)translated_address.cache_set_idx);
			State *cacheState;

			if(proc_id==this->_proc_id){
				//Issued by this processor => complete the state transition

				switch(trans_type){
					case CS4223::Processor::Transaction::Type::BusRd: // I -> S / I -> E
						{
							//Random Replacement at the cache set
							unsigned int random_blk_idx = rand()%cacheSet->size()+0;
							Processor::Block *selectedBlk = &cacheSet->at(random_blk_idx);
							State *selectedState = &cacheStateSet->at(random_blk_idx);

							if(selectedState->get_state()==State::M){
								//Write back to memory
								Processor::Transaction new_transaction(this->_proc_id,CS4223::Processor::Transaction::BusWr,address);
								this->_sharedBus->add_transaction(Bus::Type::BusWr,new_transaction);
							}

							selectedBlk->set_tag(translated_address.tag);

							//Check the shared line S
							if(this->_sharedBus->read_shared_line(address)>0){
								// S => Go to S State
								selectedState->set_state(State::S);
								this->_sharedBus->set_shared_line(address);
							}else{
								// S` => Go to E state
								selectedState->set_state(State::E);
							}
						}
						break;
					case CS4223::Processor::Transaction::Type::BusRdX:
						{
							for(unsigned int sets=0;sets<cacheSet->size();sets++){
				
								cacheState = &cacheStateSet->at(sets);

								if(translated_address.tag==cacheSet->at(sets).get_tag()
								   &&
								   cacheState->get_state()==State::I){
									
									cacheState->set_state(State::M);

									break;
								}else if(translated_address.tag==cacheSet->at(sets).get_tag()
										 &&
										cacheState->get_state()==State::S){
									
									cacheState->set_state(State::M);
									this->_sharedBus->unset_shared_line(address);

									break;
								}
							}
						}
						break;
				}
			}
			else{
				//Issued by other processor
				for(unsigned int sets=0;sets<cacheSet->size();sets++){
				
						cacheState = &cacheStateSet->at(sets);

						if(translated_address.tag==cacheSet->at(sets).get_tag()
							&&
							cacheState->get_state()!=State::state::I){
								break;
						}
				}

				switch(trans_type){

					case CS4223::Processor::Transaction::Type::BusRd:
						{	
							switch(cacheState->get_state()){
								case State::M:{
										cacheState->set_state(State::S);
										Processor::Transaction new_transaction(this->_proc_id,CS4223::Processor::Transaction::BusWr,address);
										this->_sharedBus->add_transaction(Bus::Type::BusWr,new_transaction);
									}
									break;
								case State::E:{
										cacheState->set_state(State::S);
										Processor::Transaction new_transaction(this->_proc_id,CS4223::Processor::Transaction::BusWr,address);
										this->_sharedBus->add_transaction(Bus::Type::BusWr,new_transaction);
									}
									break;
								case State::S:
									cacheState->set_state(State::S);
									break;
							}
						}
						break;
					case CS4223::Processor::Transaction::Type::BusRdX:
						{
							switch(cacheState->get_state()){
								case State::M:
									{
										cacheState->set_state(State::I);
										Processor::Transaction new_transaction(this->_proc_id,CS4223::Processor::Transaction::BusWr,address);
										this->_sharedBus->add_transaction(Bus::Type::BusWr,new_transaction);
									}
									break;
								case State::E:
									{
										cacheState->set_state(State::I);
										Processor::Transaction new_transaction(this->_proc_id,CS4223::Processor::Transaction::BusWr,address);
										this->_sharedBus->add_transaction(Bus::Type::BusWr,new_transaction);
									}
									break;
								case State::S:
									{
										cacheState->set_state(State::I);
										this->_sharedBus->unset_shared_line(address);
									}
									break;
							}
						}
						break;
				}
			}

		}
	}
}

