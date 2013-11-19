#include "Transaction.h"

namespace CS4223{
	namespace Processor{
		
		Transaction::Transaction(const unsigned short proc_id,Type type,string address):
			_issuing_proc(proc_id),	_type(type),_address(address)
		{
		}

		unsigned short Transaction::get_proc_id(){
			return _issuing_proc;
		}

		string Transaction::get_address(){
			return this->_address;
		}
	
		CS4223::Processor::Transaction::Type Transaction::get_type(){
			return this->_type;
		}
		
	}
}