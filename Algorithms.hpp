#ifndef FAULTYDISK_ALGORITHMS_HPP
#define FAULTYDISK_ALGORITHMS_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bitset>
#include <cstring>

namespace algo{
    char* encodeHamming (char* data, int dataLen);

    char* decodeHamming (char* data, int dataLen);
}

#endif //FAULTYDISK_ALGORITHMS_HPP
