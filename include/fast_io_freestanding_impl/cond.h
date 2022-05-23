#pragma once

namespace fast_io
{

namespace manipulators
{

template<typename T1,typename T2>
struct condition
{
	using manip_tag = manip_tag_t;
	using alias_type1 = T1;
	using alias_type2 = T2;
	alias_type1 t1;
	alias_type2 t2;
	bool pred;
};

template<typename T1,typename T2>
inline constexpr decltype(auto) cond(bool pred,T1&& t1,T2&& t2) noexcept
{
	using t1aliastype=decltype(fast_io::io_print_alias(t1));
	using t2aliastype=decltype(fast_io::io_print_alias(t2));
	constexpr bool type_match{::std::same_as<t1aliastype,t2aliastype>};
	if constexpr(type_match)
	{
		if(pred)
		{
			return fast_io::io_print_alias(t1);
		}
		else
		{
			return fast_io::io_print_alias(t2);
		}
	}
	else
	{
		if constexpr(sizeof(t1aliastype)<sizeof(t2aliastype))
		{
			return condition<t2aliastype,t1aliastype>{::fast_io::io_print_alias(t2),::fast_io::io_print_alias(t1),!pred};
		}
		else
		{
			return condition<t1aliastype,t2aliastype>{::fast_io::io_print_alias(t1),::fast_io::io_print_alias(t2),pred};
		}
	}
}

}

template<std::integral char_type,typename T1,typename T2>
requires (reserve_printable<char_type,T1>&&reserve_printable<char_type,T2>)
inline constexpr ::std::size_t print_reserve_size(io_reserve_type_t<char_type,::fast_io::manipulators::condition<T1,T2>>) noexcept
{
	constexpr std::size_t s1{print_reserve_size(io_reserve_type<char_type,T1>)};
	constexpr std::size_t s2{print_reserve_size(io_reserve_type<char_type,T2>)};
	if constexpr(s1<s2)
	{
		return s2;
	}
	else
	{
		return s1;
	}
}

template<std::integral char_type,typename T1,typename T2>
requires (reserve_printable<char_type,T1>&&reserve_printable<char_type,T2>)
inline constexpr char_type* print_reserve_define(io_reserve_type_t<char_type,::fast_io::manipulators::condition<T1,T2>>,char_type *iter,::fast_io::manipulators::condition<T1,T2> c) noexcept
{
	if(c.pred)
	{
		return print_reserve_define(io_reserve_type<char_type,T1>,iter,c.t1);
	}
	else
	{
		return print_reserve_define(io_reserve_type<char_type,T2>,iter,c.t2);
	}
}

template<std::integral char_type,typename T1,typename T2>
requires (scatter_printable<char_type,T1>&&scatter_printable<char_type,T2>)
inline constexpr basic_io_scatter_t<char_type> print_scatter_define(io_reserve_type_t<char_type,::fast_io::manipulators::condition<T1,T2>>,::fast_io::manipulators::condition<T1,T2> c)
{
	if(c.pred)
	{
		return {print_scatter_define(io_reserve_type<char_type,T1>,c.t1)};
	}
	else
	{
		return {print_scatter_define(io_reserve_type<char_type,T2>,c.t2)};
	}
}

template<std::integral char_type,typename T1,typename T2>
requires ((reserve_printable<char_type,T1>||dynamic_reserve_printable<char_type,T1>||scatter_printable<char_type,T1>)&&
	(reserve_printable<char_type,T2>||dynamic_reserve_printable<char_type,T2>||scatter_printable<char_type,T2>))
inline constexpr ::std::size_t print_reserve_size(io_reserve_type_t<char_type,::fast_io::manipulators::condition<T1,T2>>,::fast_io::manipulators::condition<T1,T2> c)
{
	if(c.pred)
	{
		if constexpr(scatter_printable<char_type,T1>)
		{
			return print_scatter_define(io_reserve_type<char_type,T1>,c.t1).len;
		}
		else if constexpr(reserve_printable<char_type,T1>)
		{
			constexpr std::size_t sz{print_reserve_size(io_reserve_type<char_type,T1>)};
			return sz;
		}
		else
		{
			return print_reserve_size(io_reserve_type<char_type,T1>,c.t1);	
		}
	}
	else
	{
		if constexpr(scatter_printable<char_type,T2>)
		{
			return print_scatter_define(io_reserve_type<char_type,T1>,c.t2).len;
		}
		else if constexpr(reserve_printable<char_type,T2>)
		{
			constexpr std::size_t sz{print_reserve_size(io_reserve_type<char_type,T2>)};
			return sz;
		}
		else
		{
			return print_reserve_size(io_reserve_type<char_type,T1>,c.t2);	
		}
	}
}

template<::std::integral char_type,typename T1,typename T2>
requires ((reserve_printable<char_type,T1>||dynamic_reserve_printable<char_type,T1>)&&
	(reserve_printable<char_type,T2>||dynamic_reserve_printable<char_type,T2>))
inline constexpr char_type* print_reserve_define(io_reserve_type_t<char_type,::fast_io::manipulators::condition<T1,T2>>,char_type* iter,::fast_io::manipulators::condition<T1,T2> c)
{
	if(c.pred)
	{
		return print_reserve_define(io_reserve_type<char_type,T1>,iter,c.t1);	
	}
	else
	{
		return print_reserve_define(io_reserve_type<char_type,T2>,iter,c.t2);
	}
}

template<::std::integral char_type,typename T1,typename T2,::fast_io::buffer_output_stream bop>
requires ((reserve_printable<char_type,T1>||dynamic_reserve_printable<char_type,T1>||scatter_printable<char_type,T1>||printable<char_type,T1>)&&
	(reserve_printable<char_type,T2>||dynamic_reserve_printable<char_type,T2>||scatter_printable<char_type,T2>||printable<char_type,T2>))
inline constexpr void print_define(io_reserve_type_t<char_type,::fast_io::manipulators::condition<T1,T2>>,bop b,::fast_io::manipulators::condition<T1,T2> c)
{
	if(c.pred)
	{
		print_freestanding(b,c.t1);
	}
	else
	{
		print_freestanding(b,c.t2);
	}
}

}

