#ifndef CACHE_H
#define CACHE_H

#include <vector>

#include "../Bus.h"
#include "../Protocol.h"


using namespace std;

namespace CS4223{
	namespace Processor{
		class Cache{
			private:
				unsigned int _size;
				unsigned int _blk_size;
				unsigned short _assoc;
				Bus * const _sharedBus;
				unsigned int _hit;
				unsigned int _mem_access;
				
				class Block{
					private:
						unsigned int _size;
					public:
						Block(unsigned int size): _size(size){
						}
				};

				vector<vector<Block>> *_cache;
			public:
				Cache(Bus * const sharedBus, unsigned int size, unsigned short assoc,unsigned int blk_size);
				~Cache();
				bool flush(string address);
				bool write(string address);
				bool read(string address);
				double get_miss_ratio();
			};
	}
}
#endif