#ifndef CONTENT_PARSER_H
#define CONTENT_PARSER_H

#include <string>
#include <fstream>
#include <map>
#include <list>

struct file_container {
    std::string id_or_name;
    std::string file;
    std::string md5_hash;

    file_container() {

    }

    file_container(std::string a, std::string b) {
        id_or_name = a;
        file = b;
    }
    file_container(std::string a, std::string b, std::string c) {
        id_or_name = a;
        file = b;
        md5_hash = c;
    }
};

class content_parser
{
public:
    std::string content;
    std::string charset;
    std::string transfer;
    std::list<struct file_container> attachments;
    std::list<struct file_container> inlines;

    content_parser();
    content_parser(content_parser &a){
        content = a.content;
        attachments = a.attachments;
        inlines = a.inlines;
    }

    void parse(std::ifstream &file, std::map<std::string, std::string> dic);
    std::string content_parser::read_one_content(std::ifstream& file, struct content_param param, std::list<std::string>& bound_list);

};

#endif // CONTENT_PARSER_H
