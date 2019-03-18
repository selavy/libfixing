#pragma once

#include "fix_defs.h"
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/placeholders.hpp>
#include <cassert>

// TODO: merge this with fix_parser.h via policy classes
// TODO: use versioning instead of reset

namespace fixing {

template <uint32_t A, uint32_t B, uint32_t P, uint32_t M>
struct Hash_ModPrime {
    // TODO: verify that P is prime

	static constexpr uint32_t TABLE_SIZE = M;

    static constexpr uint32_t hash(uint32_t k) noexcept {
        return (A*k + B) % P % M;
    }
};

template <uint32_t A, uint32_t B, uint32_t P, uint32_t M>
struct Hash_XOR_Simple {
    // TODO: verify that P is prime

    static constexpr uint32_t TABLE_SIZE = M;


    static constexpr uint32_t hash(uint32_t k) noexcept {
        // def hash(k):
        //     k ^= k >> A
        //     k *= P
        //     k ^= k >> B
        //     return k % M;

        // for all the stupid compilers...
        return stage3(stage2(stage1(k))) % M;
    }

private:
    static constexpr uint32_t stage1(uint32_t k) noexcept {
        return k ^ (k >> A);
    }

    static constexpr uint32_t stage2(uint32_t k) noexcept {
        return k * P;
    }

    static constexpr uint32_t stage3(uint32_t k) noexcept {
        return k ^ (k >> B);
    }
};

template <class Hash, class Tags>
class FixParser {
public:
    static constexpr size_t NTAGS = boost::mpl::size<Tags>::value;
    static constexpr size_t TABLE_SIZE = Hash::TABLE_SIZE;

	static_assert(TABLE_SIZE >= NTAGS, "Invalid table size");

    FixParser() noexcept {
        memset(&_tags[0], 0, sizeof(_tags));
        boost::mpl::for_each<Tags>(InitTagsArray(_tags));
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
        uint32_t idx;
        ReadTag r;
        while (cur < end) {
            assert(((cur == begin || *(cur - 1)) == FIXING_FIX_SEPARATOR) &&
                    "parser expects to start 1 character passed FIX separator");
            r = read_tag(cur);
            cur += r.adv;
            val = cur;
            while (*cur++ != FIXING_FIX_SEPARATOR) ;
            idx = Hash::hash(r.tag);
            assert((idx >= 0 && idx < TABLE_SIZE));
            if (r.tag == _tags[idx]) {
                _values[idx].off = static_cast<uint16_t>(val - begin);
                _values[idx].len = static_cast<uint16_t>(cur - val - 1);
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
        static_assert(!std::is_same<typename Found::type, boost::mpl::void_>::value,
                "Given tag is not present in Tags vector. Add tag to your parser type.");

        constexpr uint32_t idx = Hash::hash(Tag::value);
        assert(idx >= 0 && idx < TABLE_SIZE);
        const Value& v = _values[idx];
        return { _buffer + v.off, static_cast<std::size_t>(v.len) };
    }

private:

    //----------------------------------------------------------
    // Verify no hash function collisions
    //----------------------------------------------------------
    template <class Tag>
    struct ApplyHash : boost::mpl::int_<Hash::hash(Tag::value)> {};

    using HashedTags = typename boost::mpl::transform<Tags, ApplyHash<boost::mpl::placeholders::_1>>::type;
    using SortedHashedTags = typename boost::mpl::sort<HashedTags>::type;
    using UniqueHashedTags = typename boost::mpl::unique<SortedHashedTags,
                                        boost::is_same<
                                          boost::mpl::placeholders::_1,
                                          boost::mpl::placeholders::_2
                                        >
                                    >::type;
    static_assert(boost::mpl::size<UniqueHashedTags>::value == NTAGS, "Hash function has collisions!");

    //----------------------------------------------------------
    // Initialize presence tags array
    //----------------------------------------------------------
    struct InitTagsArray {
        InitTagsArray(uint32_t* vals) : _vals(vals) {}

        template <typename Tag> void operator()(Tag k)
        {
            constexpr uint32_t tag = Tag::value;
            constexpr uint32_t index = Hash::hash(tag);
            _vals[index] = tag;
        }

        uint32_t* _vals;
    };

    //----------------------------------------------------------
    // ASCII Tag -> Integer
    //----------------------------------------------------------
    struct ReadTag { uint32_t tag; uint32_t adv; };

    // TODO: move this to common location to share with binary search version
    FIXING_CONSTEXPR_FUNCTION
    static ReadTag read_tag(const char* FIXING_RESTRICT const c) noexcept {
        assert(isdigit(c[0]) && "called read_tag on non-numeric character");

        if (c[1] == '=') {
            const uint32_t tag = c[0] - '0';
            const uint32_t adv = 2;
            return { tag, adv };
        } else if (c[2] == '=') {
            const uint32_t tag = (c[0] - '0')*10 + (c[1] - '0')*1;
            const uint32_t adv = 3;
            return { tag, adv };
        } else if (c[3] == '=') {
            const uint32_t tag = (c[0] - '0')*100 + (c[1] - '0')*10 + (c[2] - '0')*1;
            const uint32_t adv = 4;
            return { tag, adv };
        } else if (c[4] == '=') {
            const uint32_t tag = (c[0] - '0')*1000 + (c[1] - '0')*100 + (c[2] - '0')*10 + (c[3] - '0')*1;
            const uint32_t adv = 5;
            return { tag, adv };
        } else if (c[5] == '=') {
            const uint32_t tag = (c[0] - '0')*10000 + (c[1] - '0')*1000 + (c[2] - '0')*100 + (c[3] - '0')*10 + (c[4] - '0')*1;
            const uint32_t adv = 6;
            return { tag, adv };
        } else {
            FIXING_UNREACHABLE();
        }
    }

    struct Value { uint16_t off, len; };
    const char* _buffer;
    Value       _values[TABLE_SIZE];
    uint32_t    _tags[TABLE_SIZE];
};

} /* fixing */
