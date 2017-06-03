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

        bool operator==(const Vec2 &v2) const {
            return (x == v2.x) && (y == v2.y);
        }

        Vec2 &operator+=(const Vec2 &v) {
            *this = *this + v;
            return *this;
        }

        Vec2 operator+(const Vec2 &v) const {
            auto x = this->x + v.x, y = this->y + v.y;
            return Vec2(x, y);
        }

        Vec2 &operator*=(uint8_t scale) {
            *this = *this * scale;
            return *this;
        }

        Vec2 operator*(uint8_t scale) const {
            auto x = this->x * scale, y = this->y * scale;
            return Vec2(x, y);
        }
    };
}

#endif //SERVER_APP_H
