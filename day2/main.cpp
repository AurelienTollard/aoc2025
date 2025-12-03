#include <algorithm>
#include <cmath>
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

size_t dec_size( const uint64_t id )
{
	if( id == 0 )
		return 1;
	return static_cast<int>( std::log10( id ) ) + 1;
}

bool is_valid_id( const uint64_t id )
{
	size_t size = dec_size( id );
	uint64_t divisor = static_cast<uint64_t>( std::pow( 10, size / 2 ) );
	uint64_t first = id % divisor;
	uint64_t second = id / divisor;
	return first != second;
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

bool is_valid_id_complex( uint64_t id )
{
	size_t size = dec_size( id );
	auto divs = divisors( size );
	divs.pop_back();
	for( uint64_t i : divs )
	{
		uint64_t divisor = static_cast<uint64_t>( std::pow( 10, i ) );
		std::vector<uint64_t> parts( size / i );
		uint64_t local_id = id;

		for( uint64_t j = 0; j < size / i; ++j )
		{
			parts[j] = local_id % divisor;
			local_id /= divisor;
		}
		bool all_equal = std::ranges::all_of(
		    std::next( parts.begin() ), parts.end(), [&]( const uint64_t &part ) { return parts.front() == part; } );
		if( all_equal )
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
			if( !is_valid_id( id ) )
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
			if( !is_valid_id_complex( id ) )
			{
				output += id;
			}
		}
	}
	std::println( "Part 2: {}", output );

	return 0;
}
