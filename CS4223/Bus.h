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
			//DRAGON
			bool shared_line;
			//If read miss, other cache will use this shared_line to indicate that they have
			//the data, and pass the data to the requesting cache.
			//If write hit, and if other cache assert the shared_line to indicate they also have
			//the data, Change to Shared-Dirty and change all others to Shared-Clean.
		public:
			Bus();
			~Bus();

			enum Type{
				//Common
				BusRd,
				//MESI
				BusRdX,
				//DRAGON
				BusUpdate
			};

			void set_bytes_per_transaction(unsigned short address,unsigned short data);
			void add_transaction(Type transation_type, CS4223::Processor::Transaction new_transaction);
			CS4223::Processor::Transaction* next_transaction();
			double get_total_address_traffic();
			double get_total_data_traffic();
			//If a cache has a memory address that is being requested via BusRd, assert the shared line and
			//return the respective cache that has the memory address.
			void assert_shared_line();
	};
}
#endif