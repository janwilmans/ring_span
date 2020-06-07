# ring_span

Another search for the one ring.
Thanks to Björn Fahller for writing the ring_span! I only had to slightly modify the original code.

features:

* mostly constrexpr (with some obvious exceptions)
* works on top of an existing container of choice
* allows insertion and removal without construction or destruction of contained objects
* pushing a value beyond the capacity of the span overwrites the oldest value

restrictions:

* requires to be used on contains that has a size that is a power of 2

The restriction allows the span to be slightly more efficient.

design considerations:

* the span overwrites the oldest value if a value is pushed in when its full
* 

