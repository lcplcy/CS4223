#include "Block.h"

namespace CS4223{
	namespace Processor{

		Block::Block(unsigned int size): _size(size){
				this->Tag = "";
		}

		void Block::set_tag(string tag){
			this->Tag = tag;
		}

		string Block::get_tag(){
			return this->Tag;
		}
	}
}