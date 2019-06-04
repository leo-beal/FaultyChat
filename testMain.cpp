#include <iostream>
#include "Utility.hpp"
#include "Algorithms.hpp"

int main(){

    std::cout << std::bitset<8>('b').to_string() << std::endl;

    std::string testString = "ab";

    char* test = (char*)testString.c_str();

    char* test2 = algo::encodeHamming(test, 2);

    std::cout << std::bitset<8>(test2[2]).to_string() << std::endl;
    std::cout << std::bitset<8>(test2[3]).to_string() << std::endl;

    char testDec[2] = {(char)std::bitset<8>("11001100").to_ulong(), (char)std::bitset<8>("01010100").to_ulong()};
    char testDec2[2] = {(char)std::bitset<8>("01001100").to_ulong(), (char)std::bitset<8>("01110100").to_ulong()};

    char* testDec3 = algo::decodeHamming(testDec, 2);
    char* testDec4 = algo::decodeHamming(testDec2, 2);

    std::cout << testDec3[0] << std::endl;
    std::cout << testDec4[0] << std::endl;

    return 0;
}