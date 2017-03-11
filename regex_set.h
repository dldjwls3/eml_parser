#ifndef REGEX_SET_H
#define REGEX_SET_H

#include <string>
#include <regex>

using namespace std;

enum pattern_name{
    ip,
    word,
    date,
    email,
    encode,
    plain,
    pcontent_type,
    pcharset,
    pboundary,
    pname,
    pcontent_dispostion,
    pattern_cnt
};

class regex_set
{
private:
    regex pattern[pattern_cnt];
    smatch result;
public:
    regex_set(){
        this->pattern[email] = regex("([\\w\\.]+)@(\\w+[\\S]*)\\.([A-Za-z]+)");
        this->pattern[ip] = regex("\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}.\\d{1,3}");
        this->pattern[date] = regex("[\\w]{1,3}, [123]?[0-9] [\\w]{1,3} [\\d]{4} \\d\\d:\\d\\d:\\d\\d [-+]\\d\\d\\d\\d");
        this->pattern[word] = regex("\\s[\\S]+");
        this->pattern[encode] = regex("\\=\\?[\\S]*\\?\\=");
        this->pattern[plain] = regex("\\\"[\\S]+\\\"");
        this->pattern[pcontent_type] = regex("[\\w]+\\/[\\w\\.]+;?");
        this->pattern[pcharset] = regex("((charset)|(Charset))=\\\"?\\S+\\\"?;?");
        this->pattern[pboundary] = regex("((boundary)|(Boundary))=\\\"?\\S+\\\"?;?");
        this->pattern[pname] = regex("((name)|(Name)|(filename)|(Filename)|(FileName))=\\\"?\\S+\\\"?;?");
        this->pattern[pcontent_dispostion] = regex("\\w+;?");
    }
    int search(string input, int a, string& s);
    void parse_and_decode(string input, string& s);
};

extern regex_set parse;

#endif // REGEX_SET_H
