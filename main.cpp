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

divby2_results divideby2(std::string& decimal){
    std::string result;
    int carry = 0;
    bool started = false; //ignore zeros to left
    bool nonzero = false; //ignore zeros to left of the result
    for(int idx = 0;idx < size(decimal); idx++){
        int digit = decimal[idx]-48; //48 == '0
        if(!started and digit!=0){
            started = true;
        }
        if(started){
            int value = 10*carry + digit;
            int quocient = value/2;
            carry = value%2;
            if(!nonzero and quocient>0) nonzero = true;
            if(nonzero) result += std::to_string(quocient);
        }
    }
    if(result == "") result = "0";
    return {result, carry};
}

//convert string to binary
binary dtob(std::string decimal){
    divby2_results div;
    div.quocient = decimal;

    binary bin;
    int index = 0;
    while(div.quocient != "0"){
        div = divideby2(div.quocient);
        if(div.carry) set_bin(bin, index, 1);
        index++;
    }
    return bin;
}

int main(){
    using std::cout;
    std::string num = " ";
    while(num != "0"){
        std::cin >> num;
        binary bin = dtob(num);
        printb(bin);
    }
    
    
    return 0;
}