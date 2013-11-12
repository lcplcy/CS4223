#ifndef BLOCK_H
#define BLOCK_H

#include <string>

using namespace std;

namespace CS4223{
	namespace Processor{
		class Block{
					private:
						unsigned int _size;
						string Tag;
					public:
						Block(unsigned int size);
						void set_tag(string tag);
						string get_tag();
			};
	}
}

#endif