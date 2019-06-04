#include "Algorithms.hpp"

std::vector <char*> algo::vectorize(char *memblock, const int &segs, const int &lenRem) {
    std::vector<char*> vect;
    for(int x = 0; x < segs; x++){
        if(x < segs - 1) {
            vect.push_back(new char[10]);
            memcpy(vect[x], memblock + x * 10, 10);
        }else{
            if(lenRem > 0){
                vect.push_back(new char[lenRem]);
                memcpy(vect[x], memblock + x * 10, lenRem);
            }else{
                vect.push_back(new char[10]);
                memcpy(vect[x], memblock + x * 10, 10);
            }
        }
    }
    return vect;
}

char* algo::devectorize(const std::vector<char *> &vect, const int &lenRem) {
    char* memblock = new char[((vect.size() - 1) * 10) + lenRem];
    for(int x = 0; x < vect.size() - 1; x++){
        memcpy(memblock + x * 10, vect[x], 10);
    }
    if(lenRem > 0){
        memcpy(memblock + (vect.size() - 1) * 10, vect[vect.size() - 1], lenRem);
    }else{
        memcpy(memblock + (vect.size() - 1) * 10, vect[vect.size() - 1], 10);
    }
    return memblock;
}

char* algo::encodeHamming(char* data, int dataLen){
    char* toReturn = new char[dataLen * 2];
    int toRetPos = 0;

    for(int x = 0; x < dataLen; x++){
        std::string bits = std::bitset<8>(data[x]).to_string();
        std::string firstHalf = bits.substr(0, 4);
        std::string secondHalf = bits.substr(4, 4);
        std::string firstByte;
        std::string secondByte;
        for(int y = 0; y < 2; y++){

            int check[3] = {0, 1, 3};
            for(int z = 0; z < 3; z++){
                int count1s = 0;
                for(int a = 0; a < 3; a++){
                    if(y == 0){
                        if(firstHalf.at(check[a]) == '1'){
                            count1s++;
                        }
                    }
                    else{
                        if(secondHalf.at(check[a]) == '1'){
                            count1s++;
                        }
                    }
                }//End A loop

                if(y == 0){
                    if(count1s%2 == 0){
                        firstByte += "0";
                    }else{
                        firstByte += "1";
                    }
                }else{
                    if(count1s%2 == 0){
                        secondByte += "0";
                    }else{
                        secondByte += "1";
                    }
                }
                if(z == 0){
                    check[1]++;
                }
                if(z == 1){
                    check[0]++;
                }

            }//End Z loop

        }//End Y loop

        firstByte.insert(2, firstHalf, 0, 1);
        firstByte += firstHalf.substr(1, 3) + "0";
        secondByte.insert(2, secondHalf, 0, 1);
        secondByte += secondHalf.substr(1, 3) + "0";
        char first = std::bitset<8>(firstByte).to_ulong();
        char second = std::bitset<8>(secondByte).to_ulong();
        toReturn[toRetPos] = first;
        toRetPos++;
        toReturn[toRetPos] = second;

    }//End X loop

    return toReturn;
}

char* algo::decodeHamming(const std::vector<char *> &vect, int dataLen){
    bool des = false;
    for(int x = 0; x < vect.size() - 1; x++){
        for(int y = 0; y < dataLen; y++){
            char temp = vect[x][y] ^ vect[x + 1][y];
            if((int)temp != 0){
                des = true;
            }
        }
    }
    return (char*)'a';
}
