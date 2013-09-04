/* 
 * File:   iterator.hpp
 * Author: manu343726
 *
 * Created on 1 de septiembre de 2013, 19:38
 */

#ifndef ITERATOR_HPP
#define	ITERATOR_HPP

namespace mpl
{
    template<typename VALUE>
    struct iterator
    {
        using value = VALUE;
    };

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


    template<typename T>
    struct begin;

    template<typename T>
    struct end;

    template<typename T>
    struct rbegin;

    template<typename T>
    struct rend;

    template<typename T>
    struct previous;

    template<typename T>
    struct next;

    template<typename T>
    using first = typename mpl::begin<T>::value;

    template<typename T>
    using last = typename mpl::rbegin<T>::value;

    template<typename VALUE1 , typename VALUE2>
    struct equal<mpl::iterator<VALUE1> , mpl::iterator<VALUE2>> : public mpl::equal<VALUE1 , VALUE2> {};

    template<typename VALUE1 , typename RIGHT1 , typename VALUE2 , typename RIGHT2>
    struct equal<mpl::forward_iterator<VALUE1,RIGHT1> , forward_iterator<VALUE2,RIGHT2>> : public mpl::logical_and<equal<VALUE1,VALUE2> , mpl::equal<RIGHT1,RIGHT2>> {};

    template<typename VALUE1 , typename LEFT1 , typename VALUE2 , typename LEFT2>
    struct equal<mpl::backward_iterator<LEFT1 , VALUE1> , backward_iterator<LEFT2,VALUE2>> : public mpl::logical_and<equal<VALUE1,VALUE2> , mpl::equal<LEFT1,LEFT2>> {};

    template<typename LEFT1 , typename VALUE1 , typename RIGHT1 , typename LEFT2 , typename VALUE2 , typename RIGHT2>
    struct equal<mpl::bidirectional_iterator<LEFT1,VALUE1,RIGHT1> , mpl::bidirectional_iterator<LEFT2,VALUE2,RIGHT2>> : public mpl::logical_and<mpl::equal<LEFT1,LEFT2> , mpl::equal<VALUE1,VALUE2> , mpl::equal<RIGHT1,RIGHT2>> {};
}

#endif	/* ITERATOR_HPP */
