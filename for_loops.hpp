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

#pragma once


#include "list.hpp"
#include "core.hpp"

namespace tml
{
    template<bool BREAK_CONDITION>
    struct loop_kernel
    {
        using abort = tml::boolean<BREAK_CONDITION>;
    };
    
    using no_abort_kernel = tml::loop_kernel<false>;
    
    namespace 
    {
        template<bool FLAG , typename A , typename B>
        struct _conditional;

        template<typename A , typename B>
        struct _conditional<true,A,B> : public tml::type_t<A> {};
        
        template<typename A , typename B>
        struct _conditional<false,A,B> : public tml::type_t<B> {};
        
        
        template<typename BEGIN , typename END , typename START_RESULT , template<typename,typename> class KERNEL , typename STEP>
        class __for_loop
        {
        private:
            template<typename CURRENT , typename PREVIOUS_RESULT , bool FINISHED>
            struct _for_loop
            {
                using kernel_result = typename KERNEL<typename CURRENT::value , PREVIOUS_RESULT>::result;
                
                /* lazy evaluation */
                
                template<typename LOOP_ABORTED , typename NON_GLOBAL_SPECIALIZATION_WORKAROUND = tml::no_type>
                struct loop_result;
                
                template<typename WAROUND>
                struct loop_result<tml::true_type , WAROUND>
                {
                    using result = kernel_result;
                };
                
                template<typename WAROUND>
                struct loop_result<tml::false_type , WAROUND>
                {
                    using next_iterator = tml::next<CURRENT,STEP>;
                    using next = _for_loop<next_iterator , kernel_result,tml::equal<next_iterator,END>::value>;
                    using result = typename next::result;
                };
                
              
                using result = typename loop_result<typename KERNEL<typename CURRENT::value , PREVIOUS_RESULT>::abort>::result;
            };

            template<typename CURRENT , typename PREVIOUS_RESULT>
            struct _for_loop<CURRENT,PREVIOUS_RESULT,true>
            {
                using result = PREVIOUS_RESULT;
            };

        public:
            using result = typename _for_loop<BEGIN,START_RESULT,false>::result;
        };
        
        template<typename BEGIN , typename START_RESULT , template<typename,typename> class KERNEL , typename STEP>
        class __for_loop<BEGIN,BEGIN,START_RESULT,KERNEL,STEP>
        {
            using result = START_RESULT;
        };
        
        template<typename BEGIN , typename END , template<typename> class KERNEL , template<typename> class FILTER , typename STEP>
        class __for_each
        {
            template<typename CURRENT , bool FINISHED , typename... KERNELS>
            struct _for_each
            {
                /* 
                 * The idea here is to use lazy instantation of the KERNEL through the filter: If the filter passes (Its evaluated to true with
                 * the current value as parameter) the kernel is instantiated with the current value as argumment (In other words, the computations of this step
                 * of the for loop are performed).
                 * If not, thanks to the lazy instantation provided by wrapping the for_each result, the kernel is not instantiated and the computations are not
                 * performed.
                 */
                
                //The value passes the filter: The kernel is instantiated
                template<bool FILTER_RESULT , typename NON_GLOBAL_SPECIALIZATION_WORKAROUND = tml::no_type>
                struct _result : public _for_each<tml::next<CURRENT,STEP>, tml::equal<tml::next<CURRENT,STEP>,END>::value,KERNELS...,typename KERNEL<typename CURRENT::value>::result> {};
                
                //The value desn't pass the filter: The kernel is not instantiated
                template<typename NON_GLOBAL_SPECIALIZATION_WORKAROUND>
                struct _result<false,NON_GLOBAL_SPECIALIZATION_WORKAROUND> : public _for_each<tml::next<CURRENT,STEP>, tml::equal<tml::next<CURRENT,STEP>,END>::value,KERNELS...> {};
                
                
                using kernel_result = typename KERNEL<typename CURRENT::value>::result;
                using result = typename _result<FILTER<typename CURRENT::value>::value>::result;
            };

            template<typename CURRENT , typename... KERNELS>
            struct _for_each<CURRENT , true , KERNELS...>
            {
                using result = tml::list<KERNELS...>;
            };

        public:
            using result = typename _for_each<BEGIN,false>::result;
        };
        
        
        /* Optimized for_each version: */
        
        template<typename LIST , template<typename> class KERNEL , template<typename> class FILTER , typename STEP>
        struct __for_each_in_list;
        
        //O(n)
        template<typename... Ts , template<typename> class KERNEL , template<typename> class FILTER , typename STEP>
        struct __for_each_in_list<tml::list<Ts...> , KERNEL , FILTER , STEP> : public __for_each<tml::begin<tml::list<Ts...>> , tml::end<tml::list<Ts...>> , KERNEL , FILTER , STEP> {};
        
        //O(1)
        template<typename... Ts , template<typename> class KERNEL , typename STEP>
        struct __for_each_in_list<tml::list<Ts...> , KERNEL , tml::true_predicate , STEP> : public tml::function<tml::list<typename KERNEL<Ts>::result...>> {};
        
        //O(1)
        template<typename... Ts , template<typename> class KERNEL , typename STEP>
        struct __for_each_in_list<tml::list<Ts...> , KERNEL , tml::false_predicate , STEP> : public tml::function<tml::empty_list> {};
        
    }
    
    template<typename CONDITION , typename A , typename B>
    using conditional = typename _conditional<CONDITION::value,A,B>::type;
    
    template<typename BEGIN , typename END , typename INIT_DATA , template<typename,typename> class KERNEL , typename STEP = tml::no_type>
    using for_loop = typename __for_loop<BEGIN,END,INIT_DATA,KERNEL,STEP>::result;
    
    template<typename BEGIN , typename END , template<typename> class KERNEL , template<typename> class FILTER = tml::true_predicate , typename STEP = tml::no_type>
    using for_each = typename __for_each<BEGIN,END,KERNEL,FILTER,STEP>::result;
    
    template<typename LIST , template<typename> class KERNEL , template<typename> class FILTER = tml::true_predicate , typename STEP = tml::no_type>
    using for_each_in_list = typename __for_each_in_list<LIST,KERNEL,FILTER,STEP>::result;
}