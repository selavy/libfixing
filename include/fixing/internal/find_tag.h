#pragma once

#include <boost/mpl/vector.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/unique.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/sort.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/next_prior.hpp>

namespace fixing {

template <class... Tags>
using MakeTags = typename boost::mpl::unique<
        typename boost::mpl::sort<boost::mpl::vector<Tags...>>::type,
        boost::is_same<
            boost::mpl::placeholders::_1,
            boost::mpl::placeholders::_2
        >
    >::type;

namespace detail {

template <class First, class Begin, class End>
struct FindTag {
    static int call(int tag) {
        using Length = typename boost::mpl::distance<Begin, End>::type;
        using Middle = typename boost::mpl::advance<Begin, boost::mpl::int_<Length::value / 2>>::type;
        using MiddleNext = typename boost::mpl::next<Middle>::type;
        using MiddleTag = typename Middle::type;
        if (tag < MiddleTag::value) {
            return FindTag<First, Begin, Middle>::call(tag);
        } else if (tag == MiddleTag::value) {
            using Index = typename boost::mpl::distance<First, Middle>::type;
            return static_cast<int>(Index::value);
        } else {
            return FindTag<First, MiddleNext, End>::call(tag);
        }
    }
};

template <class First, class End>
struct FindTag<First, End, End> {
    static int call(int tag) {
        return -1;
    }
};

} /*detail*/

template <class Seq>
int tag2idx(int tag) {
    using Begin = typename boost::mpl::begin<Seq>::type;
    using End   = typename boost::mpl::end<Seq>::type;
    return detail::FindTag<Begin, Begin, End>::call(tag);
}

template <class Seq>
bool find_tag(int tag) {
    return tag2idx<Seq>(tag) >= 0;
}

} /*fixing*/
