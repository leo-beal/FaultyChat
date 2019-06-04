#include <iostream>
#include "Utility.hpp"
#include "Algorithms.hpp"

int main(){



    std::string testString = "abcd";

    char* test = (char*)testString.c_str();

    char* test2 = algo::encodeHamming(test, 4);

    char* testDec = algo::decodeHamming(test2, 8);

    std::string toPrint (testDec, 4);

    std::cout << testDec << std::endl;

    return 0;
}