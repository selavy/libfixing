#include <array>
#include <algorithm>
#include <utility>
#include <cassert>
#include <cstdio>

template <char... Cs>
struct sequence {};

template <int I, char... Cs>
struct gen_seq
{
    using type = typename gen_seq<I / 10, (I % 10) + '0', Cs...>::type;
};

template <char... Cs>
struct gen_seq<0, Cs...>
{
    using type = sequence<Cs...>;
};

template <char... Ts>
constexpr auto make_tag(sequence<Ts...>) -> std::array<char, sizeof...(Ts) + 2>
{
    return { '\001', Ts..., '=' };
}

namespace Fix {

template <unsigned N>
struct Tag
{
    static constexpr auto Hash = make_tag(typename gen_seq<N>::type{});
};

} /* Fix */

const char* find_tag1(const char* begin, const char* end) noexcept
{
    constexpr const char Tag[] = "\001151=";
    printf("Tag = %.*s\n", (int)sizeof(Tag), &Tag[0]);
    return std::search(begin, end, std::begin(Tag), std::end(Tag));
}

const char* find_tag2(const char* begin, const char* end) noexcept
{
    constexpr const auto Tag = Fix::Tag<151>::Hash;
    printf("Tag = %.*s\n", (int)sizeof(Tag), &Tag[0]);
    return std::search(begin, end, std::begin(Tag), std::end(Tag));
}

int main(int argc, char** argv)
{
    std::string msg = "8=FIX.4.2|9=253|35=8|128=XYZ|34=124|49=CCG|56=ABC_DEFG04|52=20100208-18:51:42|55=RRC|37=NF 0015/02082010|11=NF 0015/02082010|17=0|20=0|39=0|150=0|54=2|38=1000|40=2|44=55.3600|59=0|31=0|32=0|14=0|6=0|151=1000|60=20100208-18:51:42|58=New order|30=N|1=ABC123ZYX|207=N|7999=Peter's Custom Tag That Has Data|47=A|10=037|";
    std::replace(msg.begin(), msg.end(), '|', '\001');

    auto result1 = find_tag1(&*msg.begin(), &*msg.end());
    auto result2 = find_tag1(&*msg.begin(), &*msg.end());
    assert(result1 == result2);

    return 0;
}
