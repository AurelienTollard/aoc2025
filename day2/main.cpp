#include <algorithm>
#include <cstdint>
#include <print>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

#include "common.hpp"

struct Ranges
{
	uint64_t first_id;
	uint64_t last_id;
};

std::vector<Ranges> parse_input( std::string_view file )
{
	std::vector<Ranges> ranges;
	auto parts = std::views::split( file, ',' );
	for( auto part : parts )
	{
		auto subparts = std::views::split( part, '-' );
		auto it = std::ranges::begin( subparts );
		auto first = std::stoull( std::string( std::ranges::begin( *it ), std::ranges::end( *it ) ) );
		++it;
		auto second = std::stoull( std::string( std::ranges::begin( *it ), std::ranges::end( *it ) ) );
		ranges.push_back( { first, second } );
	}

	return ranges;
}

bool is_valid_id( const std::string_view id )
{
	if( id.empty() || id.length() % 2 != 0 )
	{
		return true;
	}

	std::string_view view{ id };
	std::size_t half = id.length() / 2;
	std::string_view first = view.substr( 0, half );
	std::string_view second = view.substr( half );

	return first != second;
}

std::vector<std::string_view> split_size( std::string_view input, size_t size )
{
	std::vector<std::string_view> result;
	result.reserve( input.length() / size );

	const std::size_t total = input.size();
	for( std::size_t i = 0; i < total; i += size )
	{
		result.emplace_back( input.substr( i, size ) );
	}

	return result;
}

std::vector<uint64_t> divisors( uint64_t n )
{
	std::vector<uint64_t> result;
	uint64_t sqrt_n = static_cast<uint64_t>( std::sqrt( n ) );

	for( uint64_t i = 1; i <= sqrt_n; ++i )
	{
		if( n % i == 0 )
		{
			result.push_back( i );
			if( i != n / i )
				result.push_back( n / i );
		}
	}
	std::ranges::sort( result );
	return result;
}

bool is_valid_id_complex( std::string_view id )
{
	auto divs = divisors( id.length() );
	divs.pop_back();
	for( uint64_t i : divs )
	{
		std::vector<std::string_view> parts = split_size( id, i );
		std::string_view first = parts.front();
		bool all_equal = std::ranges::any_of(
		    std::next( parts.begin() ), parts.end(), [&]( const std::string_view &sub ) { return first != sub; } );
		if( !all_equal )
		{
			return false;
		}
	}
	return true;
}

int main( int argc, char *argv[] )
{
	args::Parser parser( argc, argv );
	auto file1 = reader::read_file<std::string>( parser.get_or( "file", "day2/in.txt" ) );
	std::vector<Ranges> ranges = parse_input( file1.value() );
	uint64_t output = 0;

	for( auto &range : ranges )
	{
		for( uint64_t id = range.first_id; id <= range.last_id; ++id )
		{
			if( !is_valid_id( std::to_string( id ) ) )
			{
				output += id;
			}
		}
	}
	std::print( "Part 1: {}\n", output );

	output = 0;
	for( auto &range : ranges )
	{
		for( uint64_t id = range.first_id; id <= range.last_id; ++id )
		{
			if( !is_valid_id_complex( std::to_string( id ) ) )
			{
				output += id;
			}
		}
	}
	std::println( "Part 2: {}", output );

	return 0;
}
