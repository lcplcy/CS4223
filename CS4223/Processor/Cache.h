#ifndef CACHE_H
#define CACHE_H

#include <vector>
#include <cmath>
#include <stdlib.h>

#include "../Bus.h"
#include "../Protocol.h"
#include "Transaction.h"

using namespace std;

namespace CS4223{
	namespace Processor{
		class Cache{
			private:
				//Cache Info
				Bus *_sharedBus;
				unsigned int _size;
				unsigned int _blk_size;
				unsigned short _assoc;
				unsigned int _cache_sets;
				unsigned int _num_of_blks;

				unsigned short _memory_addr_len_bits;
				unsigned short _N_offset_len_bits;
				unsigned short _M_cache_set_len_bits;
				unsigned short _tag_len_bits;
				unsigned short _word_size_bytes;

				//Analytics
				unsigned int _hit;
				unsigned int _mem_access;

				class Block{
					private:
						unsigned int _size;
						bool Valid;
						bool Dirty;
						string Tag;
						Protocol::State *_state;
					public:
						Block(unsigned int size, Protocol::State *state): _size(size){
							this->_state = state;
							this->Valid = false;
							this->Dirty = false;
							this->Tag = "";
						}
						void set_valid(bool valid){
							this->Valid=valid;
						}
						bool get_valid(){
							return this->Valid;
						}
						void set_dirty(bool dirty){
							this->Dirty = dirty;
						}
						bool get_dirty(){
							return this->Dirty;
						}
						void set_tag(string tag){
							this->Tag = tag;
						}
						string get_tag(){
							return this->Tag;
						}
				};

				vector<vector<Block>> *_cache;

				string conHexToBin(string hex);
				unsigned int conBinToUInt(string bin);
				unsigned short powerOf2(unsigned int num);

				string extractTag(string hex);
				unsigned int extractCacheSetIdx(string hex);
				unsigned int extractOffset(string hex);

			public:
				Cache(Bus *bus, unsigned int size, unsigned short assoc,unsigned int blk_size);
				~Cache();
				bool read(string address);
				void fetch(string address);
				void write(string address);
				double get_miss_ratio();
			};
	}
}
#endif