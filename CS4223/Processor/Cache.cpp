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
			this->_word_size_bytes = 2;

			this->_num_of_words = this->_blk_size/this->_word_size_bytes;
			this->_num_of_blks = this->_size/this->_blk_size;
			this->_num_of_cache_sets = this->_num_of_blks/this->_assoc;
			
			vector<Block> cache_set(this->_assoc,Block(this->_blk_size));

			this->_cache = new vector<vector<Block>>(_num_of_cache_sets,cache_set);
		}

		Cache::~Cache(){

		}

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

		double Cache::string_to_double(string bin){
			
			unsigned long long x = 0;
			for (std::string::const_iterator it = bin.begin(); it != bin.end(); ++it)
			{
				x = (x << 1) + (*it - '0');
			}
			double d;
			memcpy(&d, &x, 8);
			return d;
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

		Cache::Address Cache::translate_address(string raw_address){

			string bin_address = this->conHexToBin(raw_address);

			unsigned int offset_bits_len = this->powerOf2(this->_num_of_words),
						 cache_set_bits_len = this->powerOf2(this->_num_of_cache_sets),
						 tag_bits_len = 32 - cache_set_bits_len - offset_bits_len;

			string offset_bits = bin_address.substr(0,offset_bits_len-1),
				   cache_set_bits = bin_address.substr(offset_bits_len,cache_set_bits_len-1),
				   tag_bits = bin_address.substr(cache_set_bits_len,tag_bits_len);

			unsigned int offset = std::bitset<32>(offset_bits).to_ulong(),
						 cache_set = std::bitset<32>(cache_set_bits).to_ulong();

			Cache::Address translated = {"",0,0};

			translated.tag = tag_bits;
			translated.cache_set_idx = cache_set;
			translated.offset = offset;

			return translated;
		}

		/* Analytics */

		double Cache::get_miss_ratio(){
			unsigned int misses = this->_cache_access - this->_hit;

			return (double) misses/this->_cache_access;
		}

		/* Operations */

		bool Cache::read(string address){

			this->_cache_access+=1;

			//Extract block identifier
			Cache::Address translated_address = this->translate_address(address);
			vector<Block> cacheSet = this->_cache->at((unsigned int)translated_address.cache_set_idx);
			bool miss=true;

			for(unsigned int sets=0;sets<cacheSet.size();sets++){

				//A Hit
				if(translated_address.tag==cacheSet[sets].get_tag() && cacheSet[sets].get_valid()==true){
					miss=false;
					//No Bus Transaction
				}

			}

            //not miss -> READ HIT
			if(!miss){
				this->_hit+=1;
			}else{
				//Read cache blk from memory via the bus
				CS4223::Processor::Transaction new_transaction(address);
				this->_sharedBus->add_transaction(Bus::Type::BusRd,new_transaction);

				//Extract block identifier
				Cache::Address translated_address = this->translate_address(address);
				vector<Block> *cacheSet = &this->_cache->at(translated_address.cache_set_idx);
			
				//Random Replacement 
				unsigned int random_blk_idx = rand()%cacheSet->size()+0;
				Block *selectedBlk = &cacheSet->at(random_blk_idx);
			
				if(selectedBlk->get_dirty()){
					//if the block is dirty flush to memory => Assume free due to write buffer
					Transaction new_transaction(address);
					this->_sharedBus->add_transaction(Bus::Type::BusWr,new_transaction);
				}

				//Write Allocate to the Cache Block
				selectedBlk->set_tag(translated_address.tag);
				selectedBlk->set_valid(true);
				selectedBlk->set_dirty(false);
			}

			return !miss;
		}

		void Cache::write(string address){

			this->_cache_access+=1;

			//Extract block identifier
			Cache::Address translated_address = this->translate_address(address);
			vector<Block> *cacheSet = &this->_cache->at((unsigned int)translated_address.cache_set_idx);
			bool miss=true;

			for(unsigned int sets=0;sets<cacheSet->size();sets++){

				Block *scanningBlk = &cacheSet->at(sets);

				//A Write Hit
				if(translated_address.tag==scanningBlk->get_tag()&&scanningBlk->get_valid()==true){
					miss=false;
					//No Bus Transaction
					scanningBlk->set_dirty(true);
                    scanningBlk->set_state("M")

					break;
				}
			}

			//Write Miss
			if(miss){
				//Read cache blk from memory via the bus
				CS4223::Processor::Transaction new_transaction(address);
				this->_sharedBus->add_transaction(Bus::Type::BusRd,new_transaction);

				//Extract block identifier
				Cache::Address translated_address = this->translate_address(address);
				vector<Block> *cacheSet = &this->_cache->at(translated_address.cache_set_idx);
			
				//Random Replacement 
				unsigned int random_blk_idx = rand()%cacheSet->size()+0;
				Block *selectedBlk = &cacheSet->at(random_blk_idx);
			
				if(selectedBlk->get_dirty()){
					//if the block is dirty flush to memory => Assume free due to write buffer
					Transaction new_transaction(address);
					this->_sharedBus->add_transaction(Bus::Type::BusWr,new_transaction);
				}

				//Write Allocate to the Cache Block
				selectedBlk->set_tag(translated_address.tag);
				selectedBlk->set_valid(true);
				selectedBlk->set_dirty(true);	//Because you wrote the data into this cache block
			}
		}

		unsigned int Cache::get_total_cache_access(){
			return this->_cache_access;
		}
	}
}