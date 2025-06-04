#include<iostream>
#include<cstdint>
#include<vector>
#include<string>

using binary = std::vector<uint64_t>;

struct divby2_results{
    std::string quocient;
    int carry = 0;
};

//change the bit in index postion to value
void set_bin(binary& bin, int index, bool value= 0){
    int chunk = index/64;
    int bit = index%64;
    if(chunk > (int)bin.size() -1) bin.resize(chunk+1);
    if(!value) bin[chunk] = ~(1ULL << bit) & bin[chunk]; //value ==0
    else bin[chunk] = (1ULL << bit) | bin[chunk]; //value == 1
}

void printb(const binary bin){
    bool started = 0;//to print the first significaly bit
    //each chunk of binary
    for(int chunk = bin.size()-1; chunk >= 0; chunk--){
        //each bit of the chunk
        for(int index = 63; index >= 0; index--){
            bool bit = (bin[chunk] >> index) & 1;
            if(bit) started = 1;
            if(started) std::cout << bit;
        }
    }
    if(!started) std::cout << "0";
    std::cout << "\n";
}

int bin_size(const binary bin){
    for(int chunk = bin.size()-1; chunk>=0; chunk--){

        if(bin[chunk] != 0){
            //__builtin_clzll conta quantos zeros há no chunk não significativos
            return 64*chunk + (64- __builtin_clzll(bin[chunk]));
        }
    }
    return 0;
}

//plans: divide-and-conquer algorithm
divby2_results divideby2(std::string& decimal){
    std::string result;
    result.reserve(decimal.size()); // Evita realocações
    int carry = 0;

    //Buscar o primeiro bit significativo
    bool started = false; 
    bool nonzero = false; 
    for(int idx = 0;idx < size(decimal); idx++){
        int digit = decimal[idx]-48; //48 == '0'
        if(!started and digit!=0){
            started = true;
        }
        if(started){
            int value = 10*carry + digit;
            int quocient = value/2;
            carry = value%2;
            if(!nonzero and quocient>0) nonzero = true;
            if(nonzero) result += '0' + quocient;
        }
    }
    if(result == "") result = "0";
    return {result, carry};
}

//convert string to binary
binary dectobin(std::string decimal){
    divby2_results div;
    div.quocient = decimal;

    binary bin;
    set_bin(bin, 0, 0);
    int index = 0;
    while(div.quocient != "0"){
        div = divideby2(div.quocient);
        if(div.carry) set_bin(bin, index, 1);
        index++;
    }
    return bin;
}

//define bin << n
binary operator<<(binary &bin,const int bits){
    //error tratament
    if(bits <= 0) return bin;
    if(bin.empty()) return bin;

    const int full_chunks = bits / 64; //total full chunks shifted
    const int shift = bits % 64; //other bits shifted

    //resize bin size
    bin.resize(bin.size()+full_chunks + (shift > 0 ? 1 : 0), 0);

    for(int chunk = bin.size()-1-full_chunks; chunk >=0; --chunk){

        if(shift >0 and chunk+full_chunks+1 < bin.size() and bin[chunk] >> (64-shift) != 0){
            bin[chunk+full_chunks+1] |= bin[chunk] >> 64-shift;
        }
        bin[chunk+full_chunks] = bin[chunk] << shift;
    }
    std::fill(bin.begin(), bin.begin()+full_chunks,0);
    return bin;
}

binary& operator<<=(binary &bin, const int bits){
    bin = bin << bits;
    return bin;
}

//ignore this part.

/* binary add(binary bin1, binary bin2){
    
    for(int chunk = bin1.size()-1; chunk >=0; chunk--){
        
    }
    return bin;
} */

/* std::string bintodec(binary bin){
    int bsize = bin_size(bin);
    the maximum of byte we will need
} */

int main(){
    return 0;
}