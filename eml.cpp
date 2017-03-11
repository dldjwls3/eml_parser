#include "eml.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include "regex_set.h"

#include <QDebug>

eml::eml() {

}


void create_receive_list(map<string, string> &dic, eml_metadata *meta){
    size_t next, last = 0;
    string rec = dic["received"];
    int count = 0;

    cout << rec << endl;
    received temp;
    while((next = rec.find('\n', last))!=string::npos){

        if(next == last){
            temp.original = rec;
            cout << "---------------" << endl;
            cout << temp.by[0] << " " << temp.by[1] << endl;
            cout << temp.from[0] << " " << temp.from[1] << endl;

            meta->receive_list.push_back(temp);
            temp.clear();
            count++;
            last = next + 1;
            continue;
        }

        string input = rec.substr(last, next - last);
        if(input.substr(0, 4) == "from"){
            for(int i = 0; i < 2; i++){
                parse.search(input, i, temp.from[i]);
            }
        }
        else if(input.substr(0, 2) == "by"){
            for(int i = 0; i < 2; i++){
                parse.search(input, i, temp.by[i]);
            }
        }
        else{
            parse.search(input, date, temp.date);
        }
        last = next + 1;
    }

    meta->receive_list.push_back(temp);
    cout << "---------------" << endl;
    cout << temp.by[0] << " " << temp.by[1] << endl;
    cout << temp.from[0] << " " << temp.from[1] << endl;


}

void create_meta(map<string, string> &dic, eml_metadata *meta){
    meta->composed_date = dic["date"];
    parse.parse_and_decode(dic["from"], meta->from_text);
    parse.search(dic["from"], email, meta->from_email);
    parse.parse_and_decode(dic["to"], meta->to_text);
    parse.search(dic["to"], email, meta->to_email);

    size_t next = 0, last = 0;
    while((next = dic["subject"].find('\n', last)) != string::npos){
        string temp;
        parse.parse_and_decode(dic["subject"].substr(last, next - last) ,temp);
        meta->subject += temp;
        last = next + 1;
    }

}

void eml::eml_parse(ifstream &file){

    string key, value, str;
    size_t pos;

    while(getline(file, str)){
        /* when meet empty line, stop reading eml file */
        size_t start = str.find_first_not_of(" \t\n\v\f\r");
        if(start == string::npos)
            break;

        /* starting with whitespace is value */
        if(!isspace(str.at(0)) && (pos = str.find(":"))!= string::npos){
            key = str.substr(0, pos);
            start = pos + 2;
            if(start >= str.length()) {
                start = str.length();
            }
            transform(key.begin(), key.end(), key.begin(), ::tolower);
            if(key == "received" && dic[key] != "") {
                meta.receive_cnt++;
                dic[key] += "\n";
            }
        }
        value = str.substr(start, str.length() - start) + "\n";
        this->dic[key] += value;
    }

    create_receive_list(dic, &meta);
    create_meta(dic, &meta);
    content.parse(file, dic);
}





void eml::print_dic(){
    map<string, string>::iterator iter;
    for(iter=this->dic.begin(); iter != this->dic.end(); iter++){
        qDebug() << "dictionary element---------" << endl;
        qDebug() << QString::fromStdString(iter->first) << endl;
        qDebug() << QString::fromStdString(iter->second) << endl;
    }

}

void eml::print_receive_list(){
    vector<received>::iterator iter;
    for(iter = meta.receive_list.begin(); iter != meta.receive_list.end(); iter++){
        qDebug() << "receive element---------" << endl;
        qDebug() << "by ip:     " << QString::fromStdString(iter->by[ip]) << endl;
        qDebug() << "by word:   " << QString::fromStdString(iter->by[word]) << endl;
        qDebug() << "from ip:   " << QString::fromStdString(iter->from[ip]) << endl;
        qDebug() << "from word: " << QString::fromStdString(iter->from[word]) << endl;
        qDebug() << "date       " << QString::fromStdString(iter->date) << endl;
    }
}

eml_metadata eml::getMeta(){
    return this->meta;
}

content_parser eml::getContent(){
    return this->content;
}

void eml_metadata::print_meta(){
    qDebug() << "from email:    " << QString::fromStdString(from_email) << endl;
    qDebug() << "from text:     " << QString::fromStdString(from_text) << endl;
    qDebug() << "to email:      " << QString::fromStdString(to_email) << endl;
    qDebug() << "to text:       " << QString::fromStdString(to_text) << endl;
    qDebug() << "subject:       " << QString::fromStdString(subject) << endl;
    qDebug() << "composed_date: " << QString::fromStdString(composed_date) << endl;

}
