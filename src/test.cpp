
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "ring_span.h"
#include <vector>

struct Foo
{
    int value;
};

TEST_CASE("vector", "[ring_span]")
{
    std::vector<Foo> v;
    v.resize(256);
    ring_span<Foo> rs(v.data(), v.size());
}

TEST_CASE("three_values", "[ring_span]")
{
    std::vector<Foo> v;
    v.resize(256);
    ring_span<Foo> rs(v.data(), v.size());

    auto& a = rs.push_back();
    a.value = 1;
    auto& b = rs.push_back();
    b.value = 2;
    auto& c = rs.push_back();
    c.value = 3;

    CHECK(rs.front().value == 1);
    rs.pop_front();
    CHECK(rs.front().value == 2);
    rs.pop_front();
    CHECK(rs.front().value == 3);
    rs.pop_front();
}

TEST_CASE("roll over", "[ring_span]")
{
    std::vector<Foo> v;
    v.resize(256);
    ring_span<Foo> rs(v.data(), v.size());

    for (int i = 0; i < 260; ++i)
    {
        auto& a = rs.push_back();
        a.value = i;
    }

    for (int i = 0; i < 255; ++i)
    {
        CHECK(rs.front().value == (4 + i));
        rs.pop_front();
    }
}