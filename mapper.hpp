#pragma once

#include "Generic_mapper.hpp"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <iterator>
#include <queue>
#include <algorithm>
#include <functional>
#include <exception>
#include <chrono> 

class mapper: public travel::Generic_mapper {
   public: 
    
      mapper(const std::string & _filename1,const std::string & , uint64_t , uint64_t ); 
      mapper(const std::string & _filename1,const std::string & ,const std::string&, const std::string&); 
     ~mapper();
   protected:

   void read_stations(const std::string & _filename) override;
   void read_connections(const std::string & _filename) override;
   std::vector < std::pair < uint64_t,uint64_t > > compute_travel(uint64_t _start, uint64_t _end) override;
   std::vector < std::pair < uint64_t,uint64_t > > compute_and_display_travel(uint64_t _start, uint64_t _end) override;
   int levenshtein_distance(const std::string ,const std::string );
   void disp_time(uint64_t);
   
   void green() {printf("\033[1;32m");}
   void reset(){printf("\033[0m");}
   void blue(){printf("\033[1;33m");}
   void cyan(){printf("\033[1;36m");}
   void purple(){printf("\033[1;35m");}
    		
	     		
	    

};
