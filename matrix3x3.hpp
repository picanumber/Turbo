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

#ifndef MATRIX3X3_HPP
#define	MATRIX3X3_HPP

#include "core.hpp"
#include "to_string.hpp"
#include "trigonometry.hpp"
#include "vector.hpp"
#include "transformations.hpp"

#include <sstream>
#include <string>


namespace math
{
    template<typename M11 , typename M12 , typename M13 ,
             typename M21 , typename M22 , typename M23 ,
             typename M31 , typename M32 , typename M33>
    struct matrix3x3
    {
        using m11 = M11; using m12 = M12; using m13 = M13;
        using m21 = M21; using m22 = M22; using m23 = M23;
        using m31 = M31; using m32 = M32; using m33 = M33;
    };


    /* unitary matrix */

    template<typename T>
    using unity3x3 = math::matrix3x3<tml::one<T>  , tml::zero<T> , tml::zero<T> ,
                                     tml::zero<T> , tml::one<T>  , tml::zero<T> ,
                                     tml::zero<T> , tml::zero<T> , tml::one<T>>;


    /* 2d transformations */

    template<typename X , typename Y>
    struct translate_t<math::vec2<X,Y>> : public tml::function<math::matrix3x3<tml::one<X>  , tml::zero<X> , X ,
                                                                               tml::zero<X> , tml::one<X>  , Y ,
                                                                               tml::zero<X> , tml::zero<X> , tml::one<X>>>
    {};

    template<typename SCALE>
    struct scale_t : public tml::function<math::matrix3x3<       SCALE     , tml::zero<SCALE> , tml::zero<SCALE> ,
                                                          tml::zero<SCALE> ,       SCALE      , tml::zero<SCALE> ,
                                                          tml::zero<SCALE> , tml::zero<SCALE> ,       SCALE>>
    {};

    template<typename X , typename Y>
    struct scale_t<math::vec2<X,Y>> : public tml::function<math::matrix3x3<      X      , tml::zero<X> , tml::zero<X> ,
                                                                           tml::zero<X> ,       Y      , tml::zero<X> ,
                                                                           tml::zero<X> , tml::zero<X> , tml::one<X>>>
    {};

    template<typename ANGLE,typename TRIGONOMETRY_APROX_TERMS>
    struct rotate_t<ANGLE,math::rotation_2d,TRIGONOMETRY_APROX_TERMS> 
    : public tml::function<math::matrix3x3<math::cos<ANGLE,TRIGONOMETRY_APROX_TERMS>                , math::sin<ANGLE,TRIGONOMETRY_APROX_TERMS>  , tml::zero<ANGLE> ,
                                           tml::opposite<math::sin<ANGLE,TRIGONOMETRY_APROX_TERMS>> , math::cos<ANGLE,TRIGONOMETRY_APROX_TERMS>  , tml::zero<ANGLE> ,
                                           tml::zero<ANGLE>                                         ,               tml::zero<ANGLE>             , tml::one<ANGLE>>> 
    {};
        
        

}

namespace tml
{
    /* to string function specialization */

    template<typename M11 , typename M12 , typename M13 ,
             typename M21 , typename M22 , typename M23 ,
             typename M31 , typename M32 , typename M33>

    struct to_string_t<math::matrix3x3<M11,M12,M13,
                                       M21,M22,M23,
                                       M31,M32,M33>>
    {
        operator std::string()
        {
            std::ostringstream os;

            os << "|" << tml::to_string<M11>() << "," << tml::to_string<M12>() << "," << tml::to_string<M13>() << "|" << std::endl;
            os << "|" << tml::to_string<M21>() << "," << tml::to_string<M22>() << "," << tml::to_string<M23>() << "|" << std::endl;
            os << "|" << tml::to_string<M31>() << "," << tml::to_string<M32>() << "," << tml::to_string<M33>() << "|";

            return os.str();
        }
    };



    /* Arithmetic functions */


    // matrix + matrix:

    template<typename LHS11 , typename LHS12 , typename LHS13 ,
             typename LHS21 , typename LHS22 , typename LHS23 ,
             typename LHS31 , typename LHS32 , typename LHS33 ,

             typename RHS11 , typename RHS12 , typename RHS13 ,
             typename RHS21 , typename RHS22 , typename RHS23 ,
             typename RHS31 , typename RHS32 , typename RHS33>
    struct add_t<
                 math::matrix3x3<LHS11,LHS12,LHS13,
                                 LHS21,LHS22,LHS23,
                                 LHS31,LHS32,LHS33> , 

                 math::matrix3x3<RHS11,RHS12,RHS13,
                                 RHS21,RHS22,RHS23,
                                 RHS31,RHS32,RHS33>
                > : public tml::function<math::matrix3x3<tml::add<LHS11,RHS11> , tml::add<LHS12,RHS12> , tml::add<LHS13,RHS13> , 
                                                         tml::add<LHS21,RHS21> , tml::add<LHS22,RHS22> , tml::add<LHS23,RHS23> , 
                                                         tml::add<LHS31,RHS31> , tml::add<LHS32,RHS32> , tml::add<LHS33,RHS33>>> 
    {};


    // matrix - matrix:

    template<typename LHS11 , typename LHS12 , typename LHS13 ,
             typename LHS21 , typename LHS22 , typename LHS23 ,
             typename LHS31 , typename LHS32 , typename LHS33 ,

             typename RHS11 , typename RHS12 , typename RHS13 ,
             typename RHS21 , typename RHS22 , typename RHS23 ,
             typename RHS31 , typename RHS32 , typename RHS33>
    struct sub_t<
                 math::matrix3x3<LHS11,LHS12,LHS13,
                                 LHS21,LHS22,LHS23,
                                 LHS31,LHS32,LHS33> , 

                 math::matrix3x3<RHS11,RHS12,RHS13,
                                 RHS21,RHS22,RHS23,
                                 RHS31,RHS32,RHS33>
                > : public tml::function<math::matrix3x3<tml::sub<LHS11,RHS11> , tml::sub<LHS12,RHS12> , tml::sub<LHS13,RHS13> , 
                                                         tml::sub<LHS21,RHS21> , tml::sub<LHS22,RHS22> , tml::sub<LHS23,RHS23> , 
                                                         tml::sub<LHS31,RHS31> , tml::sub<LHS32,RHS32> , tml::sub<LHS33,RHS33>>> 
    {};


    // matrix * scalar:

    template<typename LHS11 , typename LHS12 , typename LHS13 ,
             typename LHS21 , typename LHS22 , typename LHS23 ,
             typename LHS31 , typename LHS32 , typename LHS33 , 

             typename RHS>

    struct mul_t< 
                 math::matrix3x3<LHS11,LHS12,LHS13,
                                 LHS21,LHS22,LHS23,
                                 LHS31,LHS32,LHS33> , 
                 RHS
                > : public tml::function<math::matrix3x3<tml::mul<LHS11,RHS> , tml::mul<LHS12,RHS> , tml::mul<LHS13,RHS> , 
                                                         tml::mul<LHS21,RHS> , tml::mul<LHS22,RHS> , tml::mul<LHS23,RHS> , 
                                                         tml::mul<LHS31,RHS> , tml::mul<LHS32,RHS> , tml::mul<LHS33,RHS>>> 
    {};


    // scalar * matrix:

    template<typename RHS11 , typename RHS12 , typename RHS13 ,
             typename RHS21 , typename RHS22 , typename RHS23 ,
             typename RHS31 , typename RHS32 , typename RHS33 , 

             typename LHS>

    struct mul_t<LHS ,
                 math::matrix3x3<RHS11,RHS12,RHS13,
                                 RHS21,RHS22,RHS23,
                                 RHS31,RHS32,RHS33>
                > : public tml::function<math::matrix3x3<tml::mul<RHS11,LHS> , tml::mul<RHS12,LHS> , tml::mul<RHS13,LHS> , 
                                                         tml::mul<RHS21,LHS> , tml::mul<RHS22,LHS> , tml::mul<RHS23,LHS> , 
                                                         tml::mul<RHS31,LHS> , tml::mul<RHS32,LHS> , tml::mul<RHS33,LHS>>> 
    {};


    //matrix / scalar:

    template<typename LHS11 , typename LHS12 , typename LHS13 ,
             typename LHS21 , typename LHS22 , typename LHS23 ,
             typename LHS31 , typename LHS32 , typename LHS33 , 

             typename RHS>

    struct div_t< 
                 math::matrix3x3<LHS11,LHS12,LHS13,
                                 LHS21,LHS22,LHS23,
                                 LHS31,LHS32,LHS33> , 
                 RHS
                > : public tml::function<math::matrix3x3<decltype(LHS11()/RHS()) , decltype(LHS12()/RHS()) , tml::div<LHS13,RHS> , 
                                                         tml::div<LHS21,RHS> , tml::div<LHS22,RHS> , tml::div<LHS23,RHS> , 
                                                         tml::div<LHS31,RHS> , tml::div<LHS32,RHS> , tml::div<LHS33,RHS>>> 
    {};


    // matrix * matrix

    template<typename LHS11 , typename LHS12 , typename LHS13 ,
             typename LHS21 , typename LHS22 , typename LHS23 ,
             typename LHS31 , typename LHS32 , typename LHS33 ,

             typename RHS11 , typename RHS12 , typename RHS13 ,
             typename RHS21 , typename RHS22 , typename RHS23 ,
             typename RHS31 , typename RHS32 , typename RHS33>
    struct mul_t<
                 math::matrix3x3<LHS11,LHS12,LHS13,
                                 LHS21,LHS22,LHS23,
                                 LHS31,LHS32,LHS33> , 

                 math::matrix3x3<RHS11,RHS12,RHS13,
                                 RHS21,RHS22,RHS23,
                                 RHS31,RHS32,RHS33>
                > : public tml::function<math::matrix3x3<decltype( LHS11()*RHS11() + LHS12()*RHS21() + LHS13()*RHS31() ) , //M11
                                                         decltype( LHS11()*RHS12() + LHS12()*RHS22() + LHS13()*RHS32() ) , //M12
                                                         decltype( LHS11()*RHS13() + LHS12()*RHS23() + LHS13()*RHS33() ) , //M13

                                                         decltype( LHS21()*RHS11() + LHS22()*RHS21() + LHS23()*RHS31() ) , //M21
                                                         decltype( LHS21()*RHS12() + LHS22()*RHS22() + LHS23()*RHS32() ) , //M22
                                                         decltype( LHS21()*RHS13() + LHS22()*RHS23() + LHS23()*RHS33() ) , //M23

                                                         decltype( LHS31()*RHS11() + LHS32()*RHS21() + LHS33()*RHS31() ) , //M31
                                                         decltype( LHS31()*RHS12() + LHS32()*RHS22() + LHS33()*RHS32() ) , //M32
                                                         decltype( LHS31()*RHS13() + LHS32()*RHS23() + LHS33()*RHS33() )>> //M33
    {};


    // matrix * vector (Apply transform):

    template<typename LHS11 , typename LHS12 , typename LHS13 ,
             typename LHS21 , typename LHS22 , typename LHS23 ,
             typename LHS31 , typename LHS32 , typename LHS33 , 

             typename X , typename Y>

    struct mul_t< 
                 math::matrix3x3<LHS11,LHS12,LHS13,
                                 LHS21,LHS22,LHS23,
                                 LHS31,LHS32,LHS33> , 
            math::vec2<X,Y>
                > : public tml::function<math::vec2<decltype( LHS11()*X() + LHS12()*Y() + LHS13() ),
                                                    decltype( LHS21()*X() + LHS22()*Y() + LHS23() )>>
    {};
    
    
    
    /* mpl::to_list specialization: */
    
    template<typename M11 , typename M12 , typename M13 ,
             typename M21 , typename M22 , typename M23 ,
             typename M31 , typename M32 , typename M33>
    struct to_list_t<math::matrix3x3<M11,M12,M13,
                                     M21,M22,M23,
                                     M31,M32,M33>>
    : public tml::function<tml::list<M11,M12,M13,M21,M22,M23,M31,M32,M33>> 
    {};
    
    
    /* mpl::from_list specialization: */
    
    template<typename M11 , typename M12 , typename M13 ,
             typename M21 , typename M22 , typename M23 ,
             typename M31 , typename M32 , typename M33>
    struct from_list_t<tml::list<M11,M12,M13,M21,M22,M23,M31,M32,M33>>
    : public tml::function<math::matrix3x3<M11,M12,M13,
                                           M21,M22,M23,
                                           M31,M32,M33>
                          >
    {};
    
}
#endif	/* MATRIX3X3_HPP */

