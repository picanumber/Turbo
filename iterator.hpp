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

#ifndef ITERATOR_HPP
#define	ITERATOR_HPP

#include "core.hpp"


namespace tml
{
    template<typename VALUE>
    struct iterator
    {
        using value = VALUE;
    };
    
    template<typename ITERATOR>
    using dereference = typename ITERATOR::value;

    template<typename VALUE , typename RIGHT>
    struct forward_iterator : public iterator<VALUE>
    {
        using right = RIGHT;
    };

    template<typename LEFT , typename VALUE>
    struct backward_iterator : public iterator<VALUE>
    {
        using left = LEFT;
    };

    template<typename LEFT , typename VALUE , typename RIGHT>
    struct bidirectional_iterator : public iterator<VALUE>
    {
        using right = RIGHT;
        using left = LEFT;
    };
    
    
    template<typename VALUE , typename RIGHT>
    using make_forward_iterator = tml::function<tml::forward_iterator<VALUE,RIGHT>>;
    
    template<typename LEFT , typename VALUE>
    using make_backward_iterator = tml::function<tml::backward_iterator<LEFT,VALUE>>;
    
    template<typename LEFT , typename VALUE , typename RIGHT>
    using make_bidirectional_iterator = tml::function<tml::bidirectional_iterator<LEFT,VALUE,RIGHT>>;
    


    template<typename T>
    struct begin_t;

    template<typename T>
    struct end_t;

    template<typename T>
    struct rbegin_t;

    template<typename T>
    struct rend_t;

    template<typename T , typename INCREMENT = tml::no_type>
    struct previous_t;

    template<typename T , typename INCREMENT = tml::no_type>
    struct next_t;
   
    
    template<typename T>
    using begin = typename tml::begin_t<T>::result;
    
    template<typename T>
    using end = typename tml::end_t<T>::result;
    
    template<typename T>
    using rbegin = typename tml::rbegin_t<T>::result;
    
    template<typename T>
    using rend = typename tml::rend_t<T>::result;
    
    template<typename T>
    using previous = typename tml::previous_t<T>::result;
    
    template<typename T , typename INCREMENT = tml::no_type>
    using next = typename tml::next_t<T,INCREMENT>::result;

    template<typename T>
    using first = typename tml::begin<T>::value;

    template<typename T>
    using last = typename tml::rbegin<T>::value;
    
    
    //Apaño:
    template<typename LEFT , typename VALUE>
    struct next_t<tml::backward_iterator<LEFT,VALUE>> : public tml::function<tml::previous<tml::backward_iterator<LEFT,VALUE>>> {};
    

    template<typename VALUE1 , typename VALUE2>
    struct equal_t<tml::iterator<VALUE1> , tml::iterator<VALUE2>> : public tml::function<tml::equal<VALUE1 , VALUE2>> {};

    template<typename VALUE1 , typename RIGHT1 , typename VALUE2 , typename RIGHT2>
    struct equal_t<tml::forward_iterator<VALUE1,RIGHT1> , forward_iterator<VALUE2,RIGHT2>> : public tml::function<tml::logical_and<equal<VALUE1,VALUE2> , tml::equal<RIGHT1,RIGHT2>>> {};

    template<typename VALUE1 , typename LEFT1 , typename VALUE2 , typename LEFT2>
    struct equal_t<tml::backward_iterator<LEFT1 , VALUE1> , backward_iterator<LEFT2,VALUE2>> : public tml::function<tml::logical_and<equal<VALUE1,VALUE2> , tml::equal<LEFT1,LEFT2>>> {};

    template<typename LEFT1 , typename VALUE1 , typename RIGHT1 , typename LEFT2 , typename VALUE2 , typename RIGHT2>
    struct equal_t<tml::bidirectional_iterator<LEFT1,VALUE1,RIGHT1> , tml::bidirectional_iterator<LEFT2,VALUE2,RIGHT2>> : public tml::function<decltype( tml::equal<LEFT1,LEFT2>() && tml::equal<VALUE1,VALUE2>() && tml::equal<RIGHT1,RIGHT2>())> {};


    namespace
    {   
        template<typename ITERATOR , typename DISTANCE>
        struct _add_t;
        
        template<typename ITERATOR , std::size_t DISTANCE>
        struct _add_t<ITERATOR,tml::size_t<DISTANCE>>
        {
            using result = typename _add_t<tml::next<ITERATOR> , tml::size_t<DISTANCE-1>>::result;
        };
        
        template<typename ITERATOR>
        struct _add_t<ITERATOR,tml::size_t<0>>
        {
            using result = ITERATOR;
        };
        
        
        template<typename ITERATOR , typename DISTANCE>
        struct _sub_t;
        
        template<typename ITERATOR , std::size_t DISTANCE>
        struct _sub_t<ITERATOR,tml::size_t<DISTANCE>>
        {
            using result = typename _sub_t<tml::previous<ITERATOR> , tml::size_t<DISTANCE-1>>::result;
        };
        
        template<typename ITERATOR>
        struct _sub_t<ITERATOR,tml::size_t<0>>
        {
            using result = ITERATOR;
        };
    }
    
    template<typename VALUE , typename RIGHT , std::size_t DISTANCE>
    struct add_t<tml::forward_iterator<VALUE,RIGHT>,tml::size_t<DISTANCE>> : public tml::function<typename _add_t<tml::forward_iterator<VALUE,RIGHT>,tml::size_t<DISTANCE>>::result> {};
    
    template<typename LEFT , typename VALUE , std::size_t DISTANCE>
    struct sub_t<tml::backward_iterator<LEFT,VALUE>,tml::size_t<DISTANCE>> : public tml::function<typename _sub_t<tml::backward_iterator<LEFT,VALUE>,tml::size_t<DISTANCE>>::result> {};
    
    
}

#endif	/* ITERATOR_HPP */

