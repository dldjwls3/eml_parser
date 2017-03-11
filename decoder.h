#ifndef DECODER_H
#define DECODER_H

#include <string>
#include <vector>

using namespace std;

enum encode_way {
    base64,
    utf_8,
    euc_kr,
    qp
};


/* decode to multibyte */
class decoder
{
public:
    decoder(){

    }

    ~decoder(){

    }

    string decode(string input, int encode_way);
};

extern decoder decode;
#endif // DECODER_H
