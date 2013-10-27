#include "Cache.h"

namespace CS4223{

	Processor::Cache::Cache(Bus * const sharedBus,Protocol::Type protocol_type,unsigned int size, unsigned short assoc, unsigned int blk_size):
		_sharedBus(sharedBus),_size(size),_assoc(assoc),_blk_size(blk_size)
	{
		if(protocol_type==Protocol::MESI){
			this->protocol = new CS4223::Protocols::MESI();
		}else if(protocol_type==Protocol::DRAGON){
			this->protocol = new CS4223::Protocols::DRAGON();
		}
	}

	Processor::Cache::~Cache(){

	}

}