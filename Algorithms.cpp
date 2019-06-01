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

char* algo::vote(const std::vector<char *> &vect, int dataLen) {
    char* toReturn = new char[dataLen];
    std::vector<std::string> voters;
    for(int x = 0; x < vect.size(); x++){
        std::string tempVoter = "";
        for(int y = 0; y < dataLen; y++){
            tempVoter += std::bitset<8>(vect[x][y]).to_string();
        }
        voters.push_back(tempVoter);
    }

    std::string voted = "";
    for(int x = 0; x < voters[0].length(); x++){
        int num1 = 0;
        int num0 = 0;
        for(int y = 0; y < voters.size(); y++){
            if(voters[y].at(x) == '1'){
                num1++;
            }else{
                num0++;
            }
        }
        if(num1 > num0){
            voted += "1";
        }else{
            voted += "0";
        }
    }

    for(int x = 0; x < voted.length() / 8; x++){
        unsigned long ret = std::bitset<8>(voted.substr(0 + x*8, 8)).to_ulong();
        toReturn[x] = (char)ret;
    }
    //toReturn[len] = '\0';
    return toReturn;
}

bool algo::discrep(const std::vector<char *> &vect, int dataLen) {
    bool des = false;
    for(int x = 0; x < vect.size() - 1; x++){
        for(int y = 0; y < dataLen; y++){
            char temp = vect[x][y] ^ vect[x + 1][y];
            if((int)temp != 0){
                des = true;
            }
        }
    }
    return des;
}
