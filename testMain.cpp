#include <iostream>
#include "Utility.hpp"
#include "Algorithms.hpp"

int main(){



    std::string testString = "abcd";

    char* test = (char*)testString.c_str();

    uint64_t totalLength;

    unsigned char* test2 = util::createMessage(654852, 4, test, totalLength);

    uint64_t uuid;
    uint32_t length = 0;

    unsigned char* test3 = util::parseMessage((char*)test2, uuid, length, totalLength);

    std::string test4((char*)test3, length);

    std::cout << "uuid: " << uuid << std::endl
    << "length: " << length << std::endl
    << "total Length: " << totalLength << std::endl
    << "String: " << test4 << std::endl;

    return 0;
}