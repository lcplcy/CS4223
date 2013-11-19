#include "MESI.h"


namespace CS4223{
	namespace Protocols{
		
		MESI::MESI(const unsigned short proc_id,CS4223::Processor::Cache *cache, CS4223::Bus *sharedBus)			
			:_proc_id(proc_id),_cache(cache),_sharedBus(sharedBus){
			
		}

		MESI::~MESI(){

		}

		void MESI::ProRd(string address,unsigned int *wait_cycles){
	
		}

		void MESI::ProWr(string address,unsigned int *wait_cycle){

		}

		void MESI::Snoop(CS4223::Processor::Transaction incoming_transaction){

		}

	}
}

