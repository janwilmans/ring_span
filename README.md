# ring_span

Another search for the one ring.
Thanks to Björn Fahller for writing the ring_span! I only had to slightly modify the original code.


features:

* mostly constrexpr (with some obvious exceptions)
* works on top of an existing container of choise
* allows insertion and removal without construction or destruction of contained objects

restrictions:

* requires to be used on contains that has a size that is a power of 2

The restriction allows the span to be slightly more efficient.
