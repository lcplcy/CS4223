#include "Cache.h"

namespace CS4223{
	namespace Processor{
		
		Cache::Cache(Bus *sharedBus, unsigned int size,unsigned short assoc,unsigned int blk_size):
		_sharedBus(sharedBus),
		_size(size),
		_assoc(assoc),
		_blk_size(blk_size)
		{
			//Given in assignment paper
			this->_memory_addr_len_bits = 32;
			this->_word_size_bytes = 2;

			this->_num_of_blks = this->_size/this->_blk_size;
			this->_cache_sets = this->_size/this->_assoc;

			this->_N_offset_len_bits = this->powerOf2(this->_blk_size/this->_word_size_bytes); 
			this->_M_cache_set_len_bits = this->powerOf2(this->_cache_sets);
			this->_tag_len_bits = this->_memory_addr_len_bits - (this->_N_offset_len_bits+this->_M_cache_set_len_bits);

			vector<vector<Block>> cache(this->_cache_sets);

			for(unsigned int set=0;set<this->_cache_sets;set+=1){

				//Each cache set have m number of cache blocks
				cache[set] = vector<Block>(this->_assoc,Block(this->_blk_size,new Protocol::State()));
			}

			this->_cache = &cache;
		}

		Cache::~Cache(){

		}

		/**
		*	Returns a 32 bit binary string
		*/
		string Cache::conHexToBin(string hex){
			string sReturn = "";
			for (int i = 0; i < hex.length (); ++i)
			{
				switch (hex [i])
				{
					case '0': sReturn.append ("0000"); break;
					case '1': sReturn.append ("0001"); break;
					case '2': sReturn.append ("0010"); break;
					case '3': sReturn.append ("0011"); break;
					case '4': sReturn.append ("0100"); break;
					case '5': sReturn.append ("0101"); break;
					case '6': sReturn.append ("0110"); break;
					case '7': sReturn.append ("0111"); break;
					case '8': sReturn.append ("1000"); break;
					case '9': sReturn.append ("1001"); break;
					case 'a': sReturn.append ("1010"); break;
					case 'b': sReturn.append ("1011"); break;
					case 'c': sReturn.append ("1100"); break;
					case 'd': sReturn.append ("1101"); break;
					case 'e': sReturn.append ("1110"); break;
					case 'f': sReturn.append ("1111"); break;
				}
			}
			return sReturn;
		}

		unsigned int Cache::conBinToUInt(string bin){
				unsigned int uint = 0;

				for(int i = 0; i < bin.size(); i++)
				{
					if(bin[bin.size() - 1 - i] == '1')
						uint +=  pow(2 , i) * 1;
				}

				return uint; 
		}

		string Cache::extractTag(string address){

			string binary = this->conHexToBin(address);

			string tag = binary.substr(0,this->_tag_len_bits);

			return tag;
		}

		unsigned int Cache::extractCacheSetIdx(string address){
			string binary = this->conHexToBin(address);

			string cache_set = binary.substr(this->_tag_len_bits,this->_M_cache_set_len_bits);

			unsigned int cacheSetIdx=this->conBinToUInt(cache_set);

			return cacheSetIdx;
		}

		unsigned int Cache::extractOffset(string address){
			string binary = this->conHexToBin(address);

			string offset = binary.substr(this->_M_cache_set_len_bits,this->_memory_addr_len_bits);

			unsigned int offsetIdx = this->conBinToUInt(offset);

			return offsetIdx;
		}

		double Cache::get_miss_ratio(){
			unsigned int misses = this->_mem_access - this->_hit;

			return (double) misses/this->_mem_access;
		}
		
		unsigned short Cache::powerOf2(unsigned int num){

			double base = 2
				,ans = 0
				,exp = 1;


			while(1){
				
				ans = pow(base,exp);

				if(ans==num){
					return exp;
				}

				exp+=1;
			}
		}

		bool Cache::read(string address){

			//Add to memory access
			this->_mem_access+=1;

			//Extract block identifier
			string tag = this->extractTag(address);
			unsigned int cacheIdx = this->extractCacheSetIdx(address);
			unsigned int offset = this->extractOffset(address);
			vector<Block> cacheSet = this->_cache->at(cacheIdx);
			string extracted_tag="";
			bool miss=true;

			for(unsigned int sets=0;sets<cacheSet.size();sets++){

				//A Hit
				if(tag==cacheSet[sets].get_tag()&&cacheSet[sets].get_valid()==true){

					miss=false;

				}

			}

			if(miss){

			}else{
				this->_hit+=1;
			}

			return !miss;
		}

		void Cache::fetch(string address){
			//Extract block identifier
			string tag = this->extractTag(address);
			unsigned int cacheIdx = this->extractCacheSetIdx(address);
			unsigned int offset = this->extractOffset(address);
			vector<Block> *cacheSet = &this->_cache->at(cacheIdx);
			
			//Random Replacement 
			unsigned int random_blk_idx = rand()%cacheSet->size()+0;
			Block *selectedBlk = &cacheSet->at(random_blk_idx);
			
			if(selectedBlk->get_dirty()){
				//flush to memory
				Transaction new_transaction(address);
				this->_sharedBus->add_transaction(new_transaction);

			}

			selectedBlk->set_tag(tag);
			selectedBlk->set_valid(true);
			selectedBlk->set_dirty(false);
		}

		void Cache::write(string address){
			
		}
	}
}