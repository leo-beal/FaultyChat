#include "Algorithms.hpp"

char* algo::encodeHamming(char* data, uint64_t& dataLen){
    dataLen = dataLen * 2;
    char* toReturn = new char[dataLen];
    int toRetPos = 0;

    for(int x = 0; x < dataLen / 2; x++){
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
        toRetPos++;

    }//End X loop

    return toReturn;
}

char* algo::decodeHamming(char* data, uint64_t& dataLen){
    dataLen = dataLen / 2;
    char* toReturn = new char[dataLen];
    std::vector<std::string> parts;
    int dataBits[4] = {2, 4, 5, 6};
    int paraty[3] = {0, 1, 3};

    for(int x = 0; x < dataLen * 2; x++){
        std::string byte = std::bitset<8>(data[x]).to_string();
        std::string nybble;
        int toFlip = 0;
        int check[3] = {2, 4, 6};
        for(int y = 0; y < 3; y++){
            int count1s = 0;
            for(int z = 0; z < 3; z++){
                if(byte.at(check[z]) == '1'){
                    count1s++;
                }
            }//End Z loop
            if(count1s%2 == 0 && byte.at(paraty[y]) == '1'){
                toFlip += paraty[y] + 1;
            }
            if(count1s%2 == 1 && byte.at(paraty[y]) == '0'){
                toFlip += paraty[y] + 1;
            }
            if(y == 0){
                check[1]++;
            }
            if(y == 1){
                check[0]+=2;
            }
        }//End Y loop
        if(toFlip == 0){
            toFlip++;
        }
        if(byte.at(toFlip-1) == '1'){
            byte.at(toFlip-1) = '0';
        }else{
            byte.at(toFlip-1) = '1';
        }
        for(int i = 0; i < 4; i++){
            nybble+=byte.at(dataBits[i]);
        }//End I loop
        parts.push_back(nybble);
    }//End X loop

    for(int j = 0; j < parts.size() - 1; j+=2){
        std::string total = parts[j] + parts[j + 1];
        toReturn[j/2] = std::bitset<8>(total).to_ulong();
    }

    return toReturn;
}
