#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

using namespace std;

namespace CS4223{
	namespace Processor{
		class Transaction{
			private:
				string _address;
			public:
				enum Type{
					BusRd,
					BusRdX,
					BusUp,
					BusWr
				}_type;
				Transaction(Type type,string address);
				string Transaction::get_address();
				Type Transaction::get_type();
		};
	}
}

#endif