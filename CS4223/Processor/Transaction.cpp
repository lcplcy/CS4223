#include "Transaction.h"

namespace CS4223{
	namespace Processor{
		
		Transaction::Transaction(Type type,string address):_type(type),_address(address)
		{
		}

		string Transaction::get_address(){
			return this->_address;
		}
	
		CS4223::Processor::Transaction::Type Transaction::get_type(){
			return this->_type;
		}
		
	}
}