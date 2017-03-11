#include "content_parser.h"
#include "regex_set.h"
#include "decoder.h"
#include "md5.h"
#include <vector>
#include <list>
#include <iostream>

#include <QDebug>
using namespace std;

struct content_param{
    string type;
    string charset;
    string boundary;
    string name;
    string disposition;
    string filename;
    string id;
    string transfer_encoding;
};

struct content_param content_type_parse(string& content_type, string& transfer, string& content_disposition, string& content_id){
    string type;
    if(!parse.search(content_type, pcontent_type, type)){
//        cout << ":  there is no type" << endl;
    }

    string charset;
    if(!parse.search(content_type, pcharset, charset)){
//        cout << ":  there is no charset" << endl;
    }
    else{
        size_t pos = charset.find("=") + 1;
        size_t last = charset.length();

        if(charset.at(last - 1) == ';'){
            last--;
        }
        if(charset.at(pos) == '\"'){
            pos++;
            last--;
        }

        charset = charset.substr(pos , last - pos);
        transform(charset.begin(), charset.end(), charset.begin(), ::toupper);
    }

    string bound;
    if(!parse.search(content_type, pboundary, bound)){
//        cout << ":  there is no boundary" << endl;
    }
    else{
        size_t pos = bound.find("=") + 1;
        size_t last = bound.length();

        if(bound.at(last - 1) == ';'){
            last--;
        }
        if(bound.at(pos) == '\"'){
            pos++;
            last--;
        }

        bound = bound.substr(pos, last - pos);
    }

    string name;
    if(!parse.search(content_type, pname, name)){
//        cout << ":  there is no name" << endl;
    }
    else{
        size_t pos = name.find("=") + 1;
        size_t last = name.length();

        if(name.at(last - 1) == ';'){
            last--;
        }
        if(name.at(pos) == '\"'){
            pos++;
            last--;
        }

        name = name.substr(pos , last - pos);
    }


    string disposition;
    if(!parse.search(content_disposition, pcontent_dispostion, disposition)){
//        cout << ":  there is no dispostition" << endl;
    }
    else {
        if(disposition[disposition.length() -1] == '\n')
            disposition = disposition.substr(0, disposition.length() - 1);
        if(disposition[disposition.length() -1] == ';')
            disposition = disposition.substr(0, disposition.length() - 1);
        transform(disposition.begin(), disposition.end(), disposition.begin(), ::tolower);
        cout << disposition << endl;
    }

    string filename;
    if(!parse.search(content_disposition, pname, filename)){
//        cout << ":  there is no filename" << endl;
    }
    else{
        size_t pos = filename.find("=") + 1;
        size_t last = filename.length();

        if(filename.at(last - 1) == ';'){
            last--;
        }
        if(filename.at(pos) == '\"'){
            pos++;
            last--;
        }

        filename = filename.substr(pos , last - pos);
    }

    transform(transfer.begin(), transfer.end(), transfer.begin(), ::tolower);
    if(transfer.length() > 0 && transfer[transfer.length() -1] == '\n')
        transfer = transfer.substr(0, transfer.length() - 1);

    struct content_param temp;
    temp.boundary = bound;
    temp.charset = charset;
    temp.type = type;
    parse.parse_and_decode(name, temp.name);
    temp.transfer_encoding = transfer;
    temp.disposition = disposition;
    temp.filename = filename;
    temp.id = content_id;

    return temp;
}

int key_value_parse(ifstream &file, map<string, string> &dic){
    string key, value, str;
    size_t pos;
    int eof_check = 0;
    while(getline(file, str)){
        eof_check++;

        /* when meet empty line, stop reading eml file */
        size_t start = str.find_first_not_of(" \t\n\v\f\r");
        if(start == string::npos)
            break;

        /* starting with whitespace is value */
        if(!isspace(str.at(0)) && (pos = str.find(":"))!= string::npos){
            key = str.substr(0, pos);
            start = pos + 2;
            transform(key.begin(), key.end(), key.begin(), ::tolower);
        }
        value = str.substr(start, str.length() - start);
        dic[key] += value + "\n";
    }

    if(eof_check == 0)
        return 0;
    return 1;
}

string content_parser::read_one_content(ifstream& file, struct content_param param, list<string>& bound_list){
    string type = param.type;
    string str;
    type = type.substr(0, type.find('/'));
    cout << "content type: " << type << endl;

    if(type == "multipart"){
        if(param.boundary == ""){
            cout << "this multipart not contain boundary" << endl;
            throw param;
        }

        while(1){
            getline(file, str);
            if(str == "--" + param.boundary)
                break;
            if(str == "--" + param.boundary + "--")
                return "";
        }

        bound_list.push_back(param.boundary);
        return "";
    }
    else{
        string bound;
        if(!bound_list.size() == 0){
            bound = bound_list.back();
        }
        else {
            bound = "";
        }

        string buf = "";
        while(1){
            if(!getline(file, str)){
                break;
            }
            if(str == "--" + bound){
                break;
            }
            if(str == "--" + bound + "--"){
                bound_list.pop_back();
                if(bound_list.size() > 0){
                    bound = bound_list.back();
                    //throw 1;
                    continue;
                }
                else{
                    break;
                }
            }
            buf += str;
        }

        /* decode with charset, transfer-encodig */
        cout << param.disposition << endl;
        string decoded_str;
        if(param.transfer_encoding == "base64")
            decoded_str = decode.decode(buf, base64);
        else if(param.transfer_encoding == "quoted-printable") {
            decoded_str = decode.decode(buf, qp);
        }
        else if(param.transfer_encoding == "8bit" || param.transfer_encoding == "7bit" || param.transfer_encoding == "") {
            decoded_str = buf;
        }
        else {
            cout << "THERE IS NO MATCHED ENCODING:" << param.transfer_encoding << endl;
            throw 1;
        }

        if(param.disposition == "attachment") {
            string md5_str = md5(decoded_str);
            this->attachments.push_back(file_container(param.name, decoded_str, md5_str));
            return "";
        }
        else if(param.disposition == "inline") {
            this->inlines.push_back(file_container(param.id, decoded_str));
            return "";
        }

        return decoded_str;
    }

}
content_parser::content_parser(){

}

void content_parser::parse(ifstream &file, map<string, string> dic) {
    struct content_param param;

    cout << "parse1" << endl;
    param = content_type_parse(dic["content-type"], dic["content-transfer-encoding"], dic["content-disposition"], dic["content-id"]);

    string str;
    list<string> bound_list;

    content += "charset:    " + param.charset + "\n   ";
    content += "transfer:   " + param.transfer_encoding + "\n";
    content += read_one_content(file, param, bound_list);
    content += "\n";

    cout << "parse2" << endl;
    while(bound_list.size() > 0){
        map<string, string> temp;
        if(!key_value_parse(file, temp)){
            qDebug() << "content read fail" << endl;
            break;
        }
        param = content_type_parse(temp["content-type"], temp["content-transfer-encoding"], temp["content-disposition"], dic["content-id"]);

        content += "charset:    " + param.charset + "\n";
        content += "transfer:   " + param.transfer_encoding + "\n";
        content += read_one_content(file, param, bound_list);
        content += "\n";

    }

}


