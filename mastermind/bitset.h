// Copyright (c) 2013 Sami Väisänen, Ensisoft 
//
// http://www.ensisoft.com
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.

#pragma once

#include <cstdint>
#include <cassert>

namespace mastermind
{
    template<typename Enum, 
        typename Bits = std::uint32_t>
    class Bitset
    {
    public:
        enum {
            BitCount = sizeof(Bits) * 8
        };

        Bitset() : bits_(0)
        {}
        Bitset(Enum initial) : bits_(0)
        {
            set(initial);
        }
        Bitset(Bits value) : bits_(value)
        {}

        Bitset& set(Enum value, bool on = true) 
        {
            const auto b = bittify(value);

            if (on)
                bits_ |= b;
            else bits_ &= ~b;
            return *this;
        }

        Bitset& operator |= (Bitset other)
        {
            bits_ |= other.bits_;
            return *this;
        }

        Bitset& operator &= (Bitset other)
        { 
            bits_ &= other.bits_;
            return *this;
        }

        // test a particular value.
        bool test(Enum value) const
        { 
            const auto b = bittify(value); 
            return (bits_ & b) == b;
        }
        
        // test for any value.
        bool test(Bitset values) const 
        { return bits_ & values.bits_; }

        // test of the nth bith.
        bool test(unsigned index) const 
        {
            const auto b = bittify((Enum)index);
            return (bits_ & b);
        }

        void clear() 
        { bits_ = 0x0; }

        bool any_bit() const 
        { return bits_ != 0; }

        Bits value() const 
        { return bits_; }        

        void set_from_value(Bits b)
        { bits_ = b; }

    private:
        Bits bittify(Enum value) const 
        {
            assert((unsigned)value < BitCount &&
                "The value of enum member is too large to fit in the bitset."
                "You need to use larger underlying type.");
            return Bits(1) << Bits(value);
        }

    private:
        Bits bits_;
    };

    // we only provide this operator, since its global
    // this also covers Enum | Bitset<Enum> and Bitset<Enum> | Enum
    // through implicit conversion to Bitset<Enum>
    template<typename Enum, typename Bits>
    auto operator | (Bitset<Enum, Bits> lhs, Bitset<Enum, Bits> rhs) -> decltype(lhs)
    {
        return { lhs.value() | rhs.value() };
    }

    template<typename Enum, typename Bits>
    auto operator | (Bitset<Enum, Bits> lhs, Enum e) -> decltype(lhs)
    {
        return lhs | Bitset<Enum, Bits>(e);
    }

    template<typename Enum, typename Bits>
    auto operator & (Bitset<Enum, Bits> lhs, Bitset<Enum, Bits> rhs) -> decltype(lhs)
    {
        return Bitset<Enum, Bits>(lhs.value() & rhs.value());
    }

    template<typename Enum, typename Bits>
    auto operator & (Bitset<Enum, Bits> lhs, Enum e) -> decltype(lhs)
    {
        return lhs & Bitset<Enum>(e);
    }

} // mastermind


