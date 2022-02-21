#pragma once
#ifndef SCL_UTILITY_SIMILAR_METHOD_H
#define SCL_UTILITY_SIMILAR_METHOD_H

namespace ScL { namespace Detail
{
    template < typename _Type, typename _Method >
    struct MethodSignature;
    template < typename _Type, typename _Result, typename ... _Arguments >
    struct MethodSignature< _Type, _Result( _Arguments ... ) > { using Type = _Result(::std::decay_t< _Type >::*)( _Arguments ... ); };
    template < typename _Type, typename _Result, typename ... _Arguments >
    struct MethodSignature< _Type const, _Result( _Arguments ... ) > { using Type = _Result(::std::decay_t< _Type >::*)( _Arguments ... ) const; };
    template < typename _Type, typename _Result, typename ... _Arguments >
    struct MethodSignature< _Type const &, _Result( _Arguments ... ) > { using Type = _Result(::std::decay_t< _Type >::*)( _Arguments ... ) const &; };
    template < typename _Type, typename _Result, typename ... _Arguments >
    struct MethodSignature< _Type const &&, _Result( _Arguments ... ) > { using Type = _Result(::std::decay_t< _Type >::*)( _Arguments ... ) const &&; };
    template < typename _Type, typename _Result, typename ... _Arguments >
    struct MethodSignature< _Type volatile, _Result( _Arguments ... ) > { using Type = _Result(::std::decay_t< _Type >::*)( _Arguments ... ) volatile; };
    template < typename _Type, typename _Result, typename ... _Arguments >
    struct MethodSignature< _Type volatile &, _Result( _Arguments ... ) > { using Type = _Result(::std::decay_t< _Type >::*)( _Arguments ... ) volatile &; };
    template < typename _Type, typename _Result, typename ... _Arguments >
    struct MethodSignature< _Type volatile &&, _Result( _Arguments ... ) > { using Type = _Result(::std::decay_t< _Type >::*)( _Arguments ... ) volatile &&; };
    template < typename _Type, typename _Result, typename ... _Arguments >
    struct MethodSignature< _Type const volatile, _Result( _Arguments ... ) > { using Type = _Result(::std::decay_t< _Type >::*)( _Arguments ... ) const volatile; };
    template < typename _Type, typename _Result, typename ... _Arguments >
    struct MethodSignature< _Type const volatile &, _Result( _Arguments ... ) > { using Type = _Result(::std::decay_t< _Type >::*)( _Arguments ... ) const volatile &; };
    template < typename _Type, typename _Result, typename ... _Arguments >
    struct MethodSignature< _Type const volatile &&, _Result( _Arguments ... ) > { using Type = _Result(::std::decay_t< _Type >::*)( _Arguments ... ) const volatile &&; };
}}

namespace ScL
{
    template < typename _Type, typename _Signature >
    using SimilarMethod = typename ::ScL::Detail::MethodSignature< _Type, _Signature >::Type;
    /*!
     * Тип метода члена класса _Type с сигнатуры _Result( _Arguments ...) * с
     * точным соответствием квалификаторов const/volatile и специализации rvalue/lvalue,
     * заданных для типа _Type.
     *
     * Примеры:
     *
     *  using Method = SimilarMethod< Type, void(int) >;            // void (Type::*)(int)
     *  using Method = SimilarMethod< Type &, void(int) >;          // void (Type::*)(int) &
     *  using Method = SimilarMethod< Type const, void(int) >;      // void (Type::*)(int) const
     *  using Method = SimilarMethod< Type const &&, void(int) >;   // void (Type::*)(int) const &&
     */
}

#endif
