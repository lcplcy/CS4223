#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <string>

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
				DRAGON
			};
			
			virtual void ProRd(string address,unsigned int *wait_cycle)=0;
			virtual void ProWr(string address,unsigned int *wait_cycle)=0;
	};
}
#endif