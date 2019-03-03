#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <vector>
#include <algorithm>
#include "fixing.h"

using Tags = MakeTags<
    FIX::MsgType,
    FIX::OrderID,
    FIX::Begin,
    FIX::OrderID,
    FIX::OrdStatus,
    FIX::MsgType,
    FIX::CustomTag,
    FIX::ExecType,
    FIX::TransactTime,
    FIX::Symbol,
    FIX::SenderCompID,
    FIX::SecurityExchange,
    FIX::TargetCompID,
    FIX::TargetSubID
    >;

using ExpectedTags = boost::mpl::vector<
    FIX::Begin,
    FIX::MsgType,
    FIX::OrderID,
    FIX::OrdStatus,
    FIX::SenderCompID,
    FIX::Symbol,
    FIX::TargetCompID,
    FIX::TargetSubID,
    FIX::TransactTime,
    FIX::ExecType,
    FIX::SecurityExchange,
    FIX::CustomTag
    >;

TEST_CASE("FindTag", "[fix]")
{
    const std::vector<int> tags = {
        FIX::MsgType::value,
        FIX::OrderID::value,
        FIX::Begin::value,
        FIX::OrderID::value,
        FIX::OrdStatus::value,
        FIX::MsgType::value,
        FIX::CustomTag::value,
        FIX::ExecType::value,
        FIX::TransactTime::value,
        FIX::Symbol::value,
        FIX::SenderCompID::value,
        FIX::SecurityExchange::value,
        FIX::TargetCompID::value,
        FIX::TargetSubID::value,
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
        FIX::Begin::value,
        FIX::MsgType::value,
        FIX::OrderID::value,
        FIX::OrdStatus::value,
        FIX::SenderCompID::value,
        FIX::Symbol::value,
        FIX::TargetCompID::value,
        FIX::TargetSubID::value,
        FIX::TransactTime::value,
        FIX::ExecType::value,
        FIX::SecurityExchange::value,
        FIX::CustomTag::value,
    };

    for (int i = 0; i < sorted_tags.size(); ++i) {
        int tag = sorted_tags[i];
        REQUIRE(tag2idx<Tags>(tag) == i);
    }
}
