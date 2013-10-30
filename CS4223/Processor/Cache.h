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
				bool flush(address);
				bool write(address);
				bool read(address);
				class CacheException{
					public:
						CacheException(string arg_type,const string msg): _type(arg_type),_msg(msg) {}
						~CacheException() {}
						string getType(){
							return _type;
						}
						string getMessage(){
							return _type+":"+_msg;
						}
					private:
						string _msg;
						string _type;
				};
			};
	}
}
#endif