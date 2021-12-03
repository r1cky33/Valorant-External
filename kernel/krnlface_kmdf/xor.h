#pragma once

namespace detail
{
	template <typename Type, Type OffsetBasis, Type Prime>
	struct SizeDependantData
	{
		using type = Type;

		constexpr static auto k_offset_basis = OffsetBasis;
		constexpr static auto k_prime = Prime;
	};

	template <std::size_t Bits>
	struct SizeSelector : std::false_type {};

	template <>
	struct SizeSelector<32> : SizeDependantData<std::uint32_t, 0x811c9dc5ul, 16777619ul> {};

	template <>
	struct SizeSelector<64> : SizeDependantData<std::uint64_t, 0xcbf29ce484222325ull, 1099511628211ull> {};

	template <std::size_t Size>
	class FnvHash
	{
	private:
		using data_t = SizeSelector<Size>;

	public:
		using hash = typename data_t::type;

	private:
		constexpr static auto k_offset_basis = data_t::k_offset_basis;
		constexpr static auto k_prime = data_t::k_prime;

	public:
		static __forceinline constexpr auto hash_init(
		) -> hash
		{
			return k_offset_basis;
		}

		static __forceinline constexpr auto hash_byte(
			hash current,
			std::uint8_t byte
		) -> hash
		{
			return (current ^ byte) * k_prime;
		}

		template <std::size_t N>
		static __forceinline constexpr auto hash_constexpr(
			const char(&str)[N],
			const std::size_t size = N - 1
		) -> hash
		{
			const auto prev_hash = size == 1 ? hash_init() : hash_constexpr(str, size - 1);
			const auto cur_hash = hash_byte(prev_hash, str[size - 1]);

			return cur_hash;
		}

		static auto __forceinline hash_runtime_data(
			const void* data,
			const std::size_t sz
		) -> hash
		{
			const auto bytes = static_cast<const uint8_t*>(data);
			const auto end = bytes + sz;

			auto result = hash_init();
			for (auto it = bytes; it < end; ++it)
			{
				result = hash_byte(result, *it);
			}

			return result;
		}

		static auto __forceinline hash_runtime(
			const char* str
		) -> hash
		{
			auto result = hash_init();

			do
			{
				result = hash_byte(result, *str++);
			} 				while (*str != '\0');

			return result;
		}
	};
}

using fnv32 = ::detail::FnvHash<32>;
using fnv64 = ::detail::FnvHash<64>;
using fnv = ::detail::FnvHash<sizeof(void*) * 8>;

#define FNV(str) (std::integral_constant<fnv::hash, fnv::hash_constexpr(str)>::value)
#define FNV32(str) (std::integral_constant<fnv32::hash, fnv32::hash_constexpr(str)>::value)
#define FNV64(str) (std::integral_constant<fnv64::hash, fnv64::hash_constexpr(str)>::value)

#define xor_(str)                                              \
    ::jm::make_xorstr(                                           \
        []() { return str; },                                    \
        std::make_index_sequence<sizeof(str) / sizeof(*str)>{},  \
        std::make_index_sequence<::jm::detail::_buffer_size<sizeof(str)>()>{})
#define E(str) xor_(str).crypt_get()

#ifdef _MSC_VER
#define XORSTR_FORCEINLINE __forceinline
#else
#define XORSTR_FORCEINLINE __attribute__((always_inline))
#endif

#if !defined(XORSTR_ALLOW_DATA)
#if defined(__clang__) || defined(__GNUC__)
#define XORSTR_VOLATILE volatile
#endif

#endif
#ifndef XORSTR_VOLATILE
#define XORSTR_VOLATILE
#endif

namespace jm
{
	namespace detail
	{
		template<std::size_t S>
		struct unsigned_;

		template<>
		struct unsigned_<1>
		{
			using type = std::uint8_t;
		};

		template<>
		struct unsigned_<2>
		{
			using type = std::uint16_t;
		};

		template<>
		struct unsigned_<4>
		{
			using type = std::uint32_t;
		};

		template<auto C, auto...>
		struct pack_value_type
		{
			using type = decltype(C);
		};

		template<std::size_t Size>
		constexpr std::size_t _buffer_size()
		{
			return ((Size / 16) + (Size % 16 != 0)) * 2;
		}

		template<auto... Cs>
		struct tstring_
		{
			using value_type = typename pack_value_type<Cs...>::type;
			constexpr static std::size_t size = sizeof...(Cs);
			constexpr static value_type  str[size] = { Cs... };

			constexpr static std::size_t buffer_size = _buffer_size<sizeof(str)>();
			constexpr static std::size_t buffer_align = 16;
		};

		template<std::size_t I, std::uint64_t K>
		struct _ki
		{
			constexpr static std::size_t   idx = I;
			constexpr static std::uint64_t key = K;
		};

		template<std::uint32_t Seed>
		constexpr std::uint32_t key4()
		{
			std::uint32_t value = Seed;
			value = static_cast<std::uint32_t>((value ^ 1337) * 16777619ull);

			return value;
		}

		template<std::size_t S>
		constexpr std::uint64_t key8()
		{
			constexpr auto first_part = key4<2166136261 + S>();
			constexpr auto second_part = key4<first_part>();
			return (static_cast<std::uint64_t>(first_part) << 32) | second_part;
		}

		template<class T, std::uint64_t... Keys>
		struct string_storage {
			std::uint64_t storage[T::buffer_size];

			XORSTR_FORCEINLINE constexpr string_storage() noexcept : storage{ Keys... }
			{
				using cast_type =
					typename unsigned_<sizeof(typename T::value_type)>::type;
				constexpr auto value_size = sizeof(typename T::value_type);

				for (std::size_t i = 0; i < T::size; ++i)
					storage[i / (8 / value_size)] ^=
					(std::uint64_t{ static_cast<cast_type>(T::str[i]) }
				<< ((i % (8 / value_size)) * 8 * value_size));
			}
		};

	}

	template<class T, class... Keys>
	class xor_string
	{
		alignas(T::buffer_align) std::uint64_t _storage[T::buffer_size];

		XORSTR_FORCEINLINE void _crypt_256_single(const std::uint64_t* keys,
			std::uint64_t* storage)

		{
			_mm256_store_si256(
				reinterpret_cast<__m256i*>(storage),
				_mm256_xor_si256(
					_mm256_load_si256(reinterpret_cast<const __m256i*>(storage)),
					_mm256_load_si256(reinterpret_cast<const __m256i*>(keys))));
		}

		template<std::size_t... Idxs>
		XORSTR_FORCEINLINE void _crypt_256(const std::uint64_t* keys,
			std::index_sequence<Idxs...>)
		{
			(_crypt_256_single(keys + Idxs * 4, _storage + Idxs * 4), ...);
		}

		XORSTR_FORCEINLINE void _crypt_128_single(const std::uint64_t* keys,
			std::uint64_t* storage)
		{
			_mm_store_si128(
				reinterpret_cast<__m128i*>(storage),
				_mm_xor_si128(_mm_load_si128(reinterpret_cast<const __m128i*>(storage)),
					_mm_load_si128(reinterpret_cast<const __m128i*>(keys))));
		}

		template<std::size_t... Idxs>
		XORSTR_FORCEINLINE void _crypt_128(const std::uint64_t* keys,
			std::index_sequence<Idxs...>)
		{
			(_crypt_128_single(keys + Idxs * 2, _storage + Idxs * 2), ...);
		}

		XORSTR_FORCEINLINE constexpr void _copy()
		{
			constexpr detail::string_storage<T, Keys::key...> storage;
			static_cast<void>(std::initializer_list<std::uint64_t>{
				(const_cast<XORSTR_VOLATILE std::uint64_t*>(_storage))[Keys::idx] =
					storage.storage[Keys::idx]... });
		}

	public:
		using value_type = typename T::value_type;
		using size_type = std::size_t;
		using pointer = value_type*;
		using const_pointer = const pointer;

		XORSTR_FORCEINLINE xor_string() { _copy(); }

		XORSTR_FORCEINLINE constexpr size_type size() const
		{
			return T::size - 1;
		}

		XORSTR_FORCEINLINE void crypt()
		{
			alignas(T::buffer_align) std::uint64_t keys[T::buffer_size];
			static_cast<void>(std::initializer_list<std::uint64_t>{
				(const_cast<XORSTR_VOLATILE std::uint64_t*>(keys))[Keys::idx] =
					Keys::key... });

			_copy();

			_crypt_128(keys, std::make_index_sequence<T::buffer_size / 2>{});
		}

		XORSTR_FORCEINLINE const_pointer get() const
		{
			return reinterpret_cast<const_pointer>(_storage);
		}

		XORSTR_FORCEINLINE const_pointer crypt_get()
		{
			crypt();
			return reinterpret_cast<const_pointer>(_storage);
		}
	};

	template<class Tstr, std::size_t... StringIndices, std::size_t... KeyIndices>
	XORSTR_FORCEINLINE constexpr auto
		make_xorstr(Tstr str_lambda,
			std::index_sequence<StringIndices...>,
			std::index_sequence<KeyIndices...>)
	{
		return xor_string<detail::tstring_<str_lambda()[StringIndices]...>,
			detail::_ki<KeyIndices, detail::key8<KeyIndices>()>...>{};
	}
}