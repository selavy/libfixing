#pragma once

#include "find_tag.h"
#include <boost/mpl/size.hpp>
#include <boost/mpl/find.hpp>
#include <cstdint>
#include <cstring>
#include <cassert>

#ifdef HAVE_STD_STRING_VIEW
#include <string_view>
namespace fixing {
using std::string_view;
} /*fixing*/
#else
#include <experimental/string_view>
namespace fixing {
using std::experimental::string_view;
} /*fixing*/
#endif

// TODO: move to a definitions file
#define FIXING_LIKELY(x) __builtin_expect(x, 1)
#define FIXING_UNLIKELY(x) __builtin_expect(x, 0)
#define FIXING_UNREACHABLE() __builtin_unreachable()
#define FIXING_RESTRICT __restrict
#define FIXING_HOT_FUNCTION __attribute__((hot))
#define FIXING_COLD_FUNCTION __attribute__((cold, noinline))
#define FIXING_CONSTEXPR_FUNCTION constexpr
#define FIXING_CONSTEXPR constexpr

// TODO: detect if have nodiscard
#define FIXING_NODISCARD
// #define FIXING_NODISCARD [[nodiscard]]

#if defined(USE_IACA)
    #include "iacaMarks.h"
    #define FIXING_IACA_START IACA_START
    #define FIXING_IACA_END   IACA_START
#else
    #define FIXING_IACA_START
    #define FIXING_IACA_END
#endif

#define FIXING_FIX_SEPARATOR '\001'
#define FIXING_CHECKSUM_TAG 10

namespace fixing {

template <class Tags>
class Parser {
public:
    FIXING_CONSTEXPR static int NTAGS = boost::mpl::size<Tags>::value;

    // TODO: verify that Tags is a boost::mpl::vector
    // TODO: verify that Tags sorted
    // TODO: verify that Tags uniq

    Parser() noexcept { reset(); }

    // TODO: evaluate if better to have _version vs. resetting all tags.
    FIXING_HOT_FUNCTION
    void reset() FIXING_RESTRICT noexcept {
        memset(&_values[0], 0, sizeof(_values));
    }

    FIXING_HOT_FUNCTION
    void parse(const char* const FIXING_RESTRICT begin,
               const char* const FIXING_RESTRICT end) FIXING_RESTRICT noexcept
    {
        FIXING_IACA_START

        // TODO: can I use the fact that expecting at least "10=XXX" at the end
        ReadTagResult r = read_tag(begin);
        int tag = r.tag;
        const char* cur = begin + r.adv;
        const char* value = cur;
        assert(*(cur - 1) == '=' && "didn't read tag correctly");
        while (cur < end) {
            const char c = *cur++;
            if (FIXING_UNLIKELY(c == '\001')) {
                const int idx = tag2idx<Tags>(tag);
                if (idx >= 0) {
                    _values[idx].v[0] = static_cast<uint16_t>(value - begin);
                    _values[idx].v[1] = static_cast<uint16_t>(cur - begin -1);
                }
                r = read_tag(cur);
                tag = r.tag;
                cur += r.adv;
                if (FIXING_UNLIKELY(tag == FIXING_CHECKSUM_TAG)) {
                    break;
                }
                assert(*(cur - 1) == '=' && "didn't read tag correctly");
                value = cur;
            }
        }
        _buffer = begin;

        FIXING_IACA_END
    }

    template <class Tag>
    FIXING_HOT_FUNCTION FIXING_CONSTEXPR_FUNCTION FIXING_NODISCARD
    string_view get() const FIXING_RESTRICT noexcept {
        using Begin = typename boost::mpl::begin<Tags>::type;
        using Found = typename boost::mpl::find<Tags, Tag>::type;
        using Index = typename boost::mpl::distance<Begin, Found>::type;
        FIXING_CONSTEXPR int idx = Index::value;
        const Value& v = _values[idx];
        return { _buffer + v.v[0], static_cast<size_t>(v.v[1] - v.v[0]) };
    }

private:

    struct ReadTagResult { int tag; int adv; };

    FIXING_HOT_FUNCTION FIXING_CONSTEXPR_FUNCTION
    static ReadTagResult read_tag(const char* FIXING_RESTRICT const c) noexcept {
        const int r1 = (c[0] - '0')*1;
        const int r2 = (c[0] - '0')*10 + (c[1] - '0')*1;
        const int r3 = (c[0] - '0')*100 + (c[1] - '0')*10 + (c[2] - '0')*1;
        const int r4 = (c[0] - '0')*1000 + (c[1] - '0')*100 + (c[2] - '0')*10 + (c[3] - '0')*1;

        const int e1 = c[1] == '=';
        const int e2 = c[2] == '=';
        const int e3 = c[3] == '=';
        const int e4 = c[4] == '=';

        const bool c1 =  e1;
        const bool c2 = !e1 &&  e2;
        const bool c3 = !e1 && !e2 && e3;
        const bool c4 = !e1 && !e2 && !e3 & e4;

        assert(c1 || c2 || c3 || c4 && "couldn't find '='");
        int tag = c1*r1 | c2*r2 | c3*r3 | c4*r4;
        int adv = c1*2  | c2*3  | c3*4  | c4*5 ;
        return { tag, adv };
    }

private:
    // TODO: is it possible to have an empty tag? i.e. is there
    // ever a reason to differentiate between empty and not present?

    // make as std::pair<uint16_t, uint16_t>, but using this to reduce compile time
    // because I don't need all the machinery around pair<> and tuple<>
    struct Value { uint16_t v[2]; };

    const char* _buffer;
    Value _values[NTAGS];
};

} /*fixing*/
