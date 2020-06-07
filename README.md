# ring_span

Another search for the one ring.
Thanks to Björn Fahller for writing the original code, I modified it to fit my use cases and extended it to make it have 'the usual methods' like capacity() and clear().

Usage:

```
    std::vector<int> v;
    v.resize(256);
    nostd::ring_span rs(v);
    rs.push_back(1);  // 'normal' push_back
```

```
    class Foo{ std::vector<int> values; } ;
    std::vector<Foo> v;
    v.resize(16);
    nostd::ring_span rs(v);
    Foo & foo = rs.push_back(); // modify / re-use existing elements without re-construction
```


features:

* mostly constrexpr (with some obvious exceptions)
* works on top of an existing container of choice, it must have .data() and .size()
* allows insertion and removal without construction or destruction of contained objects
* pushing a value beyond the capacity of the span overwrites the oldest value

restrictions:

* requires to be used on contains that has a size that is a power of 2

The restriction allows the span to be slightly more efficient.

design considerations:

* keep the span simple
* the span overwrites the oldest value if a value is pushed in when its full
* minimal error handling, if needed it can layered on top, see safe_push

```
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
```



