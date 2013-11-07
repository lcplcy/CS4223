#include "Transaction.h"

namespace CS4223{
	namespace Processor{
		
		Transaction::Transaction(string address){
			this->_address = address;
		}

		string Transaction::get_address(){
			return this->_address;
		}

	}
}