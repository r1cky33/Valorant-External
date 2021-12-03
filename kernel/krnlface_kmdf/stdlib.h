#pragma once

namespace std
{
	typedef unsigned __int64   size_t;
	typedef unsigned __int64  uintptr_t;

	typedef signed char        int8_t;
	typedef short              int16_t;
	typedef int                int32_t;
	typedef long long          int64_t;
	typedef unsigned char      uint8_t;
	typedef unsigned short     uint16_t;
	typedef unsigned int       uint32_t;
	typedef unsigned long long uint64_t;

	typedef signed char        int_least8_t;
	typedef short              int_least16_t;
	typedef int                int_least32_t;
	typedef long long          int_least64_t;
	typedef unsigned char      uint_least8_t;
	typedef unsigned short     uint_least16_t;
	typedef unsigned int       uint_least32_t;
	typedef unsigned long long uint_least64_t;

	typedef signed char        int_fast8_t;
	typedef int                int_fast16_t;
	typedef int                int_fast32_t;
	typedef long long          int_fast64_t;
	typedef unsigned char      uint_fast8_t;
	typedef unsigned int       uint_fast16_t;
	typedef unsigned int       uint_fast32_t;
	typedef unsigned long long uint_fast64_t;

	typedef long long          intmax_t;
	typedef unsigned long long uintmax_t;

	template<class T, T v>
	struct integral_constant
	{
		static constexpr T value = v;
		typedef T value_type;
		typedef integral_constant type;
		constexpr operator value_type() const { return value; }
		constexpr value_type operator()() const { return value; }
	};

	template <class _Elem>
	class initializer_list
	{
	public:
		using value_type = _Elem;
		using reference = const _Elem&;
		using const_reference = const _Elem&;
		using size_type = size_t;

		using iterator = const _Elem*;
		using const_iterator = const _Elem*;

		constexpr initializer_list() : _First(nullptr), _Last(nullptr)
		{ }

		constexpr initializer_list(const _Elem* _First_arg, const _Elem* _Last_arg) : _First(_First_arg), _Last(_Last_arg)
		{ }

		constexpr const _Elem* begin() const
		{
			return _First;
		}

		constexpr const _Elem* end() const
		{
			return _Last;
		}

		constexpr size_t size() const
		{
			return static_cast<size_t>(_Last - _First);
		}

	private:
		const _Elem* _First;
		const _Elem* _Last;
	};

	template <class _Ty>
	struct remove_reference {
		using type = _Ty;
		using _Const_thru_ref_type = const _Ty;
	};

	template <class _Ty>
	struct remove_reference<_Ty&> {
		using type = _Ty;
		using _Const_thru_ref_type = const _Ty&;
	};

	template <class _Ty>
	struct remove_reference<_Ty&&> {
		using type = _Ty;
		using _Const_thru_ref_type = const _Ty&&;
	};

	template <class _Ty>
	using remove_reference_t = typename remove_reference<_Ty>::type;

	template <class _Ty>
	constexpr _Ty&& forward(
		remove_reference_t<_Ty>& _Arg) noexcept { // forward an lvalue as either an lvalue or an rvalue
		return static_cast<_Ty&&>(_Arg);
	}

	template <class>
	constexpr bool is_lvalue_reference_v = false;

	template <class _Elem>
	constexpr const _Elem* begin(initializer_list<_Elem> _Ilist)
	{
		return _Ilist.begin();
	}

	template <class _Elem>
	constexpr const _Elem* end(initializer_list<_Elem> _Ilist)
	{
		return _Ilist.end();
	}

	template <size_t... Ints>
	struct index_sequence
	{
		using type = index_sequence;
		using value_type = size_t;
		static constexpr std::size_t size() { return sizeof...(Ints); }
	};

	template <class Sequence1, class Sequence2>
	struct _merge_and_renumber;

	template <size_t... I1, size_t... I2>
	struct _merge_and_renumber<index_sequence<I1...>, index_sequence<I2...>>
		: index_sequence<I1..., (sizeof...(I1) + I2)...>
	{ };

	template <size_t N>
	struct make_index_sequence
		: _merge_and_renumber<typename make_index_sequence<N / 2>::type,
		typename make_index_sequence<N - N / 2>::type>
	{ };

	template<typename T>
	T get_remainder(T val, T min, T max)
	{
		while (val > max)
		{
			val -= max * 2.f;
		}

		while (val < min)
		{
			val += max * 2.f;
		}

		return val;
	}

	template<> struct make_index_sequence<0> : index_sequence<> { };
	template<> struct make_index_sequence<1> : index_sequence<0> { };
}

using namespace std;