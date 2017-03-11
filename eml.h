#ifndef EML_H
#define EML_H

#include <fstream>
#include <vector>
#include <map>
#include "content_parser.h"

using namespace std;

class received{
public:
    string by[2];
    string from[2];
    string date;
    string original;

    void clear(){
        by[0].clear();
        by[1].clear();
        from[0].clear();
        from[1].clear();
        date.clear();
        original.clear();
    }
};

class eml_metadata {

public:
    vector<received> receive_list;
    string composed_date;
    string from_text;
    string from_email;
    string to_text;
    string to_email;
    string subject;
   // string content;
    int receive_cnt = 1;

    eml_metadata(){

    }
    eml_metadata(eml_metadata &A) {
        receive_list = A.receive_list;
        composed_date = A.composed_date;
        from_text = A.from_text;
        from_email = A.from_email;
        to_text = A.to_text;
        to_email = A.to_email;
        subject = A.subject;
        receive_cnt = A.receive_cnt;
    }

    void print_meta();
};

class eml
{
public:
    eml_metadata meta;
    content_parser content;

    map<string, string> dic;

    eml();
    void eml_parse(ifstream &file);
    void print_dic();
    void print_receive_list();
    eml_metadata getMeta();
    content_parser getContent();
};

#endif // EML_H
