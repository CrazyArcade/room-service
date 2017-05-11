#ifndef SERVER_UTILS_H
#define SERVER_UTILS_H


template <typename EnumClass>
inline auto transform_enum(EnumClass value) -> typename std::underlying_type<EnumClass>::type {
    return static_cast<typename std::underlying_type<EnumClass>::type>(value);
}

#endif //SERVER_UTILS_H
