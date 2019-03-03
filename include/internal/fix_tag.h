#pragma once

#include <boost/mpl/int.hpp>

template <int N>
struct FixTag : boost::mpl::int_<N> {};

namespace FIX {

using Begin            = FixTag<8>;
using MsgType          = FixTag<35>;
using OrderID          = FixTag<37>;
using OrdStatus        = FixTag<39>;
using Price            = FixTag<44>;
using SenderCompID     = FixTag<49>;
using Symbol           = FixTag<55>;
using TargetCompID     = FixTag<56>;
using TargetSubID      = FixTag<57>;
using TransactTime     = FixTag<60>;
using MissingTag       = FixTag<75>;
using ExecType         = FixTag<150>;
using SecurityExchange = FixTag<207>;
using CustomTag        = FixTag<7999>;

} /*FIX*/
