#include <iostream>
#include "Utility.hpp"
#include "Algorithms.hpp"

int main(){



    std::string testString = "abcd";

    char* test = (char*)testString.c_str();

    uint64_t totalLength;

    unsigned char* test2 = util::createMessage(654852, 4, test, totalLength);

    std::cout << totalLength << std::endl;
    
    char* test4 = algo::encodeHamming((char*)test2, totalLength);

    std::cout << totalLength << std::endl;

    uint64_t uuid;
    uint32_t length = 0;
    
    char* test5 = algo::decodeHamming(test4, totalLength);

    std::cout << totalLength << std::endl;

    unsigned char* test3 = util::parseMessage((char*)test5, uuid, length, totalLength);

    std::cout << totalLength << std::endl;

    std::string test6((char*)test3, length);

    std::cout << "uuid: " << uuid << std::endl
    << "length: " << length << std::endl
    << "total Length: " << totalLength << std::endl
    << "String: " << test6 << std::endl;

    return 0;
}