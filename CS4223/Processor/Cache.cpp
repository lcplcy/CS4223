#include "Cache.h"

namespace CS4223{

	Processor::Cache::Cache(Bus * const sharedBus,unsigned int size,unsigned short assoc,unsigned int blk_size):
		_sharedBus(sharedBus),
		_size(size),
		_assoc(assoc),
		_blk_size(blk_size)
	{
		unsigned int numOfBlks = this->_size/this->_blk_size;
		unsigned int cacheLines = this->_size/this->_assoc;

		vector<vector<Block>> cache(cacheLines);

		for(unsigned int cacheLine=0;cacheLine<cacheLines;cacheLine+=1){
			cache[cacheLine] = vector<Block>(this->_assoc,Block(this->_blk_size));
		}

		this->_cache = &cache;
	}

	Processor::Cache::~Cache(){

	}

	double Processor::Cache::get_miss_ratio(){
		unsigned int misses = this->_mem_access - this->_hit;

		return (double) misses/this->_mem_access;
	}

}