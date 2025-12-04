#include <algorithm>
#include <cstdint>
#include <iterator>
#include <print>
#include <ranges>
#include <set>
#include <string>
#include <string_view>
#include <utility>

#include "common.hpp"

struct Map
{
	std::set<std::pair<int, int>> rolls;
	std::set<std::pair<int, int>> positions;
};

std::pair<int, int> operator+( const std::pair<int, int> &lhs, const std::pair<int, int> &rhs )
{
	return { lhs.first + rhs.first, lhs.second + rhs.second };
}

Map parseFile( std::string_view file )
{
	auto lines = std::views::split( file, '\n' );

	Map map;

	int y = 0;
	for( const auto &line : lines )
	{
		int x = 0;
		for( char c : line )
		{
			if( c == '.' )
			{
				map.positions.insert( { x, y } );
			}
			else if( c == '@' )
			{
				map.rolls.insert( { x, y } );
			}
			++x;
		}
		++y;
	}

	return map;
}

inline constexpr std::array<std::pair<int, int>, 8> directions = {
	{ { 0, 1 }, { 1, 0 }, { 0, -1 }, { -1, 0 }, { 1, 1 }, { 1, -1 }, { -1, 1 }, { -1, -1 } }
};

bool isValidPosition( std::set<std::pair<int, int>> &rolls, std::pair<int, int> pos )
{
	auto count = std::ranges::count_if( directions, [&]( const auto &dir ) {
		auto newPos = pos + dir;
		return rolls.contains( newPos );
	} );
	return count < 4;
}

int main( int argc, char *argv[] )
{
	args::Parser parser( argc, argv );
	auto file1 = reader::read_file<std::string>( parser.get_or( "file", "day4/in.txt" ) );
	auto map = parseFile( file1.value() );

	uint64_t part1 = std::ranges::distance(
	    map.rolls | std::views::filter( [&]( const auto &pos ) { return isValidPosition( map.rolls, pos ); } ) );
	std::println( "Part 1: {}", part1 );

	auto part2 = 0;
	auto rolls = map.rolls;
	while( true )
	{
		auto newRolls = rolls | std::views::filter( [&]( const auto &pos ) { return !isValidPosition( rolls, pos ); } );
		auto count = rolls.size() - std::ranges::distance( newRolls );
		if( count == 0 )
			break;
		rolls = newRolls | std::ranges::to<std::set>();
		part2 += count;
	}
	std::println( "Part 2: {}", part2 );

	return 0;
}
