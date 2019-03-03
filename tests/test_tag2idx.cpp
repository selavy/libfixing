#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <vector>
#include <algorithm>
#include "fixing.h"

namespace v2 {

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

} /*v2*/

using Tags = MakeTags<
    v2::FIX::MsgType,
    v2::FIX::OrderID,
    v2::FIX::Begin,
    v2::FIX::OrderID,
    v2::FIX::OrdStatus,
    v2::FIX::MsgType,
    v2::FIX::CustomTag,
    v2::FIX::ExecType,
    v2::FIX::TransactTime,
    v2::FIX::Symbol,
    v2::FIX::SenderCompID,
    v2::FIX::SecurityExchange,
    v2::FIX::TargetCompID,
    v2::FIX::TargetSubID
    >;

using ExpectedTags = boost::mpl::vector<
    v2::FIX::Begin,
    v2::FIX::MsgType,
    v2::FIX::OrderID,
    v2::FIX::OrdStatus,
    v2::FIX::SenderCompID,
    v2::FIX::Symbol,
    v2::FIX::TargetCompID,
    v2::FIX::TargetSubID,
    v2::FIX::TransactTime,
    v2::FIX::ExecType,
    v2::FIX::SecurityExchange,
    v2::FIX::CustomTag
    >;

TEST_CASE("FindTag", "[fix]")
{
    const std::vector<int> tags = {
        v2::FIX::MsgType::value,
        v2::FIX::OrderID::value,
        v2::FIX::Begin::value,
        v2::FIX::OrderID::value,
        v2::FIX::OrdStatus::value,
        v2::FIX::MsgType::value,
        v2::FIX::CustomTag::value,
        v2::FIX::ExecType::value,
        v2::FIX::TransactTime::value,
        v2::FIX::Symbol::value,
        v2::FIX::SenderCompID::value,
        v2::FIX::SecurityExchange::value,
        v2::FIX::TargetCompID::value,
        v2::FIX::TargetSubID::value,
    };

    static_assert(boost::mpl::equal<Tags, ExpectedTags>::value);

    for (auto tag : tags) {
        auto result = find_tag<Tags>(tag);
        REQUIRE(result == true);
    }

    for (int tag = 0; tag < 10000; ++tag) {
        auto expected = std::find(tags.begin(), tags.end(), tag) != tags.end();
        auto result = find_tag<Tags>(tag);
        REQUIRE(result == expected);
    }
}

TEST_CASE("VerifyMapping", "[fix]")
{
    const std::vector<int> sorted_tags = {
        v2::FIX::Begin::value,
        v2::FIX::MsgType::value,
        v2::FIX::OrderID::value,
        v2::FIX::OrdStatus::value,
        v2::FIX::SenderCompID::value,
        v2::FIX::Symbol::value,
        v2::FIX::TargetCompID::value,
        v2::FIX::TargetSubID::value,
        v2::FIX::TransactTime::value,
        v2::FIX::ExecType::value,
        v2::FIX::SecurityExchange::value,
        v2::FIX::CustomTag::value,
    };

    for (int i = 0; i < sorted_tags.size(); ++i) {
        int tag = sorted_tags[i];
        REQUIRE(tag2idx<Tags>(tag) == i);
    }
}
