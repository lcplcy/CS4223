#include "Transaction.h"

namespace CS4223{
	namespace Processor{
		
		Transaction::Transaction(string address):_address(address)
		{
		}

		string Transaction::get_address(){
			return this->_address;
		}
		/*
		Bus::Type Transaction::get_type(){
			return this->_type;
		}
		*/
	}
}