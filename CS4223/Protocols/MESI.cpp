#include "MESI.h"


namespace CS4223{
	namespace Protocols{

		MESI::MESI(CS4223::Processor::Cache *cache, CS4223::Bus *sharedBus)			
		:_cache(cache),_sharedBus(sharedBus){
			
		}

		MESI::~MESI(){

		}

		void MESI::ProRd(string address,unsigned int *wait_cycles){
		}

		void MESI::ProWr(string address,unsigned int *wait_cycle){
		}
	}
}

