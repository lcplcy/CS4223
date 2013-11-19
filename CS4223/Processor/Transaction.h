#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

using namespace std;

namespace CS4223{
	namespace Processor{
		class Transaction{
			private:
				string _address;
				unsigned short _issuing_proc;
			public:
				enum Type{
					BusRd,
					BusRdX,
					BusUp,
					BusWr
				}_type;
				Transaction(const unsigned short proc_id, Type type,string address);
				unsigned short get_proc_id();
				string get_address();
				Type get_type();
		};
	}
}

#endif