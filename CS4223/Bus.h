#ifndef BUS_H
#define BUS_H

#include <string>
#include <queue>

#include "Processor\Transaction.h"

using namespace std;

namespace CS4223{

	class Bus{
		private:
			bool S;
			queue<CS4223::Processor::Transaction> *bus_queue;
			unsigned int short _address_traffic_per_transaction;
			unsigned int short _data_traffic_per_transaction;
			double _total_address_traffic;
			double _total_data_traffic;
		public:
			Bus();
			~Bus();

			enum Type{
				BusRd,
				BusWr,
				BusRdX,
			};

			void set_bytes_per_transaction(unsigned short address,unsigned short data);
			void add_transaction(Type transation_type, CS4223::Processor::Transaction new_transaction);
			CS4223::Processor::Transaction* next_transaction();
			double get_total_address_traffic();
			double get_total_data_traffic();
	};
}
#endif