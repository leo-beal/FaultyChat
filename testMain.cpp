#include <iostream>
#include "Utility.hpp"
#include "Algorithms.hpp"

int main(){

    std::cout << std::bitset<8>('a').to_string() << std::endl;

    std::string testString = "ab";

    char* test = (char*)testString.c_str();

    char* test2 = algo::encodeHamming(test, 2);

    std::cout << std::bitset<8>(test2[0]).to_string() << std::endl;
    std::cout << std::bitset<8>(test2[1]).to_string() << std::endl;
    return 0;
}