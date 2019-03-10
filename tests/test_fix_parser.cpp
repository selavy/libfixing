#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "fixing/fixing.h"
#include <string>
#include <algorithm>

using namespace fixing;

namespace MyFIX {

using MissingTag = FixTag<75>;
using CustomTag  = FixTag<7999>;
using CustomTag2 = FixTag<80001>;

} /*MyFIX*/


using Tags = fixing::MakeTags<
            FIX::Begin,
            FIX::MsgType,
            FIX::OrderID,
            FIX::Price,
            FIX::SenderCompID,
            FIX::Symbol,
            FIX::TransactTime,
            MyFIX::MissingTag,
            FIX::ExecType,
            MyFIX::CustomTag,
            MyFIX::CustomTag2
            >;

TEST_CASE("Parse FIX message #1", "[fix_parser]")
{
    // TODO: update length and checksum to be conforming FIX
    std::string msg = "8=FIX.4.2|9=253|35=8|128=XYZ|34=124|49=CCG|56=ABC_DEFG04|52=20100208-18:51:42|55=RRC|37=NF 0015/02082010|11=NF 0015/02082010|17=0|20=0|39=0|150=0|54=2|38=1000|40=2|44=55.3600|59=0|31=0|32=0|14=0|6=0|151=1000|60=20100208-18:51:42|58=New order|30=N|1=ABC123ZYX|207=N|7999=Peter's Custom Tag That Has Data|47=A|80001=This is a long tag|10=037|";
    std::replace(msg.begin(), msg.end(), '|', '\001');

    Parser<Tags> p;
    p.parse(&*msg.begin(), &*msg.end());

    auto begin_tag = p.get<FIX::Begin>();
    REQUIRE(begin_tag == "FIX.4.2");

    auto msg_type = p.get<FIX::MsgType>();
    // printf("MsgType: '%.*s'\n", (int)msg_type.size(), msg_type.data());
    REQUIRE(msg_type == "8");

    auto orderid = p.get<FIX::OrderID>();
    REQUIRE(orderid == "NF 0015/02082010");

    auto price = p.get<FIX::Price>();
    REQUIRE(price == "55.3600");

    auto sendercompid = p.get<FIX::SenderCompID>();
    REQUIRE(sendercompid == "CCG");

    auto missingtag = p.get<MyFIX::MissingTag>();
    REQUIRE(missingtag.empty());

    auto exectype = p.get<FIX::ExecType>();
    REQUIRE(exectype == "0");

    // // Test for error message on accessing type that isn't in Tags
    // auto targetcompid = p.get<FIX::TargetCompID>();
    // REQUIRE(targetcompid.empty());

    REQUIRE(p.get<MyFIX::CustomTag>() == "Peter's Custom Tag That Has Data");

    REQUIRE(p.get<MyFIX::CustomTag2>() == "This is a long tag");
}
