
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "ring_span.h"
#include <vector>

static int& construct_count()
{
    static int value = 0;
    return value;
}

static int& destruct_count()
{
    static int value = 0;
    return value;
}

void init_counters()
{
    construct_count() = 0;
    destruct_count() = 0;
}

void verify_counters()
{
    CHECK(construct_count() == 0);
    CHECK(destruct_count() == 0);
}

struct Foo
{
    Foo()
    {
        construct_count()++;
    }

    Foo(int value) :
        value(value)
    {
        construct_count()++;
    }

    Foo(const Foo& foo)
    {
        construct_count()++;
        this->value = foo.value;
    }

    Foo(Foo&& foo)
    {
        construct_count()++;
        this->value = foo.value;
    }

    Foo& operator=(const Foo&) = default;
    Foo& operator=(Foo&&) = default;

    ~Foo()
    {
        destruct_count()++;
    }

    int value;
};

TEST_CASE("vector", "[nostd::ring_span]")
{
    std::vector<Foo> v;
    v.resize(256);
    init_counters();
    nostd::ring_span<Foo> rs(v.data(), v.size());
    verify_counters();
}

TEST_CASE("default construct", "[nostd::ring_span]")
{
    std::vector<Foo> v;
    v.resize(256);
    nostd::ring_span<Foo> rs(v.data(), v.size());
    CHECK(rs.size() == 0);
}

TEST_CASE("pop_front_on_default_construct", "[nostd::ring_span]")
{
    std::vector<Foo> v;
    v.resize(256);
    init_counters();

    nostd::ring_span<Foo> rs(v.data(), v.size());
    rs.pop_front();
    CHECK(rs.size() == 0);
    verify_counters();
}

TEST_CASE("push_back_on_default_construct", "[nostd::ring_span]")
{
    std::vector<Foo> v;
    v.resize(256);
    init_counters();
    nostd::ring_span<Foo> rs(v.data(), v.size());
    rs.push_back();
    CHECK(rs.size() == 1);
    verify_counters();
}

TEST_CASE("push_back_n_on_default_construct", "[nostd::ring_span]")
{
    std::vector<Foo> v;
    v.resize(256);
    init_counters();
    nostd::ring_span<Foo> rs(v.data(), v.size());
    rs.push_back(42);

    CHECK(construct_count() == 1);
    CHECK(destruct_count() == 1);
    CHECK(rs.size() == 1);
}

TEST_CASE("three_values", "[nostd::ring_span]")
{
    std::vector<Foo> v;
    v.resize(256);
    init_counters();
    nostd::ring_span<Foo> rs(v.data(), v.size());

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
    verify_counters();
}

TEST_CASE("roll over", "[nostd::ring_span]")
{
    std::vector<Foo> v;
    v.resize(256);
    init_counters();
    nostd::ring_span<Foo> rs(v.data(), v.size());

    for (int i = 0; i < 260; ++i)
    {
        auto& a = rs.push_back();
        a.value = i;
    }
    CHECK(rs.size() == 256);

    for (int i = 0; i < 256; ++i)
    {
        CHECK(rs.front().value == (4 + i));
        rs.pop_front();
    }
    CHECK(rs.size() == 0);
    verify_counters();
}

TEST_CASE("read empty", "[nostd::ring_span]")
{
    std::vector<Foo> v;
    v.resize(256);
    init_counters();
    nostd::ring_span<Foo> rs(v.data(), v.size());

    for (int i = 0; i < 5; ++i)
    {
        auto& a = rs.push_back();
        a.value = i;
    }

    CHECK(rs.size() == 5);
    for (int i = 0; i < 5; ++i)
    {
        rs.pop_front();
    }
    CHECK(rs.size() == 0);
    verify_counters();
}
