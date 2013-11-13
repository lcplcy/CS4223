#include "DRAGON.h"



namespace CS4223{
	namespace Protocols{
		DRAGON::DRAGON(){

		}

		DRAGON::~DRAGON(){

		}

		//===CY DRAGON TODO===
		//From this cache's processor, if Write Miss,
		//From bus, if no other cache has it,
		//State: M

		//In M, if this cache's processor Write, stay in M
		

		//From this cache's processor, if Write Miss,
		//From bus, other cache has it,
		//State: Sm
		//Do BusUpdate to all other cache, change other Sm to Sc
		//Only this cache can be Sm

		
		//In Sm, if this cache's processor Write, and change other Sm to Sc
		//In Sm, if this cache's processor Write, and no other cache has it, NewState = M
		//In Sm, if another cache request via BusRd, do a flush and remain in Sm
		//In Sm, if another cache do a BusUpdate, NewState = Sc

		

		
		//In E, if this cache's processor Write, NewState = M
		

		

		
		//In Sc, if this cache's processor Write, NewState = Sm, Do bus update to change other Sm to Sc.
		//In Sc, if this cache's processor Write, and after bus update realise that no other cache has it, NewState = M
		//In Sc, if another cache request via BusRd, remain in Sc
		//In Sc, if another cache does a BusUpdate, remain in Sc
		//===CY DRAGON TODO===

		void DRAGON::ProRd(string address,unsigned int *wait_cycle){
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
			//In Sc, if this cache's processor Read stay in Sc
			//In E, if this cache's processor Read, stay in E
			//In Sm, if this cache's processor Read, stay in Sm
			//In M, if this cache's processor Read, stay in M
			if(!miss){
				this->_cache->inc_hit();
				//No change in DRAGON STATE.
			}else{
				//Cache Miss
				//Check if other cache's have it via bus, if not fetch from memory
				
				//Fetch Cache Block from Memory
				CS4223::Processor::Transaction new_transaction(address);
				this->_sharedBus->add_transaction(Bus::Type::DragonBusRd,new_transaction);

				//Random Replacement at the cache set
				unsigned int random_blk_idx = rand()%cacheSet->size()+0;
				Processor::Block *selectedBlk = &cacheSet->at(random_blk_idx);
				DRAGON::State *selectedState = &cacheStateSet->at(random_blk_idx);

				if(this->_sharedBus->shared_line){
					//From this cache's processor, if Read Miss,
					//From bus, other cache has it,
					//State: Sc
					//Other cache's have the memory requested, take from them instead.
					*wait_cycle+=1;
					selectedBlk->set_tag(translated_address.tag);
					selectedState->set_dragonstate("Sc");


				}else{
					//If Read Miss, take from memory, and change state to E
					//From this cache's processor, if Read Miss,
					//From bus, if no other cache has it,
					//State: E

					//Miss => access memory add 10 to processor cycle
					*wait_cycle+=10;

					/*What is get_dirty() in BASIC?
					if(selectedState->get_dirty()){
						Processor::Transaction new_transaction(address);
						this->_sharedBus->add_transaction(Bus::Type::BusWr,new_transaction);
					}*/
					selectedBlk->set_tag(translated_address.tag);
					selectedState->set_dragonstate("E");
				}
			}
		}


		void DRAGON::ProWr(string address,unsigned int *wait_cycle){


		}

		void DRAGON::Snoop(string address,unsigned int *wait_cycle){
			bool miss = true;
			this->_cache->inc_cache_access();

			//Extract block identifier
			Processor::Cache::Address translated_address = this->_cache->translate_address(address);
			vector<Processor::Block> *cacheSet = this->_cache->get_cache_set((unsigned int)translated_address.cache_set_idx);
			vector<State> *cacheStateSet = &this->_cache_state->at((unsigned int)translated_address.cache_set_idx);

			//Cache will return hit or Miss
			unsigned int getSet;
			for(unsigned int sets=0;sets<cacheSet->size();sets++){
				
				if(translated_address.tag==cacheSet->at(sets).get_tag()&&cacheStateSet->at(sets).get_valid()==true){
					//A Hit
					getSet = sets
					miss=false;
					break;
				}
			}
			if(!miss)
			//Assert shared line if I have the data
				this->_sharedBus->set_shared_line()

				CS4223::Processor::Transaction new_transaction(address);
				this->_sharedBus->add_transaction(Bus::Type::DragonBusRd,new_transaction);
				DRAGON::State *selectedState = &cacheStateSet->at(getSet);
				//In E, if another cache request via BusRd, NewState = Sc
				if(selectedState->get_dragonstate=="E")
					selectedState->set_dragonstate("Sc")
				//In M, if another cache request via BusRd, do a flush and change to Sm
				if(selectedState->get_dragonstate=="M"){
					this->_cache->
					selectedState->set_dragonstate("Sc")
				}


		}

	}
}
//init - Initialize an array to keep track of the states of each cache byte
//processor -> cache (protocol) -> bus -> protocol
//DRAGON METHODS:

/*
<this is cache.cpp>

while(not_end)
{
busTransaction = bus.busTransaction()
if busTransaction == "busRdx" && busUser != me
	dragon.doSomething(instruction,&cycle, &clock)
if(protocol = dragon && busUser == me)
	dragon.doSomething(instruction,&cycle, &clock)
if(protocol = mesi && busUser == me)
	mesi.doSomething()
}

<this is dragon.cpp>
doSomething(string instruction, int *cycle, int *clock)//public
{
	if instruction == READ
		if(busRd()) //private to dragon
			cout << pass;
		else
			cout << fail;
	if instruction == RdX
		if(busRdX())
	if instruction == someone_sent_a_read_x
		invalidate(cache_memory)
}

private bool busRdX()
{
	bool bus_read_pass_fail;
	.
	.
	.
	bus.busRdX()
	return bus_read_pass_fail;
}
<this is bus.cpp>
void busRdX()
{
	//send a signal to all other cache to invalidate their data
}
//take in &clock, &cycle and make respective changes
//

*/
