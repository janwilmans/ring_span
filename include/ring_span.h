#pragma once

#include <type_traits>
#include <utility>
#include <cassert>
#include <cstddef>

namespace nostd {

constexpr bool has_single_bit(size_t n)
{
    return (n != 0) && (n & (n - 1)) == 0;
}

template <typename T>
class ring_span
{
public:
    template <size_t N>
    constexpr ring_span(T (&array)[N]) :
        m_storage_begin(array), m_storage_mask(N - 1)
    {
        static_assert(has_single_bit(N) == 0, "size must be a power of two");
    }

    template <typename C, typename = decltype(std::declval<C&>().data() + std::declval<C&>().size())>
    constexpr ring_span(C& c) :
        m_storage_begin(c.data()), m_storage_mask(c.size() - 1)
    {
        assert(has_single_bit(c.size()) && "size must be a power of two");
    }

    constexpr ring_span(T* p, size_t n) :
        m_storage_begin(p), m_storage_mask(n - 1)
    {
        assert(has_single_bit(n) && "size must be a power of two");
    }

    constexpr T& operator[](size_t n)
    {
        return m_storage_begin[(m_first + n) & m_storage_mask];
    }

    constexpr const T& operator[](size_t n) const
    {
        return m_storage_begin[(m_first + n) & m_storage_mask];
    }

    constexpr T& front()
    {
        return (*this)[0];
    }

    constexpr const T& front() const
    {
        return (*this)[0];
    }

    constexpr T& back()
    {
        return (*this)[m_size - 1];
    }

    constexpr const T& back() const
    {
        return (*this)[m_size - 1];
    }

    constexpr size_t size() const
    {
        return m_size;
    }

    constexpr size_t capacity() const
    {
        return m_storage_mask + 1;
    }

    T& push_back()
    {
        auto& rv = (*this)[m_size];
        if ((m_size & m_storage_mask) == m_size)
        {
            ++m_size;
        }
        else
        {
            ++m_first;
        }
        return rv;
    }

    constexpr T& push_back(const T& t)
    {
        auto& rv = (*this)[m_size] = t;
        if ((m_size & m_storage_mask) == m_size)
        {
            ++m_size;
        }
        else
        {
            ++m_first;
        }
        return rv;
    }

    constexpr T& push_back(T&& t)
    {
        auto& rv = (*this)[m_size] = std::move(t);
        if ((m_size & m_storage_mask) == m_size)
        {
            ++m_size;
        }
        else
        {
            ++m_first;
        }
        return rv;
    }

    constexpr void pop_front()
    {
        if (m_size == 0)
        {
            return;
        }
        ++m_first;
        --m_size;
    }

    constexpr void clear()
    {
        m_size = 0;
    }

private:
    T* m_storage_begin;
    size_t m_storage_mask;
    size_t m_capacity;
    size_t m_first{};
    size_t m_size{};
};

template <typename T, size_t N>
ring_span(T (&)[N]) -> ring_span<T>;

template <typename C, typename P = decltype(std::declval<C&>().data())>
ring_span(C&) -> ring_span<std::remove_reference_t<std::remove_pointer_t<P>>>;

} // namespace nostd
