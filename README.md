# ring_span

Another search for the one ring.
Thanks to Björn Fahller for writing the ring_span! I only had to slightly modify the original code.

features:

* mostly constrexpr (with some obvious exceptions)
* works on top of an existing container of choise
* allows insertion and removal without construction or destruction of contained objects
* pushing a value beyond the capacity of the span overwrites the oldest value

restrictions:

* requires to be used on contains that has a size that is a power of 2

The restriction allows the span to be slightly more efficient.

design considerations:

* the span overwrites the oldest value if a value is pushed in when its full
* keep the span simple with minimal error handling, if needed it can layered on top, see safe_push

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



