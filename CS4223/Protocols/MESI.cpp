#include "MESI.h"


namespace CS4223{
	namespace Protocols{

			MESI::MESI(){
			/*
			bool prord(Cache *cache, Bus *bus, Cycle *proc_cycle, int address){
				//cache hit
				if(cache.read(address)){
					proc_cycle++;
				}
				//cache miss
				else{
					//if other processors have this mem address in cache
					//send me the data!
					if(bus.request(address)){

					}
					//if other processors hdon't ave this mem address in cache
					//get from memory and +10 cycles
					else
					{
						proc_cycle = proc_cycle+10;
					}
				}

			}
			*/
		}

		MESI::~MESI(){

		}

		bool MESI::ProRd(string address,unsigned int *wait_cycles){
			bool success = false;

			return success;
		}

		bool MESI::ProWr(string address,unsigned int *wait_cycle){
			bool success = false;

			return success;
		}
	}
}

