#include "MESI.h"


namespace CS4223{
	namespace Protocols{

		MESI::MESI(CS4223::Processor::Cache *cache, CS4223::Bus *sharedBus)			
		:_cache(cache),_sharedBus(sharedBus){
			
		}

		MESI::~MESI(){

		}

		void MESI::ProRd(string address,unsigned int *wait_cycles){
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
		}

		void MESI::ProWr(string address,unsigned int *wait_cycle){
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
		}

		void MESI::BusRd(string address,unsigned int *wait_cycle){
			if(State == M)
			{
				State = S;
				*wait_cycle+=10;
			}
			if(State == E)
			{
				State = S;
				*wait_cycle+=10;
			}
			if(State == S)
				State = S;
		}

		void MESI::BusRdX(string address,unsigned int *wait_cycle){
			if(State == M)
			{
				State = I;
				*wait_cycle+=10;
			}
			if(State == E)
			{
				State = I;
				*wait_cycle+=10;
			}
			if(State == S)
				State = I;
		}
	}
}

