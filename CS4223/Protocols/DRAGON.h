#ifndef DRAGON_H
#define DRAGON_H

#include "..\Protocol.h"

using namespace std;

namespace CS4223{
	namespace Protocols{

		class DRAGON : public CS4223::Protocol {
			private:
			public:
				DRAGON();
				~DRAGON();
				void ProRd(string address,unsigned int *wait_cycle);
				void ProWr(string address,unsigned int *wait_cycle);
		};

	}
}
#endif