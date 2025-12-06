#include <algorithm>
#include <cstdint>
#include <iterator>
#include <print>
#include <ranges>
#include <string>
#include <string_view>
#include <utility>

#include "common.hpp"

struct Database
{
	std::vector<std::pair<uint64_t, uint64_t>> ranges;
	std::vector<uint64_t> ingredients;
};

Database parse_file( std::string_view file )
{
	Database db;
	bool ranges = true;
	auto lines = std::views::split( file, '\n' );

	for( auto line : lines )
	{
		if( *line.begin() == '\r' )
		{
			ranges = false;
			continue;
		}

		if( ranges )
		{
			auto numbers = std::views::split( line, '-' );
			uint64_t start = std::stoull( std::string( ( *numbers.begin() ).begin(), ( *numbers.begin() ).end() ) );
			uint64_t end = std::stoull(
			    std::string( ( *std::next( numbers.begin() ) ).begin(), ( *std::next( numbers.begin() ) ).end() ) );
			db.ranges.push_back( { start, end } );
		}
		else
		{
			db.ingredients.push_back( std::stoull( std::string( line.begin(), line.end() ) ) );
		}
	}

	return db;
}

bool is_fresh( std::vector<std::pair<uint64_t, uint64_t>> &ranges, uint64_t ingredient )
{
	return std::ranges::any_of(
	    ranges, [ingredient]( const auto &range ) { return ingredient >= range.first && ingredient <= range.second; } );
}

int main( int argc, char *argv[] )
{
	args::Parser parser( argc, argv );
	auto file1 = reader::read_file<std::string>( parser.get_or( "file", "day5/in.txt" ) );
	Database db = parse_file( file1.value() );

	uint64_t part1 = std::ranges::count_if( db.ingredients, [&]( uint64_t i ) { return is_fresh( db.ranges, i ); } );
	std::println( "Par1: {}", part1 );

	std::ranges::sort( db.ranges, {}, &std::pair<uint64_t, uint64_t>::first );
	std::vector<std::pair<uint64_t, uint64_t>> merged = { db.ranges.front() };
	for( size_t i = 1; i < db.ranges.size(); ++i )
	{
		if( db.ranges[i].first <= merged.back().second + 1 )
		{
			merged.back().second = std::max( merged.back().second, db.ranges[i].second );
		}
		else
		{
			merged.push_back( db.ranges[i] );
		}
	}
	uint64_t part2 = std::ranges::fold_left(
	    merged, 0ull, []( uint64_t acc, const auto &range ) { return acc + range.second - range.first + 1; } );
	std::println( "Par2: {}", part2 );

	return 0;
}
