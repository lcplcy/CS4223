#include "DRAGON.h"



namespace CS4223{
	namespace Protocols{
		DRAGON::DRAGON(){

		}

		DRAGON::~DRAGON(){

		}

		//From this cache's processor, if Write Miss,
		//From bus, if no other cache has it,
		//State: M

		//In M, if this cache's processor Read, stay in M
		//In M, if this cache's processor Write, stay in M
		//In M, if another cache request via BusRd, do a flush and change to Sm

		//From this cache's processor, if Write Miss,
		//From bus, other cache has it,
		//State: Sm
		//Do BusUpdate to all other cache, change other Sm to Sc
		//Only this cache can be Sm

		//In Sm, if this cache's processor Read, stay in Sm
		//In Sm, if this cache's processor Write, and change other Sm to Sc
		//In Sm, if this cache's processor Write, and no other cache has it, NewState = M
		//In Sm, if another cache request via BusRd, do a flush and remain in Sm
		//In Sm, if another cache do a BusUpdate, NewState = Sc

		//From this cache's processor, if Read Miss,
		//From bus, if no other cache has it,
		//State: E

		//In E, if this cache's processor Read, stay in E
		//In E, if this cache's processor Write, NewState = M
		//In E, if another cache request via BusRd, NewState = Sc

		//From this cache's processor, if Read Miss,
		//From bus, other cache has it,
		//State: Sc

		//In Sc, if this cache's processor Read stay in Sc
		//In Sc, if this cache's processor Write, NewState = Sm, Do bus update to change other Sm to Sc.
		//In Sc, if this cache's processor Write, and after bus update realise that no other cache has it, NewState = M
		//In Sc, if another cache request via BusRd, remain in Sc
		//In Sc, if another cache does a BusUpdate, remain in Sc

		public void doSomething(string instruction, int *cycle, int *clock)//public
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

		private bool DRAGON::PrRd(unsigned int* cycle, unsigned int* clock){
			bool success = false;
			return success;
		}

		private bool DRAGON::PrWr(unsigned int* cycle, unsigned int* clock){
			bool success = false;
		}
		
		void DRAGON::ProRd(string address,unsigned int *wait_cycle){}

		private bool DRAGON::BusRd(unsigned int* cycle, unsigned int* clock){
			bool success = false;
			return success;
		}
		
		void DRAGON::ProWr(string address,unsigned int *wait_cycle){}

		
		private bool DRAGON::BusRdX(unsigned int* cycle, unsigned int* clock){
			bool success = false;
			return success;
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
