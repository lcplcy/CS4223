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
				Transaction(string address);
				string Transaction::get_address();
		};
	}
}

#endif