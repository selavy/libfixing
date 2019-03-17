#pragma once

#include "fix_defs.h"

// TODO: merge this with fix_parser.h via policy classes
// TODO: use versioning instead of reset

namespace fixing {

template <uint32_t A, uint32_t B, uint32_t P, uint32_t M>
struct Hash_ModPrime {
    static constexpr uint32_t hash(uint32_t k) noexcept {
        return (A*k + B) % P % M;
    }
};

template <class Hash, class Tags>
class Parser : private Hash {
public:
    FIXING_CONSTEXPR static int NTAGS = boost::mpl::size<Tags>::value;

    Parser() noexcept {
        memset(&_tags[0], 0, sizeof(_tags));
        reset();
    }

    void reset() noexcept {
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
            while (*cur++ != FIXING_FIX_SEPARATOR) ;
            idx = Hash::hash(r.tag);
            assert((idx >= 0 && idx < NTAGS));
            if (r.tag == _tags[idx]) {
                _values[idx].off = static_cast<uint16_t>(value - begin);
                _values[idx].len = static_cast<uint16_t>(cur - value - 1);
            }
        }

        _buffer = begin;

        FIXING_IACA_END
    }

    template <class Tag>
    FIXING_CONSTEXPR_FUNCTION FIXING_NODISCARD
    string_view get() const FIXING_RESTRICT noexcept {
        // if Tag isn't in Tags then this check will fail:
        using Found = typename boost::mpl::find<Tags, Tag>::type;
        using FoundValue = typename boost::mpl::deref<Found>::type;
        static_assert(FoundValue::value == Tag::value,
                "Given tag is not present in Tags vector. Add tag to your parser type.");

        FIXING_CONSTEXPR int idx = Hash::hash(Tag::value);
        assert(idx >= 0 && idx < NTAGS && _tags[idx] == Value::tag);

        const Value& v = _values[idx];
        return { _buffer + v.off, static_cast<std::size_t>(v.len) };
    }

private:
    struct Value { uint16_t off, len; };
    struct ReadTag { uint32_t tag; uint32_t adv; };

    // TODO: move this to common location to share with binary search version
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

    const char* _buffer;
    Value       _values[NTAGS];
    uint16_t    _tags[NTAGS];
};

} /* fixing */
