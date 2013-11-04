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
		public:
			Bus();
			~Bus();
			void add_transaction(CS4223::Processor::Transaction new_transaction);
			void next_transaction();
	};
}
#endif