#include "Instruction.h"

namespace CS4223{
	namespace Processor{
		Instruction::Instruction(unsigned short label, string value):_label(label),_value(value){

		}

		unsigned short Instruction::GetLabel(){
			return this->_label;
		}

		string Instruction::GetValue(){
			return this->_value;
		}
	}
}