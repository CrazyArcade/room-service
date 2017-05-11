#ifndef SERVER_OBJECTID_H
#define SERVER_OBJECTID_H

#include "uuid/uuid.h"
#include <string>

using objectID = std::string;

struct ObjectID {
private:
    static const objectID compassUUID(const uuid_t t) {
        objectID id;
        enum {
            LEN = 16
        };
        const char charTable[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                  'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                  'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                  'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                  'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                  'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                  'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                  '4', '5', '6', '7', '8', '9', '+', '/'};
        for (int i = 0; i < LEN;) {
            uint32_t a = i < LEN ? static_cast<unsigned char>(t[i++]) : 0;
            uint32_t b = i < LEN ? static_cast<unsigned char>(t[i++]) : 0;
            uint32_t c = i < LEN ? static_cast<unsigned char>(t[i++]) : 0;
            uint32_t d = (a << 0x10) + (b << 0x08) + c;

            for (int j = 3; j >= 0; j--) {
                id += charTable[(d >> j * 6) & 0x3F];
            }
        }
        // remove last 2 same characters "AA"
        id.erase(22, 23);
        return id;
    }

public:
    inline static const objectID generateID() {
        uuid_t t;
        uuid_generate(t);
        return compassUUID(t);
    }
};

#endif //SERVER_OBJECTID_H
