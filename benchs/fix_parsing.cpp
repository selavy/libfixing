#include <benchmark/benchmark.h>

#include "fixing/fixing.h"
#include <cstdint>

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

static std::string get_fix_message() {
    std::string msg = "8=FIX.4.2|9=253|35=8|128=XYZ|34=124|49=CCG|56=ABC_DEFG04|52=20100208-18:51:42|55=RRC|37=NF 0015/02082010|11=NF 0015/02082010|17=0|20=0|39=0|150=0|54=2|38=1000|40=2|44=55.3600|59=0|31=0|32=0|14=0|6=0|151=1000|60=20100208-18:51:42|58=New order|30=N|1=ABC123ZYX|207=N|7999=Peter's Custom Tag That Has Data|47=A|80001=This is a long tag|10=037|";
    std::replace(msg.begin(), msg.end(), '|', '\001');
    return msg;
}

static void BM_FixParser_BinarySearch(benchmark::State& state) {
    Parser<Tags> p;
    const std::string msg = get_fix_message();
    uint64_t deopt = 0;
    for (auto _ : state) {
        p.parse(&*msg.begin(), &*msg.end());
        deopt += p.get<FIX::Begin>()[0];
        deopt += p.get<FIX::MsgType>()[0];
    }
    if (deopt == 0) {
        printf("WOO\n");
    }
}
BENCHMARK(BM_FixParser_BinarySearch);

static void BM_FixParser_ModPrime(benchmark::State& state) {
    FixParser<Hash_ModPrime<3323256119, 163802662, 103919, 16>, Tags> p;
    const std::string msg = get_fix_message();
    uint64_t deopt = 0;
    for (auto _ : state) {
        p.parse(&*msg.begin(), &*msg.end());
        deopt += p.get<FIX::Begin>()[0];
        deopt += p.get<FIX::MsgType>()[0];
    }
    if (deopt == 0) {
        printf("WOO\n");
    }
}
BENCHMARK(BM_FixParser_ModPrime);

static void BM_FixParser_XOR_Simple(benchmark::State& state) {
    FixParser<Hash_XOR_Simple<4, 2, 100769, 16>, Tags> p;
    const std::string msg = get_fix_message();
    uint64_t deopt = 0;
    for (auto _ : state) {
        p.parse(&*msg.begin(), &*msg.end());
        deopt += p.get<FIX::Begin>()[0];
        deopt += p.get<FIX::MsgType>()[0];
    }
    if (deopt == 0) {
        printf("WOO\n");
    }
}
BENCHMARK(BM_FixParser_XOR_Simple);

BENCHMARK_MAIN();
