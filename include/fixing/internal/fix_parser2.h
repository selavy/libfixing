#pragma once

#include "fix_defs.h"

// TODO: merge this with fix_parser.h via policy classes

namespace fixing {

template <uint32_t A, uint32_t B, uint32_t P, uint32_t M>
struct Hash1 {
    static constexpr uint32_t hash(uint32_t k) noexcept {
        return (A*k + B) % P % M;
    }
};

template <class Hash, class Tags>
class Parser : private Hash {
public:
    FIXING_CONSTEXPR static int NTAGS = boost::mpl::size<Tags>::value;


private:
    struct Value { uint16_t v[2]; };

    const char* _buffer;
    Value       _values[NTAGS];
    uint16_t    _tags[NTAGS];
};

} /* fixing */
