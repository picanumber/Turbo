/******************************************************************************
* Turbo C++11 metaprogramming Library                                         *
*                                                                             *
* Copyright (C) 2013 - 2014, Manuel Sánchez Pérez                             *
*                                                                             *
* This file is part of The Turbo Library.                                     *
*                                                                             *
* The Turbo Library is free software: you can redistribute it and/or modify   *
* it under the terms of the GNU Lesser General Public License as published by *
* the Free Software Foundation, version 2 of the License.                     *
*                                                                             *
* The Turbo Library is distributed in the hope that it will be useful,        *
* but WITHOUT ANY WARRANTY; without even the implied warranty of              * 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
* GNU Lesser General Public License for more details.                         *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with The Turbo Library. If not, see <http://www.gnu.org/licenses/>.   *
******************************************************************************/

#ifndef FIXED_POINT_HPP
#define	FIXED_POINT_HPP

#include "core.hpp"
#include "iterator.hpp"
#include "to_string.hpp"

#include <cmath> //std::pow())


namespace tml
{
    template<int base , int exponent>
    struct positive_pow : public tml::long_long_integer<base * positive_pow<base,exponent-1>::value> {};

    template<int base>
    struct positive_pow<base,0> : public tml::long_long_integer<1> {};

    template<long long int number , int shift>
    struct decimal_leftshift : public tml::long_long_integer<number * positive_pow<10, shift>::value> {};

    template<long long int number , int shift>
    struct decimal_rightshift : public tml::long_long_integer<number / positive_pow<10, shift>::value> {};

    template<bool CONDITION , int NUMBER , int SHIFT>
    struct decimal_shift_chooser
    {
        using shifter = decimal_leftshift<NUMBER,SHIFT>;
    };

    template<long long NUMBER , int SHIFT>
    struct decimal_shift_chooser<false,NUMBER,SHIFT>
    {
        using shifter = decimal_rightshift<NUMBER,-SHIFT>;
    };

    template<long long number , int shift>
    struct decimal_shift
    {
        using shifter = typename decimal_shift_chooser<( shift >= 0 ) , number , shift>::shifter;
        static const long long int value = shifter::value;
    };

    /* Fixed-point implementation: */

    using fixed_point_bits      = long long int;
    using fixed_point_precision = unsigned int;

#ifndef MPL_CUSTOM_FIXED_POINT_DEFAULT_PRECISION
    const fixed_point_precision DEFAULT_FRACTIONAL_PRECISION = 8; 
#else
    const fixed_point_precision DEFAULT_FRACTIONAL_PRECISION = MPL_CUSTOM_FIXED_POINT_DEFAULT_PRECISION; 
#endif

    template<fixed_point_bits BITS , fixed_point_precision PRECISION = DEFAULT_FRACTIONAL_PRECISION>
    struct fixed_point
    {
        operator float()
        {
            return (float)BITS * std::pow(10.0f,-(float)PRECISION);
        };
    };

    template<long long int mantissa , int exponent = 0 , fixed_point_precision PRECISION = tml::DEFAULT_FRACTIONAL_PRECISION> // MANTISSA x 10^EXPONENT
    using decimal = tml::fixed_point<decimal_shift<mantissa , PRECISION + exponent>::value , PRECISION>; 


    template<fixed_point_bits BITS , fixed_point_precision PRECISION>
    struct zero_t<tml::fixed_point<BITS,PRECISION>> : public tml::function<tml::fixed_point<0,PRECISION>> {};

    template<fixed_point_bits BITS , fixed_point_precision PRECISION>
    struct one_t<tml::fixed_point<BITS,PRECISION>> : public tml::function<fixed_point<decimal_leftshift<1,PRECISION>::value,PRECISION>> {};

    
    /* Comparison operator */
    
    template<fixed_point_bits BITS1 , fixed_point_bits BITS2 , fixed_point_precision P1 , fixed_point_precision P2>
    struct equal_t<tml::fixed_point<BITS1,P1> , tml::fixed_point<BITS2,P2>> : public tml::function<tml::boolean<BITS1 == BITS2 && P1 == P2>> {};

    /* Arithmetic operators */

    template<fixed_point_bits BITS1 , fixed_point_bits BITS2 , fixed_point_precision PRECISION>
    struct add_t<tml::fixed_point<BITS1,PRECISION> , tml::fixed_point<BITS2,PRECISION>> : public tml::function<fixed_point<BITS1+BITS2 , PRECISION>> {};

    template<fixed_point_bits BITS1 , fixed_point_bits BITS2 , fixed_point_precision PRECISION>
    struct sub_t<tml::fixed_point<BITS1,PRECISION> , tml::fixed_point<BITS2,PRECISION>> : public tml::function<fixed_point<BITS1-BITS2 , PRECISION>> {};

    template<fixed_point_bits BITS1 , fixed_point_bits BITS2 , fixed_point_precision PRECISION>
    struct mul_t<tml::fixed_point<BITS1,PRECISION> , tml::fixed_point<BITS2,PRECISION>> : public tml::function<fixed_point<decimal_rightshift<BITS1*BITS2,PRECISION>::value , PRECISION>> {};

    template<fixed_point_bits BITS1 , fixed_point_bits BITS2 , fixed_point_precision PRECISION>
    struct div_t<tml::fixed_point<BITS1,PRECISION> , tml::fixed_point<BITS2,PRECISION>> : public tml::function<fixed_point<decimal_leftshift<BITS1,PRECISION>::value/BITS2 , PRECISION>> {};
    
    
    /* Operations between integral and fixed-point values */
    
    template<typename T , T v , fixed_point_bits BITS , fixed_point_precision PRECISION>
    struct add_t<tml::value_t<T,v>,tml::fixed_point<BITS,PRECISION>> : public tml::function<tml::add<tml::decimal<(fixed_point_bits)v,0,PRECISION>,tml::fixed_point<BITS,PRECISION>>> {};

    template<typename T , T v , fixed_point_bits BITS , fixed_point_precision PRECISION>
    struct add_t<tml::fixed_point<BITS,PRECISION>,tml::value_t<T,v>> : public tml::function<tml::add<tml::fixed_point<BITS,PRECISION>,tml::decimal<(fixed_point_bits)v,0,PRECISION>>> {};
    
    
    template<typename T , T v , fixed_point_bits BITS , fixed_point_precision PRECISION>
    struct sub_t<tml::value_t<T,v>,tml::fixed_point<BITS,PRECISION>> : public tml::function<tml::sub<tml::decimal<(fixed_point_bits)v,0,PRECISION>,tml::fixed_point<BITS,PRECISION>>> {};

    template<typename T , T v , fixed_point_bits BITS , fixed_point_precision PRECISION>
    struct sub_t<tml::fixed_point<BITS,PRECISION>,tml::value_t<T,v>> : public tml::function<tml::sub<tml::fixed_point<BITS,PRECISION>,tml::decimal<(fixed_point_bits)v,0,PRECISION>>> {};
    
    
    template<typename T , T v , fixed_point_bits BITS , fixed_point_precision PRECISION>
    struct mul_t<tml::value_t<T,v>,tml::fixed_point<BITS,PRECISION>> : public tml::function<tml::mul<tml::decimal<(fixed_point_bits)v,0,PRECISION>,tml::fixed_point<BITS,PRECISION>>> {};

    template<typename T , T v , fixed_point_bits BITS , fixed_point_precision PRECISION>
    struct mul_t<tml::fixed_point<BITS,PRECISION>,tml::value_t<T,v>> : public tml::function<tml::mul<tml::fixed_point<BITS,PRECISION>,tml::decimal<(fixed_point_bits)v,0,PRECISION>>> {};
    
    
    template<typename T , T v , fixed_point_bits BITS , fixed_point_precision PRECISION>
    struct div_t<tml::value_t<T,v>,tml::fixed_point<BITS,PRECISION>> : public tml::function<tml::div<tml::decimal<(fixed_point_bits)v,0,PRECISION>,tml::fixed_point<BITS,PRECISION>>> {};

    template<typename T , T v , fixed_point_bits BITS , fixed_point_precision PRECISION>
    struct div_t<tml::fixed_point<BITS,PRECISION>,tml::value_t<T,v>> : public tml::function<tml::div<tml::fixed_point<BITS,PRECISION>,tml::decimal<(fixed_point_bits)v,0,PRECISION>>> {};




    /* mpl::to_string */
    
    template<fixed_point_bits BITS , fixed_point_precision PRECISION>
    struct to_string_t<tml::fixed_point<BITS,PRECISION>>
    {
        operator std::string() const
        {
            std::ostringstream os;
            os << tml::fixed_point<BITS,PRECISION>();
            return os.str();
        }
    };
}

#endif	/* FIXED_POINT_HPP */

