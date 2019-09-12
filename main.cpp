#include <iostream>
#include <string>
#include "mapper.hpp"

bool isNumber(std::string);
void reset();
void cyan();

int main(int argc, char** argv)
{
try{
    if (argc != 5) {
throw "\nNot enough arguments, please try again! \n\nEX: 'stations file name' 'connections file name' 'start ID/Name station ' 'distination ID/Name station'.\n\n";
         }
}
catch (char const* msg_error_arguments) {
            cyan();
            std::cerr << msg_error_arguments << std::endl;
            reset();
            exit(1);
        }
    bool arg3, arg4;
    arg3 = isNumber(argv[3]);
    arg4 = isNumber(argv[4]);

    if (arg3 && arg4) {
        uint64_t start_id = std::stoi(argv[3]);
        uint64_t end_id = std::stoi(argv[4]);
        try {
            mapper station(argv[1], argv[2], start_id, end_id);
        }
        catch (char const* msg_error_start) {
            cyan(); 
            std::cerr << msg_error_start << std::endl;
            reset(); 
        }
    }
    else {

        try {
            mapper station(argv[1], argv[2], argv[3], argv[4]);
        }
        catch (char const* msg_error_start) {
            cyan();
            std::cerr << msg_error_start << std::endl;
            reset();
        }
    }

    return 0;
}

bool isNumber(std::string s)
{
    for (unsigned int i = 0; i < s.length(); i++)
        if (isdigit(s[i]) == false)
            return false;
    return true;
}

void reset() { printf("\033[0m"); }
void cyan(){printf("\033[1;36m");}


