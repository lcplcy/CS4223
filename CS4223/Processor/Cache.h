#ifndef CACHE_H
#define CACHE_H

#include <vector>
#include <cmath>
#include <stdlib.h>
#include <bitset>

#include "../Bus.h"
#include "../Protocol.h"
#include "Transaction.h"
#include "Block.h"

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
				unsigned int _num_of_cache_sets;
				unsigned int _num_of_blks;
				unsigned int _num_of_words;
				unsigned short _word_size_bytes;

				//Analytics
				unsigned int _cache_access;
				unsigned int _hit;

				class Block{
					private:
						unsigned int _size;
						bool Valid;
						bool Dirty;
						string Tag;
					public:
						Block(unsigned int size): _size(size){
							//MESI
							this->Valid = false;
							//DRAGON
							this->Dirty = false;
							//I assume TAG is to indicate cache block state?
							this->Tag = "";
							this->State = "";
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

						void set_state(string state){
							this->State = state;
						}
						string get_state(){
							return this->State;
						}
				};

				struct Address{
					string tag;
					unsigned int cache_set_idx;
					unsigned int offset;
				};

				vector<vector<Block>> *_cache;

				string Cache::conHexToBin(string hex);
				double Cache::string_to_double(string bin);
				unsigned short Cache::powerOf2(unsigned int num);
			public:
				Cache(Bus *sharedBus, unsigned int size,unsigned short assoc,unsigned int blk_size);
				~Cache();
				
				struct Address{
					string tag;
					unsigned int cache_set_idx;
					unsigned int offset;
				};
				vector<Block>* get_cache_set(unsigned int cache_set_idx);
				Address translate_address(string raw_address);
				double get_miss_ratio();
				unsigned int get_total_cache_hit();
				unsigned int get_total_cache_access();
				unsigned short Cache::get_associativity();
				unsigned int Cache::get_num_of_cache_sets();
				void inc_cache_access();
				void inc_hit();
			};
	}
}
#endif