#ifndef CS4223_H
#define CS4223_H

#include "stdafx.h"

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <fstream>
#include <algorithm>
#include <thread>

#include "Protocol.h"
#include "Processor.h"
#include "Bus.h"

using namespace std;

namespace CS4223{
	class Core {
		private:
			Protocol::Type protocol_type;
			string trace_file_name;
			string output_file_name;
			unsigned short num_of_processors, 
						   max_num_of_processor,
						   associativity,
						   block_size;
		    unsigned int cache_size;
			double clock;
			bool isPowerOf2(unsigned int);
			void readFile(string rel_dir,vector<Processor::Instruction*> *instructions,unsigned int *instr_count);
			vector<CS4223::Processor::Core> processors;
			Bus *sharedBus;
		public:
			Core(const int arg_count,char* main_arg[]);
			~Core();
			void initialise();
			void run();
			void clean_up();
			void analyse();
			class ArgumentException : exception{
				public:
						ArgumentException(const string arg_type,const string msg) : _type(arg_type),_msg(msg) {}
						~ArgumentException( ) {}
						string getMessage( ){
							return _type + " : " + _msg;
						}
				private:
					string _msg;
					string _type;
			};
			class InitialisationException : exception{
				public:
						InitialisationException(const string arg_type,const string msg) : _type(arg_type),_msg(msg) {}
						~InitialisationException( ) {}
						string getMessage( ){
							return _type + " : " + _msg;
						}
				private:
					string _msg;
					string _type;
			};

	};
}
#endif