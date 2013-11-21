#include "DRAGON.h"

namespace CS4223{
	namespace Protocols{

		
		DRAGON::DRAGON(const unsigned short proc_id,CS4223::Processor::Cache *cache, CS4223::Bus *sharedBus)
			:_proc_id(proc_id),_cache(cache),_sharedBus(sharedBus)
		{
			vector<State> cache_set(this->_cache->get_associativity(),State());
			this->_cache_state = new vector<vector<State>>(this->_cache->get_num_of_cache_sets(),cache_set);
		}

		DRAGON::~DRAGON(){

		}

		void DRAGON::ProRd(string address,unsigned int *wait_cycle){
			this ->prev_step = "R";
			bool miss = true;
			this->_cache->inc_cache_access();
			
			//Extract block identifier
			Processor::Cache::Address translated_address = this->_cache->translate_address(address);
			vector<Processor::Block> *cacheSet = this->_cache->get_cache_set((unsigned int)translated_address.cache_set_idx);
			vector<State> *cacheStateSet = &this->_cache_state->at((unsigned int)translated_address.cache_set_idx);

			//Cache will return hit or miss
			for(unsigned int sets=0;sets<cacheSet->size();sets++){
				if(translated_address.tag==cacheSet->at(sets).get_tag()&&cacheStateSet->at(sets).get_dragonstate()=="M"
					|| translated_address.tag==cacheSet->at(sets).get_tag()&&cacheStateSet->at(sets).get_dragonstate()=="E"
					|| translated_address.tag==cacheSet->at(sets).get_tag()&&cacheStateSet->at(sets).get_dragonstate()=="Sc"
					|| translated_address.tag==cacheSet->at(sets).get_tag()&&cacheStateSet->at(sets).get_dragonstate()=="Sm"){
					//A Hit
					miss=false;
					break;
				}
			}

			
			if(!miss){
				//Cache Hit
				//In Sc, if this cache's processor Read stay in Sc
				//In E, if this cache's processor Read, stay in E
				//In Sm, if this cache's processor Read, stay in Sm
				//In M, if this cache's processor Read, stay in M
				this->_cache->inc_hit();
				*wait_cycle+=1;
				//No change in DRAGON STATE.
			}else{
				//Cache Miss
				//Check if other cache's have it via bus, if not fetch from memory
				
				//Send a bus read to request from other caches or memory
				CS4223::Processor::Transaction newtrans = CS4223::Processor::Transaction(this->_proc_id,CS4223::Processor::Transaction::BusRd, address);
				this->_sharedBus->add_transaction(Bus::Type::BusRd,newtrans);
				*wait_cycle+=1;
			}
		}

		
		void DRAGON::ProWr(string address,unsigned int *wait_cycle){
			
			this ->prev_step = "W";
			bool miss = true;
			int getSet;
			this->_cache->inc_cache_access();

			//Extract block identifier
			Processor::Cache::Address translated_address = this->_cache->translate_address(address);
			vector<Processor::Block> *cacheSet = this->_cache->get_cache_set((unsigned int)translated_address.cache_set_idx);
			vector<State> *cacheStateSet = &this->_cache_state->at((unsigned int)translated_address.cache_set_idx);

			//Cache will return hit or miss
			for(unsigned int sets=0;sets<cacheSet->size();sets++){

				if(translated_address.tag==cacheSet->at(sets).get_tag()&&cacheStateSet->at(sets).get_dragonstate()=="M"
					|| translated_address.tag==cacheSet->at(sets).get_tag()&&cacheStateSet->at(sets).get_dragonstate()=="E"
					|| translated_address.tag==cacheSet->at(sets).get_tag()&&cacheStateSet->at(sets).get_dragonstate()=="Sc"
					|| translated_address.tag==cacheSet->at(sets).get_tag()&&cacheStateSet->at(sets).get_dragonstate()=="Sm"){
					//A Hit
					getSet = sets;
					miss=false;
					break;
				}	
			}
			if(!miss){
				//Cache Hit
				DRAGON::State *selectedState = &cacheStateSet->at(getSet);
				//In Sc, if this cache's processor Write, NewState = Sm, Do bus update to change other Sm to Sc.
				if(selectedState->get_dragonstate()=="Sc"){
					selectedState->set_dragonstate("Sm");
					this->_cache->inc_hit();
					*wait_cycle+=1;
				}
				//In E, if this cache's processor Write, NewState = M
				else if(selectedState->get_dragonstate()=="E"){
					selectedState->set_dragonstate("M");
					this->_cache->inc_hit();
					*wait_cycle+=1;
				}
				//In M, if this cache's processor Write, stay in M
				else if(selectedState->get_dragonstate()=="M"){
					selectedState->set_dragonstate("M");
					this->_cache->inc_hit();
					*wait_cycle+=1;
				}
				//In Sm, if this cache's processor Write, and change other Sm to Sc
				//In Sm, if this cache's processor Write, and no other cache has it, NewState = M
				else if(selectedState->get_dragonstate()=="Sm"){
					CS4223::Processor::Transaction newtrans = CS4223::Processor::Transaction(this->_proc_id,CS4223::Processor::Transaction::BusUp, address);
					this->_sharedBus->add_transaction(Bus::Type::BusUp,newtrans);
					*wait_cycle+=1;
					this->_cache->inc_hit();
				}
				
			}else{
				//Cache Miss
				//Check if other cache's have it via bus, if not fetch from memory
				
				//Send a bus read to request from other caches or memory
				CS4223::Processor::Transaction newtrans = CS4223::Processor::Transaction(this->_proc_id,CS4223::Processor::Transaction::BusRd, address);
				this->_sharedBus->add_transaction(Bus::Type::BusRd,newtrans);
				*wait_cycle+=1;
			}
		}

		void DRAGON::Snoop(CS4223::Processor::Transaction incoming_transaction){

			//Extract Transaction Information
			unsigned short proc_id = incoming_transaction.get_proc_id();
			CS4223::Processor::Transaction::Type trans_type = incoming_transaction.get_type();
			string address = incoming_transaction.get_address();

			//BusUpdate and no other cache has my stuff
			if(trans_type == CS4223::Processor::Transaction::BusUp 
				&& proc_id== this->_proc_id
				&& this->_sharedBus->read_shared_line(address)>0)
			{
				bool miss = true;
				//Extract block identifier
				Processor::Cache::Address translated_address = this->_cache->translate_address(address);
				vector<Processor::Block> *cacheSet = this->_cache->get_cache_set((unsigned int)translated_address.cache_set_idx);
				vector<State> *cacheStateSet = &this->_cache_state->at((unsigned int)translated_address.cache_set_idx);

				//Cache will return hit or Miss
				unsigned int getSet;
				for(unsigned int sets=0;sets<cacheSet->size();sets++){
					
					if(translated_address.tag==cacheSet->at(sets).get_tag()&&cacheStateSet->at(sets).get_dragonstate()=="M" ||
						translated_address.tag==cacheSet->at(sets).get_tag()&&cacheStateSet->at(sets).get_dragonstate()=="Sc" ||
						translated_address.tag==cacheSet->at(sets).get_tag()&&cacheStateSet->at(sets).get_dragonstate()=="Sm" ||
						translated_address.tag==cacheSet->at(sets).get_tag()&&cacheStateSet->at(sets).get_dragonstate()=="E"){
						//A Hit
						getSet = sets;
						miss=false;
						break;
					}
				}
				if(!miss){
					//In Sc, if this cache's processor Write, 
					//and after bus update realise that no other cache has it, NewState = M
					DRAGON::State *selectedState = &cacheStateSet->at(getSet);
					selectedState->set_dragonstate("M");
				}
				
			}
			//BusUpdate and another cache editing my stuff
			if(trans_type == CS4223::Processor::Transaction::BusUp
				&& proc_id != this->_proc_id)
			{
				bool miss = true;
				//Extract block identifier
				Processor::Cache::Address translated_address = this->_cache->translate_address(address);
				vector<Processor::Block> *cacheSet = this->_cache->get_cache_set((unsigned int)translated_address.cache_set_idx);
				vector<State> *cacheStateSet = &this->_cache_state->at((unsigned int)translated_address.cache_set_idx);

				//Cache will return hit or Miss
				unsigned int getSet;
				for(unsigned int sets=0;sets<cacheSet->size();sets++){
					
					if(translated_address.tag==cacheSet->at(sets).get_tag()&&cacheStateSet->at(sets).get_dragonstate()=="M" ||
						translated_address.tag==cacheSet->at(sets).get_tag()&&cacheStateSet->at(sets).get_dragonstate()=="Sc" ||
						translated_address.tag==cacheSet->at(sets).get_tag()&&cacheStateSet->at(sets).get_dragonstate()=="Sm" ||
						translated_address.tag==cacheSet->at(sets).get_tag()&&cacheStateSet->at(sets).get_dragonstate()=="E"){
						//A Hit
						getSet = sets;
						miss=false;
						break;
					}
				}
				if(!miss){
					//In Sm, if another cache do a BusUpdate, NewState = Sc
					//In Sc, if another cache does a BusUpdate, remain in Sc
					DRAGON::State *selectedState = &cacheStateSet->at(getSet);
					this->_sharedBus->set_shared_line(address);
					selectedState->set_dragonstate("Sc");
				}
			}
			//BusRead and another cache has asserted that he has my stuff
			else if(trans_type == CS4223::Processor::Transaction::BusRd
				&& this->_sharedBus->read_shared_line(address)>0 
				&& proc_id == this->_proc_id){
				//Extract block identifier
				Processor::Cache::Address translated_address = this->_cache->translate_address(address);
				vector<Processor::Block> *cacheSet = this->_cache->get_cache_set((unsigned int)translated_address.cache_set_idx);
				vector<State> *cacheStateSet = &this->_cache_state->at((unsigned int)translated_address.cache_set_idx);
				
				//Random Replacement at the cache set
				unsigned int random_blk_idx = rand()%cacheSet->size()+0;
				Processor::Block *selectedBlk = &cacheSet->at(random_blk_idx);
				DRAGON::State *selectedState = &cacheStateSet->at(random_blk_idx);
				if(this->prev_step == "R"){
					//From this cache's processor, if Read Miss,
					//From bus, other cache has it,
					//State: Sc
					//Other cache's have the memory requested, take from them instead.
					selectedBlk->set_tag(translated_address.tag);
					selectedState->set_dragonstate("Sc");
					this->_sharedBus->unset_shared_line(address);
				}else if(this->prev_step == "W"){
					//From this cache's processor, if Write Miss,
					//From bus, other cache has it,
					//State: Sm
					//Do BusUpdate to all other cache, change other Sm to Sc
					//Only this cache can be Sm
					selectedBlk->set_tag(translated_address.tag);
					selectedState->set_dragonstate("Sm");
					CS4223::Processor::Transaction newtrans = CS4223::Processor::Transaction(this->_proc_id,CS4223::Processor::Transaction::BusUp, address);
					this->_sharedBus->add_transaction(Bus::Type::BusUp,newtrans);
					this->_sharedBus->unset_shared_line(address);
				
				}
			}
			//BusRead and another cache wants my stuff
			else if(trans_type == CS4223::Processor::Transaction::BusRd
				&& this->_sharedBus->read_shared_line(address) >0 
				&& proc_id != this->_proc_id){
				bool miss = true;
				this->_cache->inc_cache_access();

				//Extract block identifier
				Processor::Cache::Address translated_address = this->_cache->translate_address(address);
				vector<Processor::Block> *cacheSet = this->_cache->get_cache_set((unsigned int)translated_address.cache_set_idx);
				vector<State> *cacheStateSet = &this->_cache_state->at((unsigned int)translated_address.cache_set_idx);

				//Cache will return hit or Miss
				unsigned int getSet;
				for(unsigned int sets=0;sets<cacheSet->size();sets++){
					
					if(translated_address.tag==cacheSet->at(sets).get_tag()&&cacheStateSet->at(sets).get_dragonstate()=="M" ||
						translated_address.tag==cacheSet->at(sets).get_tag()&&cacheStateSet->at(sets).get_dragonstate()=="Sc" ||
						translated_address.tag==cacheSet->at(sets).get_tag()&&cacheStateSet->at(sets).get_dragonstate()=="Sm" ||
						translated_address.tag==cacheSet->at(sets).get_tag()&&cacheStateSet->at(sets).get_dragonstate()=="E"){
						//A Hit
						getSet = sets;
						miss=false;
						break;
					}
				}
				if(!miss){
				//Assert shared line if I have the data
					this->_sharedBus->set_shared_line(address);
					//CS4223::Processor::Transaction new_transaction(CS4223::Processor::Transaction::Type::BusRd, address);
					//this->_sharedBus->add_transaction(Bus::Type::BusRd,new_transaction);
					DRAGON::State *selectedState = &cacheStateSet->at(getSet);
					//In E, if another cache request via BusRd, NewState = Sc
					if(selectedState->get_dragonstate()=="E")
						selectedState->set_dragonstate("Sc");
					//In M, if another cache request via BusRd, do a flush and change to Sm
					else if(selectedState->get_dragonstate()=="M"){
						this->_cache->flush();
						selectedState->set_dragonstate("Sm");
					}
					//In Sm, if another cache request via BusRd, do a flush and remain in Sm
					else if(selectedState->get_dragonstate()=="Sm"){
						this->_cache->flush();
						selectedState->set_dragonstate("Sm");
					}
					//In Sc, if another cache request via BusRd, remain in Sc
					else if(selectedState->get_dragonstate()=="Sc"){
						selectedState->set_dragonstate("Sc");
					}
				}
			}
			//BusRead and no other cache has my stuff
			else if(trans_type == CS4223::Processor::Transaction::BusRd
				&& this->_sharedBus->read_shared_line(address) > 0 
				&& proc_id != this->_proc_id){
				
					//Miss => access memory add 10 to processor cycle
					//Extract block identifier
					Processor::Cache::Address translated_address = this->_cache->translate_address(address);
					vector<Processor::Block> *cacheSet = this->_cache->get_cache_set((unsigned int)translated_address.cache_set_idx);
					vector<State> *cacheStateSet = &this->_cache_state->at((unsigned int)translated_address.cache_set_idx);
				
					//Random Replacement at the cache set
					unsigned int random_blk_idx = rand()%cacheSet->size()+0;
					Processor::Block *selectedBlk = &cacheSet->at(random_blk_idx);
					DRAGON::State *selectedState = &cacheStateSet->at(random_blk_idx);
					
					if(this->prev_step=="R"){
						//From this cache's processor, if Read Miss,
						//From bus, if no other cache has it,
						//State: E
						selectedBlk->set_tag(translated_address.tag);
						selectedState->set_dragonstate("E");
						this->_sharedBus->unset_shared_line(address);
					}else if(this->prev_step=="W"){
						selectedBlk->set_tag(translated_address.tag);
						selectedState->set_dragonstate("M");
						this->_sharedBus->unset_shared_line(address);
					}
			}
		}

		//===CY DRAGON TODO===
		//From this cache's processor, if Write Miss,
		//From bus, if no other cache has it,
		//State: M
	}
}

/*For reference
//If Read Miss, take from memory, and change state to E
//From this cache's processor, if Read Miss,
//From bus, if no other cache has it,
//State: E

//Miss => access memory add 10 to processor cycle
*wait_cycle+=10;

//Random Replacement at the cache set
unsigned int random_blk_idx = rand()%cacheSet->size()+0;
Processor::Block *selectedBlk = &cacheSet->at(random_blk_idx);
DRAGON::State *selectedState = &cacheStateSet->at(random_blk_idx);

selectedBlk->set_tag(translated_address.tag);
selectedState->set_dragonstate("E");
this->cycle = 0;
return true;*/
