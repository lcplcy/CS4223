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
			class State{
				public:
					State(){
					};
			};
			virtual bool ProRd(string address,unsigned int *wait_cycle)=0;
			virtual bool ProWr(string address,unsigned int *wait_cycle)=0;
	};
}
#endif