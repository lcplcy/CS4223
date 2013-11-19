#include "MESI.h"


namespace CS4223{
	namespace Protocols{
		
		MESI::MESI(const unsigned short proc_id,CS4223::Processor::Cache *cache, CS4223::Bus *sharedBus)			
			:_proc_id(proc_id),_cache(cache),_sharedBus(sharedBus){
			
		}

		MESI::~MESI(){

		}

		void MESI::ProRd(string address,unsigned int *wait_cycles){
			/*
			if(State == I && Bus::Type::BusRd == true)
				State = S;
			if(State == I && Bus::Type::BusRd == false)
				State = E;
			if(State == S)
				State = S;
			if(State == E)
				State = E;
			if(State == M)
				State = M;
			*/
		}

		void MESI::ProWr(string address,unsigned int *wait_cycle){
			/*
			if(State == I)
			{
				State = M;
				Bus::Type::BusRdX = true;
			}
			if(State == S)
			{
				State = M;
				Bus::Type::BusRdX = true;
			}
			if(State == E)
				State = M;
			if(State == M)
				State = M;
				*/
		}

		void MESI::Snoop(CS4223::Processor::Transaction incoming_transaction){

		}

	}
}

