#include<string>
#include<fast_io.h>
#include<fast_io_device.h>
#include<fast_io_driver/timer.h>
#include<vector>

int main()
{
	constexpr std::size_t N(10000000);
	{
		fast_io::timer t(u8"output");
		fast_io::u8obuf_file obf(u8"line_get.txt");
		for(std::size_t i{};i!=N;++i)
			println(obf,i);
	}
	std::size_t lines{};
	{
		fast_io::timer t(u8"input");
		fast_io::u8ibuf_file ibf(u8"line_get.txt");
		for(std::u8string str;scan<true>(ibf,fast_io::mnp::line_get(str));)
		{
			++lines;
		}
	}
	println(lines);
}
