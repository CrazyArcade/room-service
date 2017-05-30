#ifndef SERVER_APP_H
#define SERVER_APP_H

#ifndef MIN
#define MIN(x, y) (((x) > (y)) ? (y) : (x))
#endif  // MIN

#ifndef MAX
#define MAX(x, y) (((x) < (y)) ? (y) : (x))
#endif  // MAX

namespace APP {
    struct Size {
        Size(unsigned int height = 0, unsigned int width = 0) : height(height), width(width) {};
        unsigned int height;
        unsigned int width;
    };

    struct Vec2 {
        Vec2(int x = 0, int y = 0) : x(x), y(y) {};
        int x;
        int y;
    };
}

#endif //SERVER_APP_H
