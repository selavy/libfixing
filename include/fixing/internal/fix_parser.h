#pragma once

#include "fix_defs.h"
#include "find_tag.h"
#include <boost/mpl/size.hpp>
#include <boost/mpl/find.hpp>

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
    void reset() FIXING_RESTRICT noexcept {
        memset(&_values[0], 0, sizeof(_values));
    }

    void parse(const char* const FIXING_RESTRICT begin,
               const char* const FIXING_RESTRICT end) FIXING_RESTRICT noexcept
    {
        FIXING_IACA_START

        const char* cur = begin;
        const char* val;
        int idx;
        ReadTag r;
        while (cur < end) {
            assert(((cur == begin || *(cur - 1)) == FIXING_FIX_SEPARATOR) &&
                    "parser expects to start 1 character passed FIX separator");
            r = read_tag(cur);
            cur += r.adv;
            val = cur;
            while (*cur++ != FIXING_FIX_SEPARATOR) {}
            idx = tag2idx<Tags>(r.tag);
            if (idx >= 0) {
                auto& v = _values[idx];
                v.off = static_cast<uint16_t>(val - begin);
                v.len = static_cast<uint16_t>(cur - val - 1);
            }
        }
        _buffer = begin;

        FIXING_IACA_END
    }

    template <class Tag>
    FIXING_CONSTEXPR_FUNCTION FIXING_NODISCARD
    string_view get() const FIXING_RESTRICT noexcept {
        using Begin = typename boost::mpl::begin<Tags>::type;
        using Found = typename boost::mpl::find<Tags, Tag>::type;
        using Index = typename boost::mpl::distance<Begin, Found>::type;

        // if Tag isn't in Tags then this check will fail:
        using FoundValue = typename boost::mpl::deref<Found>::type;
        static_assert(!std::is_same<FoundValue, boost::mpl::void_>::value,
                "Given tag is not present in Tags vector. Add tag to your parser type.");

        FIXING_CONSTEXPR int idx = Index::value;
        const Value& v = _values[idx];
        return { _buffer + v.off, static_cast<std::size_t>(v.len) };
    }

private:

    struct ReadTag { int tag; int adv; };

// #define BRANCHLESS_READ_TAG
#ifdef BRANCHLESS_READ_TAG

    // TODO: fix me
    FIXING_CONSTEXPR_FUNCTION
    static ReadTag read_tag(const char* FIXING_RESTRICT const c) noexcept {
        assert(isdigit(c[0]) && "called read_tag on non-numeric character");

        const int r1 = (c[0] - '0')*1;
        const int r2 = (c[0] - '0')*10    + (c[1] - '0')*1;
        const int r3 = (c[0] - '0')*100   + (c[1] - '0')*10   + (c[2] - '0')*1;
        const int r4 = (c[0] - '0')*1000  + (c[1] - '0')*100  + (c[2] - '0')*10  + (c[3] - '0')*1;
        const int r5 = (c[0] - '0')*10000 + (c[1] - '0')*1000 + (c[2] - '0')*100 + (c[3] - '0')*10 + (c[4] - '0')*1;

        const int e1 = c[1] == '=';
        const int e2 = c[2] == '=';
        const int e3 = c[3] == '=';
        const int e4 = c[4] == '=';
        const int e5 = c[5] == '=';

        const bool c1 =  e1;
        const bool c2 = !e1 &  e2;
        const bool c3 = !e1 & !e2 &  e3;
        const bool c4 = !e1 & !e2 & !e3 &  e4;
        const bool c5 = !e1 & !e2 & !e3 & !e4 & e5;

        assert((c1 || c2 || c3 || c4 || c5) && "couldn't find '='");
        const int tag = c1*r1 | c2*r2 | c3*r3 | c4*r4 | c5*r5;
        const int adv = c1* 2 | c2* 3 | c3* 4 | c4* 5 | c5* 6;
        return { tag, adv };
    }

#else

    FIXING_CONSTEXPR_FUNCTION
    static ReadTag read_tag(const char* FIXING_RESTRICT const c) noexcept {
        assert(isdigit(c[0]) && "called read_tag on non-numeric character");

        if (c[1] == '=') {
            const int tag = c[0] - '0';
            const int adv = 2;
            return { tag, adv };
        } else if (c[2] == '=') {
            const int tag = (c[0] - '0')*10 + (c[1] - '0')*1;
            const int adv = 3;
            return { tag, adv };
        } else if (c[3] == '=') {
            const int tag = (c[0] - '0')*100 + (c[1] - '0')*10 + (c[2] - '0')*1;
            const int adv = 4;
            return { tag, adv };
        } else if (c[4] == '=') {
            const int tag = (c[0] - '0')*1000 + (c[1] - '0')*100 + (c[2] - '0')*10 + (c[3] - '0')*1;
            const int adv = 5;
            return { tag, adv };
        } else if (c[5] == '=') {
            const int tag = (c[0] - '0')*10000 + (c[1] - '0')*1000 + (c[2] - '0')*100 + (c[3] - '0')*10 + (c[4] - '0')*1;
            const int adv = 6;
            return { tag, adv };
        } else {
            FIXING_UNREACHABLE();
        }
    }

#endif

private:
    // TODO: is it possible to have an empty tag? i.e. is there
    // ever a reason to differentiate between empty and not present?

    // make as std::pair<uint16_t, uint16_t>, but using this to reduce compile time
    // because I don't need all the machinery around pair<> and tuple<>
    struct Value { uint16_t off, len; };

    const char* _buffer;
    Value _values[NTAGS];
};

} /*fixing*/
