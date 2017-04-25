#ifndef SERVER_UTILS_H
#define SERVER_UTILS_H

#include <string>
#include <cstdint>
#include <ctime>
#include <iostream>
#include <sstream>
#include "uuid/uuid.h"

using objectID = std::string;

const objectID generateID();

template <typename EnumClass>
inline auto transform_enum(EnumClass value) -> typename std::underlying_type<EnumClass>::type {
    return static_cast<typename std::underlying_type<EnumClass>::type>(value);
}

#endif //SERVER_UTILS_H
