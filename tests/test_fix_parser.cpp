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
            FIX::Begin,        // 8
            FIX::MsgType,      // 35
            FIX::OrderID,      // 37
            FIX::Price,        // 44
            FIX::SenderCompID, // 49
            FIX::Symbol,       // 55
            FIX::TransactTime, // 60
            MyFIX::MissingTag, // 75
            FIX::ExecType,     // 150
            MyFIX::CustomTag,  // 7999
            MyFIX::CustomTag2  // 80001
            >;

TEST_CASE("Parse FIX message #1", "[fix_parser]")
{
    // TODO: update length and checksum to be conforming FIX
    std::string msg = "8=FIX.4.2|9=253|35=8|128=XYZ|34=124|49=CCG|56=ABC_DEFG04|52=20100208-18:51:42|55=RRC|37=NF 0015/02082010|11=NF 0015/02082010|17=0|20=0|39=0|150=0|54=2|38=1000|40=2|44=55.3600|59=0|31=0|32=0|14=0|6=0|151=1000|60=20100208-18:51:42|58=New order|30=N|1=ABC123ZYX|207=N|7999=Peter's Custom Tag That Has Data|47=A|80001=This is a long tag|10=037|";
    std::replace(msg.begin(), msg.end(), '|', '\001');

    Parser<Tags> p1;

    // // With collision to verify collision detection:
    // FixParser<Hash_ModPrime<4173853012, 3915483460, 102253, 16>, Tags> p2;
    // A = 3323256119, B = 163802662, P = 103919, M = 16
    FixParser<Hash_ModPrime<3323256119, 163802662, 103919, 16>, Tags> p2;
    // P = 100769, A = 4, B = 2, M = 16
    FixParser<Hash_XOR_Simple<4, 2, 100769, 16>, Tags> p3;
    // P = 103391, A = 9, M = 16
    FixParser<Hash_Simple<9, 103391, 16>, Tags> p4;
    // A = 4, B = 5, M = 16
    FixParser<Hash_XOR<4, 5, 16>, Tags> p5;

    p1.parse(&*msg.begin(), &*msg.end());
    p2.parse(&*msg.begin(), &*msg.end());
    p3.parse(&*msg.begin(), &*msg.end());
    p4.parse(&*msg.begin(), &*msg.end());
    p5.parse(&*msg.begin(), &*msg.end());

    REQUIRE(p1.get<FIX::Begin>() == "FIX.4.2");
    REQUIRE(p2.get<FIX::Begin>() == "FIX.4.2");
    REQUIRE(p3.get<FIX::Begin>() == "FIX.4.2");
    REQUIRE(p4.get<FIX::Begin>() == "FIX.4.2");
    REQUIRE(p5.get<FIX::Begin>() == "FIX.4.2");

    REQUIRE(p1.get<FIX::MsgType>() == "8");
    REQUIRE(p2.get<FIX::MsgType>() == "8");
    REQUIRE(p3.get<FIX::MsgType>() == "8");
    REQUIRE(p4.get<FIX::MsgType>() == "8");
    REQUIRE(p5.get<FIX::MsgType>() == "8");

    REQUIRE(p1.get<FIX::OrderID>() == "NF 0015/02082010");
    REQUIRE(p2.get<FIX::OrderID>() == "NF 0015/02082010");
    REQUIRE(p3.get<FIX::OrderID>() == "NF 0015/02082010");
    REQUIRE(p4.get<FIX::OrderID>() == "NF 0015/02082010");
    REQUIRE(p5.get<FIX::OrderID>() == "NF 0015/02082010");

    REQUIRE(p1.get<FIX::Price>() == "55.3600");
    REQUIRE(p2.get<FIX::Price>() == "55.3600");
    REQUIRE(p3.get<FIX::Price>() == "55.3600");
    REQUIRE(p4.get<FIX::Price>() == "55.3600");
    REQUIRE(p5.get<FIX::Price>() == "55.3600");

    REQUIRE(p1.get<FIX::SenderCompID>() == "CCG");
    REQUIRE(p2.get<FIX::SenderCompID>() == "CCG");
    REQUIRE(p3.get<FIX::SenderCompID>() == "CCG");
    REQUIRE(p4.get<FIX::SenderCompID>() == "CCG");
    REQUIRE(p5.get<FIX::SenderCompID>() == "CCG");

    REQUIRE(p1.get<MyFIX::MissingTag>().empty());
    REQUIRE(p2.get<MyFIX::MissingTag>().empty());
    REQUIRE(p3.get<MyFIX::MissingTag>().empty());
    REQUIRE(p4.get<MyFIX::MissingTag>().empty());
    REQUIRE(p5.get<MyFIX::MissingTag>().empty());

    REQUIRE(p1.get<FIX::ExecType>() == "0");
    REQUIRE(p2.get<FIX::ExecType>() == "0");
    REQUIRE(p3.get<FIX::ExecType>() == "0");
    REQUIRE(p4.get<FIX::ExecType>() == "0");
    REQUIRE(p5.get<FIX::ExecType>() == "0");

    // // Test for error message on accessing type that isn't in Tags
    // REQUIRE(p1.get<FIX::TargetCompID>().empty());
    // REQUIRE(p2.get<FIX::TargetCompID>().empty());
    // REQUIRE(p3.get<FIX::TargetCompID>().empty());
    // REQUIRE(p4.get<FIX::TargetCompID>().empty());
    // REQUIRE(p5.get<FIX::TargetCompID>().empty());

    REQUIRE(p1.get<MyFIX::CustomTag>() == "Peter's Custom Tag That Has Data");
    REQUIRE(p2.get<MyFIX::CustomTag>() == "Peter's Custom Tag That Has Data");
    REQUIRE(p3.get<MyFIX::CustomTag>() == "Peter's Custom Tag That Has Data");
    REQUIRE(p4.get<MyFIX::CustomTag>() == "Peter's Custom Tag That Has Data");
    REQUIRE(p5.get<MyFIX::CustomTag>() == "Peter's Custom Tag That Has Data");

    REQUIRE(p1.get<MyFIX::CustomTag2>() == "This is a long tag");
    REQUIRE(p2.get<MyFIX::CustomTag2>() == "This is a long tag");
    REQUIRE(p3.get<MyFIX::CustomTag2>() == "This is a long tag");
    REQUIRE(p4.get<MyFIX::CustomTag2>() == "This is a long tag");
    REQUIRE(p5.get<MyFIX::CustomTag2>() == "This is a long tag");
}
