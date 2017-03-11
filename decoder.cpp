#include "decoder.h"
#include "regex_set.h"
#include "base64.h"
#include <vector>
#include <string>
#include <QString>
#include <iostream>
char c2i(char in){
    if('0' <= in && in <= '9'){
        return in - '0';
    }
    else if('a' <= in && in <= 'f'){
        return in - 'a' + 10;
    }
    else if('A' <= in && in <= 'F'){
        return in - 'A' + 10;
    }
    else
        return -1;
}

string decoder::decode(string input, int encode_way){

    string ret = "";
    if(encode_way == base64){
        ret = base64_decode(input);
    }
    else if(encode_way == euc_kr) {
        ret = input;
    }
    else if(encode_way == utf_8){
        QString temp = QString::fromStdString(input.c_str());
        ret = temp.toLocal8Bit();
    }
    else if(encode_way = qp) {
        for(int i = 0; i < input.length(); i++){
            if(input[i] == '='){
                char high = c2i(input[i + 1]);
                char row = c2i(input[i + 2]);
                char c = (high << 4) | row;
                ret += c;
                i = i + 2;
                continue;
            }
            ret += input[i];
        }
    }

    return ret;
}

decoder decode = decoder();
