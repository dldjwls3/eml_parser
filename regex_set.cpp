#include "regex_set.h"
#include "decoder.h"
#include <iostream>

#include <QDebug>
#include <QString>
using namespace std;

regex_set parse = regex_set();

int regex_set::search(string input, int a, string& s){
    regex_search(input, this->result, this->pattern[a]);
    if(!this->result.empty()){
        s = this->result[0];
        return 1;
    }

    s = "";
    return 0;
}



void chunk_and_decode(string input, string& s){

    vector<string> split;

    char delimiter = '?';
    size_t next_pos = input.find(delimiter, 0);
    size_t last_pos = next_pos + 1;

    while((next_pos = input.find(delimiter, last_pos)) != string::npos){
        split.push_back(input.substr(last_pos, next_pos - last_pos));
        last_pos = next_pos + 1;
    }


    s = split.back(); split.pop_back();

    //char *enc_ptr, *buf, *buf_ptr;
    while(split.size() > 0){
        string option = split.back(); split.pop_back();

        /* make sure option is uppercase */
        transform(option.begin(), option.end(), option.begin(), ::toupper);

        /* decode */
        if(option == "B"){
            s = decode.decode(s, base64);
            cout << "base64 decode" << endl;
            cout << s << endl;
        }
        else if(option == "EUC-KR") {
            s = decode.decode(s, euc_kr);
        }
        else if(option == "UTF-8"){
            s = decode.decode(s, utf_8);

        }
        else if(option == "Q") {
            s = decode.decode(s, qp);
        }

    }

}


void regex_set::parse_and_decode(string input, string& s){
    size_t prefix = 0;
    size_t surfix = 0;

    string check;
    if(!search(input, encode, check)){
        s = input;
        return;
    }

    while((prefix = input.find("=?", surfix)) != string::npos && (surfix = input.find("?=", surfix + 1))!= string::npos) {
        cout << prefix << " " << surfix << endl;
        string chunk = input.substr(prefix, surfix - prefix + 2);
        cout << "chunk: " << chunk << endl;

        string temp;
        chunk_and_decode(chunk, temp);
        s += temp;
    }

}

