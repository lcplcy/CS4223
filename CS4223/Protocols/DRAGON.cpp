#include "DRAGON.h"
//init - Initialize an array to keep track of the states of each cache byte
//processor -> cache (protocol) -> bus -> protocol
//DRAGON METHODS:

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
CS4223::Protocols::DRAGON::DRAGON(){

}

CS4223::Protocols::DRAGON::~DRAGON(){

}