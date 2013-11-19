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
			unsigned short _bus_address_bytes;
			unsigned short _bus_rd_data_bytes;
			unsigned short _bus_wr_data_bytes;
			unsigned short _bus_rdx_data_bytes;
			unsigned short _bus_up_data_bytes;
			double _total_address_traffic;
			double _total_data_traffic;
		public:
			Bus();
			~Bus();
			enum Type{
				BusRd,
				BusRdX,
				BusUp,
				BusWr
			};
			void set_bytes_per_transaction(unsigned short bus_address,unsigned short bus_rd_data,
												unsigned short bus_wr_data,unsigned short bus_rdx_data,
												unsigned int short _bus_up_data);
			void add_transaction(Type transation_type, CS4223::Processor::Transaction new_transaction);
			CS4223::Processor::Transaction* next_transaction();
			void set_shared_line();
			void unset_shared_line();
			bool read_shared_line();
			double get_total_address_traffic();
			double get_total_data_traffic();
	};
}
#endif