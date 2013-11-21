#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <string>

#include "Processor\Transaction.h"

using namespace std;

/**
 *  Interface class for Protocol Concrete Classes
 */

namespace CS4223{
	class Protocol{
		private:
		public:
			enum Type{
				NONE,
				MESI,
				DRAGON,
				MOESI
			};
			
			virtual void ProRd(string address,unsigned int *wait_cycle)=0;
			virtual void ProWr(string address,unsigned int *wait_cycle)=0;
			virtual void Snoop(CS4223::Processor::Transaction incoming_transaction)=0;
	};
}
#endif