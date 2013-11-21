// CS4223.cpp : Defines the entry point for the console application.
//
#include "CS4223.h"

namespace CS4223{

	Core::Core(const int arg_count, char* main_arg[]){
		cout << "Reading configurations" << endl;

		string raw_protocol_type = "";

		if(arg_count<3){

			//raw_protocol_type = "NONE";
			//this->num_of_processors = 1;
			//this->trace_file_name = "WEATHER";
			this->num_of_processors = 4;
			this->cache_size = 4096;
			this->associativity = 1;
			this->block_size = 64;

		}else{
			for(int i = 0; i < arg_count; i++) {

				raw_protocol_type = main_arg[1];
				this->trace_file_name = main_arg[2];
				this->num_of_processors = (unsigned short) strtoul(main_arg[3], NULL, 0);
				this->cache_size = (unsigned int) strtoul(main_arg[4], NULL, 0);
				this->associativity  = (unsigned short) strtoul(main_arg[5], NULL, 0);
				this->block_size = (unsigned short) strtoul(main_arg[6], NULL, 0);
			}
		}

		// Verification & Assignment

		if(raw_protocol_type=="MESI"){
			this->protocol_type = Protocol::MESI;
		}else if(raw_protocol_type=="DRAGON"){
			this->protocol_type = Protocol::DRAGON;
		}else if(raw_protocol_type=="NONE"){
			this->protocol_type = Protocol::NONE;
		}
		else{
			throw ArgumentException("Protocol","Input protocol not avaliable");
		}

		if(	this->trace_file_name!="WEATHER"&&
			this->trace_file_name!="FFT"){
			
			throw ArgumentException("TraceFile","Invalid Trace File.");
		}

		this->max_num_of_processor = 16;

		if(this->num_of_processors>max_num_of_processor){
			throw ArgumentException("Processors","Number of processors exceed maximum allowed number");
		}

		if(!this->isPowerOf2(this->cache_size)){
			throw ArgumentException("Cache","Cache size is not power of 2");
		}
	
		if(!this->isPowerOf2(this->block_size)){
			throw ArgumentException("BlockSize","Block size is not power of 2");
		}
	}

	Core::~Core(){
		 cout << "System shutdown" << endl;

	}

	bool Core::isPowerOf2(unsigned int num){
	 return (
	   num == 1 || num == 2 || num == 4 || num == 8 || num == 16 || num == 32 ||
	   num == 64 || num == 128 || num == 256 || num == 512 || num == 1024 ||
	   num == 2048 || num == 4096 || num == 8192 || num == 16384 ||
	   num == 32768 || num == 65536 || num == 131072 || num == 262144 ||
	   num == 524288 || num == 1048576 || num == 2097152 ||
	   num == 4194304 || num == 8388608 || num == 16777216 ||
	   num == 33554432 || num == 67108864 || num == 134217728 ||
	   num == 268435456 || num == 536870912 || num == 1073741824 ||
	   num == 2147483648);
	}

	void Core::initialise(){

		cout << "Initialising" << endl;

		//Initialsation

		string format = ".PRG";
		string folder_name = this->trace_file_name;
		std::transform(folder_name.begin(), folder_name.end(),folder_name.begin(),::tolower);
		folder_name[0] = toupper(folder_name[0]);

		//Core setup
		this->sharedBus = new Bus();
		vector<std::thread> threads(this->num_of_processors);
		this->clock = 0;

		//Processor setup 
		vector<vector<Processor::Instruction*>*> instruction_set;
		vector<unsigned int*> all_proc_instruction_count;

		vector<Processor::Instruction*> *instructions;
		unsigned int *instruction_count;

		for(unsigned short processor=0;processor<this->num_of_processors;processor++){	

			string rel_dir = "../Benchmark/"+folder_name+std::to_string(this->num_of_processors)+"/"+this->trace_file_name+ std::to_string(processor+1)+format;

			cout << "Loading instructions into processor no. " + std::to_string(processor+1) << endl;

			instructions = new vector<Processor::Instruction*>();
			instructions->clear();
			instruction_count = new unsigned int();
			*instruction_count = 0;

			instruction_set.push_back(instructions);
			all_proc_instruction_count.push_back(instruction_count);
			
			threads[processor] = std::thread(&CS4223::Core::readFile,this,rel_dir,instructions,instruction_count);	
		}

		for(unsigned short processor=0;processor<this->num_of_processors;processor++){	

			//wait for all threads to complete
			threads[processor].join();

			processors.push_back(CS4223::Processor::Core(instruction_set.at(processor),processor,*all_proc_instruction_count.at(processor),this->sharedBus,this->protocol_type,this->cache_size,this->associativity,this->block_size));

			if(processors[processor].initialise()){
				throw InitialisationException("Processor","Individual processor initialisation failed");
			}
		}
	}

	void Core::readFile(string rel_dir,vector<Processor::Instruction*> *instructions,unsigned int *instr_count){
		
		ifstream fp(rel_dir);
		string result = "",
			   value = "";
		unsigned int label=0;
		Processor::Instruction *instr_ptr = NULL;

		cout << "From relative directory : " + rel_dir << endl;

		try{
			if(fp.is_open()){
					while (std::getline(fp, result)) {

						label = std::stoi(result.substr(0, result.find(" ")));
						value = result.substr(result.find(" "),result.length());
	
						instr_ptr = new Processor::Instruction(label,value);
								
						instructions->push_back(instr_ptr);

						*instr_count+=1;
					}
			}else{
				throw InitialisationException("FileRead","Trace file read failed");
			}

			fp.close();

		}catch (bad_alloc& ba)
		{
			cout << "bad_alloc caught: " << ba.what() << endl;
		}
	};

	void Core::run(){
	    cout  << "Begin execution" << endl;

		unsigned short completed_processors = 0;

		while(1){

			CS4223::Processor::Transaction *bus_transaction = NULL; 

			//A single clock cycle

			for(unsigned short processor=0;processor<this->num_of_processors;processor++){	

				this->processors[processor].next_instr();

				if(bus_transaction!=NULL){
					this->processors[processor].listen(bus_transaction->get_address());
				}

				if(this->processors[processor].get_state()==CS4223::Processor::Core::State::complete
				&& this->processors[processor].get_state()!=CS4223::Processor::Core::State::cleaned_up){

					//Only clean up one processor once 
					cout << "Processor no. " + std::to_string(processor) + " ended execution" << endl;
					this->processors[processor].clean_up();
					completed_processors+=1;
				}
			}

			bus_transaction = this->sharedBus->next_transaction();

			if(completed_processors==this->num_of_processors){
				// All processor completed execution

				// Complete all the bus transactions
				while(1){

					this->clock+=1;

					CS4223::Processor::Transaction *bus_transaction = this->sharedBus->next_transaction();

					if(bus_transaction==NULL){
						break;
					}
				
					for(unsigned short processor=0;processor<this->num_of_processors;processor++){	
						this->processors[processor].listen(bus_transaction->get_address());
					}
				}

				break;
			}else{
				// Inc the clock here
				this->clock+=1;
			}
		}
	}

	void Core::clean_up(){

		cout << "System cleanup" << endl;

	}

	void Core::analyse(){

		cout << "Total Number of Clk Cycle : " << this->clock << endl;

		//All private members should not be changed from this point onwards
		cout << "1. Data Cache Miss Ratio " << endl;

		for(unsigned short processor=0;processor<this->num_of_processors;processor++){	
			double miss_ratio  = this->processors[processor].get_cache_miss_ratio();
			unsigned int hit = this->processors[processor].get_total_cache_hit();
			unsigned int   access = this->processors[processor].get_total_cache_access();
			cout << "Processor" << " [" << processor << "]" << " Miss Ratio : " << miss_ratio << endl;
			cout << "Processor" << " [" << processor << "]" << " Cache Hit : " << hit  << endl;
			cout << "Processor" << " [" << processor << "]" << " Total Access : " << access  << endl;
		}

		cout << "2. Address & Data Traffic " << endl;
		
		double total_address = this->sharedBus->get_total_address_traffic();
		double total_data = this->sharedBus->get_total_data_traffic();

		for(unsigned short processor=0;processor<this->num_of_processors;processor++){	
			
			double address_per_cache_access  = (double) total_address/this->processors[processor].get_total_cache_access();
			cout << "Processor Address" << "[" << processor << "]" << " : " <<  address_per_cache_access << endl;

			double data_per_cache_access  = (double) total_data/this->processors[processor].get_total_cache_access();
			cout << "Processor Data" << "["  << processor << "]" << " : " <<  data_per_cache_access << endl;
		}

		cout << "3. Execution Cycles" << endl;

		for(unsigned short processor=0;processor<this->num_of_processors;processor++){	
			
			cout << "Processor Cycles" << "[" << processor << "]" << ":" << this->processors[processor].get_processor_execution_cycles() << endl;
		}
	}
}

int main(int argc, char* argv[])
{
	try{
		CS4223::Core system(argc, argv);

		system.initialise();

		system.run();

		system.clean_up();

		system.analyse();
	}
	catch(CS4223::Core::ArgumentException argEx){
		cout << argEx.getMessage();
	}
	catch(CS4223::Core::InitialisationException initEx){
		cout << initEx.getMessage();
	}

	system("pause");

	return 0;
}

