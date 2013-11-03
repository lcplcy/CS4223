#ifndef BASIC_H
#define BASIC_H

#include "..\Protocol.h"

using namespace std;

namespace CS4223{
	namespace Protocols{

		class BASIC : public CS4223::Protocol {
			private:
			public:
				BASIC();
				~BASIC();
		};

	}
}
#endif