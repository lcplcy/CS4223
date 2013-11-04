#include "BASIC.h"


namespace CS4223{
	namespace Protocols{

		BASIC::BASIC(CS4223::Processor::Cache *cache, CS4223::Bus *sharedBus)
			:_cache(cache),_sharedBus(sharedBus)
		{
		
		}

		BASIC::~BASIC(){

		} 

		bool BASIC::ProRd(string address,unsigned int *wait_cycle){

			bool success = false;

			//Cache will return hit or miss
			if(this->_cache->read(address)){

			}else{

				//Send transaction on the bus
				CS4223::Processor::Transaction new_transaction(address);
				this->_sharedBus->add_transaction(new_transaction);
				this->_cache->fetch(address);

				//Miss => access memory add 10 to processor cycle
				*wait_cycle+=10;	
			}

			return success;
		}

		bool BASIC::ProWr(string address,unsigned int *wait_cycle){

			bool success = false;

			this->_cache->write(address);

			return success;
		}
	}
}