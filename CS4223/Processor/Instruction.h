#include <string>

using namespace std;

namespace CS4223{
	namespace Processor{
		class Instruction{
					private:
						unsigned short _label;
						string _value;
					public:
						Instruction(unsigned short label, string value);
						unsigned short GetLabel();
						string GetValue();
				
		};
	}
}