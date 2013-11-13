#include "Bus.h"

namespace CS4223{
	Bus::Bus(){
		this->bus_queue = new queue<CS4223::Processor::Transaction>();
		this->_total_address_traffic = 0;
		this->_total_data_traffic = 0;
		//Assuming number of processors in an array starts from 0, -1 means no one
		//is asserting the shared_line
		this->shared_line = false;
	}

	Bus::~Bus(){

	}

	void Bus::set_bytes_per_transaction(unsigned short address,unsigned short data){
		this->_address_traffic_per_transaction = address;  //Each bus transaction for address in bytes
		this->_data_traffic_per_transaction;
	}

	CS4223::Processor::Transaction* Bus::next_transaction(){
		if(!this->bus_queue->empty()){
			CS4223::Processor::Transaction processing_transaction = this->bus_queue->front();
			this->bus_queue->pop();
			return &processing_transaction;
		}else{
			return NULL;
		}
	}

	void Bus::add_transaction(Type transaction_type, CS4223::Processor::Transaction new_transaction){
		this->_total_address_traffic += this->_address_traffic_per_transaction;
		this->_total_data_traffic +=this->_data_traffic_per_transaction;
		this->bus_queue->push(new_transaction);
	}

	double Bus::get_total_address_traffic(){
		return this->_total_address_traffic;
	}

	double Bus::get_total_data_traffic(){
		return this->_total_data_traffic;
	}
}

