#ifndef FAULTYDISK_ALGORITHMS_HPP
#define FAULTYDISK_ALGORITHMS_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bitset>
#include <cstring>

namespace algo{
    std::vector <char*> vectorize(char *memblock, const int &segs, const int &lenRem);

    char* encodeHamming (char* data, uint64_t& dataLen);

    char* decodeHamming (char* data, uint64_t& dataLen);
}

#endif //FAULTYDISK_ALGORITHMS_HPP
