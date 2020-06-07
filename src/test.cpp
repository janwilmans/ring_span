
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "ring_span.h"
#include <vector>
#include <array>

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

TEST_CASE("push_back_1_on_default_construct", "[nostd::ring_span]")
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

TEST_CASE("explicit_size_on_construction", "[nostd::ring_span]")
{
    std::array<Foo, 4> v;
    nostd::ring_span rs(v.data(), v.size());

    CHECK(rs.size() == 0);
    CHECK(rs.capacity() == 4);
}

TEST_CASE("deduce_size_on_construction", "[nostd::ring_span]")
{
    std::array<Foo, 4> v;
    nostd::ring_span rs(v);

    CHECK(rs.size() == 0);
    CHECK(rs.capacity() == 4);
}

TEST_CASE("push_back_n_on_default_construct", "[nostd::ring_span]")
{
    std::array<Foo, 4> v;
    nostd::ring_span rs(v.data(), v.size());

    CHECK(rs.size() == 0);
    CHECK(rs.capacity() == 4);

    rs.push_back() = 1;
    CHECK(rs.front().value == 1);
    CHECK(rs.back().value == 1);
    CHECK(rs.size() == 1);

    rs.push_back() = 2;
    CHECK(rs.front().value == 1);
    CHECK(rs.back().value == 2);
    CHECK(rs.size() == 2);

    rs.push_back() = 3;
    CHECK(rs.front().value == 1);
    CHECK(rs.back().value == 3);
    CHECK(rs.size() == 3);

    rs.push_back() = 4;
    CHECK(rs[0].value == 1);
    CHECK(rs[1].value == 2);
    CHECK(rs[2].value == 3);
    CHECK(rs[3].value == 4);
    CHECK(rs.size() == 4);

    rs.push_back() = 5;
    CHECK(rs.size() == 4);
    CHECK(rs[0].value == 2);
    CHECK(rs[1].value == 3);
    CHECK(rs[2].value == 4);
    CHECK(rs[3].value == 5);

    rs.pop_front();
    CHECK(rs.size() == 3);
    CHECK(rs.front().value == 3);
    CHECK(rs.back().value == 5);
    CHECK(rs[0].value == 3);
    CHECK(rs[1].value == 4);
    CHECK(rs[2].value == 5);

    rs.push_back() = 6;
    CHECK(rs.size() == 4);
    CHECK(rs.front().value == 3);
    CHECK(rs.back().value == 6);
    CHECK(rs[0].value == 3);
    CHECK(rs[1].value == 4);
    CHECK(rs[2].value == 5);
    CHECK(rs[3].value == 6);

    // start pop back to empty
    rs.pop_front();
    CHECK(rs.size() == 3);
    CHECK(rs.front().value == 4);
    CHECK(rs.back().value == 6);
    CHECK(rs[0].value == 4);
    CHECK(rs[1].value == 5);
    CHECK(rs[2].value == 6);

    rs.pop_front();
    CHECK(rs.size() == 2);
    CHECK(rs.front().value == 5);
    CHECK(rs.back().value == 6);
    CHECK(rs[0].value == 5);
    CHECK(rs[1].value == 6);

    rs.pop_front();
    CHECK(rs.size() == 1);
    CHECK(rs.front().value == 6);
    CHECK(rs.back().value == 6);
    CHECK(rs[0].value == 6);

    rs.pop_front();
    CHECK(rs.size() == 0);
    rs.pop_front();
    CHECK(rs.size() == 0);
}

TEST_CASE("array_n_on_default_construct", "[nostd::ring_span]")
{
    std::array<int, 4> a;
    nostd::ring_span r(a);
    assert(r.size() == 0);
    r.push_back(1);
    assert(r.size() == 1);
    assert(r.front() == 1);
    assert(r.back() == 1);
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
    CHECK(rs.size() == rs.capacity());

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
    CHECK(rs.size() == 0);
    CHECK(rs.capacity() == 256);

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

// example added exceptions for error handling
template <typename T>
void safe_push(nostd::ring_span<T> rs, T v)
{
    if (rs.size() == rs.capacity())
    {
        throw std::runtime_error("safe_push exceeds span capacity");
    }
    rs.push_back(v);
}

TEST_CASE("safe_push", "[nostd::ring_span]")
{
    std::vector<Foo> v;
    v.resize(256);
    init_counters();
    nostd::ring_span<Foo> rs(v.data(), v.size());
    safe_push<Foo>(rs, 42);
}
