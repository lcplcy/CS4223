#ifndef BUS_H
#define BUS_H

#include <string>
#include <queue>

using namespace std;

namespace CS4223{

	class Bus{
		private:
			bool S;
			string address;
		public:
			Bus();
			~Bus();
			void next_transaction();
	};
}
#endif