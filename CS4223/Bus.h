#ifndef BUS_H
#define BUS_H

#include <string>

using namespace std;

namespace CS4223{

	class Bus{
		private:
			bool S;
			string address;
		public:
			Bus();
			~Bus();
			void set_address(string address);
	};
}
#endif