#ifndef CACHE_H
#define CACHE_H

#include "../Bus.h"
#include "../Protocol.h"

#include "../Protocols/MESI.h"
#include "../Protocols/DRAGON.h"

using namespace std;

namespace CS4223{
	namespace Processor{
		class Cache{
			private:
				unsigned int _size;
				unsigned int _blk_size;
				unsigned short _assoc;
				Bus * const _sharedBus;
				Protocol *protocol;
			public:
				Cache(Bus * const sharedBus,Protocol::Type protocol_type, unsigned int size, unsigned short assoc,unsigned int blk_size);
				~Cache();
				bool flush(string address);
				bool write(string address);
				bool read(string address);
			};
	}
}
#endif