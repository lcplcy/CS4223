#include "MOESI.h"

namespace CS4223{
	namespace Protocols{
		
		MOESI::MOESI(const unsigned short proc_id, CS4223::Processor::Cache *cache, CS4223::Bus *sharedBus){
		
		}

		MOESI::~MOESI(){
		}

		void MOESI::ProRd(string address,unsigned int *wait_cycle){

		}

		void MOESI::ProWr(string address,unsigned int *wait_cycle){

		}
		
		void MOESI::Snoop(CS4223::Processor::Transaction incoming_transaction){

		}
	}
}