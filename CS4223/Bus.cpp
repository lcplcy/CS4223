#include "Bus.h"

namespace CS4223{
	Bus::Bus(){
		this->bus_queue = new queue<CS4223::Processor::Transaction>();
	}

	Bus::~Bus(){

	}

	void Bus::next_transaction(){

		if(!this->bus_queue->empty()){
			CS4223::Processor::Transaction processing_transaction = this->bus_queue->front();
			this->bus_queue->pop();
		}
	}

	void Bus::add_transaction(CS4223::Processor::Transaction new_transaction){
		this->bus_queue->push(new_transaction);
	}
}

