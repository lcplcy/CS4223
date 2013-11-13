#include "Bus.h"

namespace CS4223{
	Bus::Bus(){
		this->bus_queue = new queue<CS4223::Processor::Transaction>();
		this->_total_address_traffic = 0;
		this->_total_data_traffic = 0;
	}

	Bus::~Bus(){

	}

	void Bus::set_bytes_per_transaction(unsigned short bus_address,
										unsigned short bus_rd_data,
										unsigned short bus_wr_data,
										unsigned short bus_rdx_data,
										unsigned short bus_up_data)
	{
		this->_bus_address_bytes = bus_address;
		this->_bus_rd_data_bytes = bus_rd_data;
		this->_bus_wr_data_bytes = bus_wr_data;
		this->_bus_rdx_data_bytes = bus_rdx_data;
		this->_bus_up_data_bytes = bus_up_data;
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

		this->_total_address_traffic += this->_bus_address_bytes;

		switch(transaction_type){
			case Type::BusRd:
				this->_total_data_traffic += this->_bus_rd_data_bytes;
				break;
			case Type::BusRdX:
				this->_total_data_traffic += this->_bus_rdx_data_bytes;
				break;
			case Type::BusUp:
				this->_total_data_traffic += this->_bus_up_data_bytes;
				break;
			case Type::BusWr:
				this->_total_data_traffic += this->_bus_wr_data_bytes;
				break;
		}

		this->bus_queue->push(new_transaction);
	}

	void Bus::set_shared_line(){
		this->S = true;
	}

	void Bus::unset_shared_line(){
		this->S = false;
	}

	double Bus::get_total_address_traffic(){
		return this->_total_address_traffic;
	}

	double Bus::get_total_data_traffic(){
		return this->_total_data_traffic;
	}
}

