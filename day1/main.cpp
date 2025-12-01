#include <algorithm>
#include <functional>
#include <print>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

#include "common.hpp"

struct step
{
	enum class Direction : int32_t
	{
		Left,
		Right
	} direction;
	int distance;

	int operator+( int value ) const { return direction == Direction::Left ? value - distance : value + distance; }
};

struct ClickEvent
{
	int current;
	int previous;
	step s;
};

template <typename T> constexpr T wrap( T x, T min, T max )
{
	T range = max - min;
	return ( ( x - min ) % range + range ) % range + min;
}

template <typename T> constexpr T countNumberOfWraps( T start, T delta )
{
	if( delta == 0 )
		return 0;

	T dir = delta > 0 ? 1 : -1;
	T steps = delta > 0 ? delta : -delta;

	T end = ( start + dir * steps ) % 100;
	if( end < 0 )
		end += 100;

	T distToZero = ( dir > 0 ) ? ( 100 - start ) % 100 : ( start % 100 );

	if( distToZero == 0 )
		distToZero = 100;

	if( steps < distToZero )
		return 0;

	return 1 + ( steps - distToZero ) / 100;
}
std::vector<step> parseFile( std::string_view file )
{
	auto lines = std::views::split( file, '\n' );

	std::vector<step> steps;
	for( const auto &part : lines )
	{
		std::string_view line( part.begin(), part.end() );
		step s;
		if( line[0] == 'L' )
		{
			s.direction = step::Direction::Left;
			auto result = std::from_chars( line.data() + 1, line.data() + line.size(), s.distance );
			if( result.ec != std::errc() )
			{
				throw std::runtime_error( "Invalid input" );
			}
		}
		else if( line[0] == 'R' )
		{
			s.direction = step::Direction::Right;
			auto result = std::from_chars( line.data() + 1, line.data() + line.size(), s.distance );
			if( result.ec != std::errc() )
			{
				throw std::runtime_error( "Invalid input" );
			}
		}
		steps.push_back( s );
	}

	return steps;
}

std::vector<ClickEvent> run( std::vector<step> steps, int start, int max )
{
	int current = start;
	int previous = current;
	std::vector<ClickEvent> click_events;

	for( const auto &step : steps )
	{
		current = step + current;
		current = wrap( current, 0, max );
		click_events.push_back( { current, previous, step } );
		previous = current;
	}

	return click_events;
}

inline constexpr auto count = std::views::transform( []( auto && ) { return 1; } ) | std::views::common;

int main( int argc, char *argv[] )
{
	args::Parser parser( argc, argv );
	auto file1 = reader::read_file<std::string>( parser.get_or( "file", "day1/in.txt" ) );
	auto lines = std::views::split( file1.value(), '\n' );

	const int max = 100;
	int current = 50;
	std::vector<step> steps = parseFile( file1.value() );

	int password1 =
	    std::ranges::distance( run( steps, current, max ) | std::views::filter( []( const ClickEvent &event ) {
		                           return event.current == 0;
	                           } ) );
	std::println( "Part 1: {}", password1 );
	std::println( "" );

	int password2 = std::ranges::fold_left(
	    run( steps, current, max ) | std::views::transform( []( const ClickEvent &event ) {
		    return countNumberOfWraps(
		        event.previous, event.s.direction == step::Direction::Left ? -event.s.distance : event.s.distance );
	    } ),
	    0, std::plus<>{} );
	std::println( "Part 2: {}", password2 );

	return 0;
}
