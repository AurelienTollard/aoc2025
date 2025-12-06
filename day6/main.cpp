#include <algorithm>
#include <cstdint>
#include <print>
#include <ranges>
#include <string>
#include <string_view>

#include "common.hpp"

enum class Operation : uint32_t
{
	UNKNOWN,
	ADD,
	MULTIPLY
};

inline constexpr uint64_t apply_operation( Operation op, uint64_t a, uint64_t b )
{
	switch( op )
	{
	case Operation::ADD:
		return a + b;
	case Operation::MULTIPLY:
		return a * b;
	default:
		return 0;
	}
	return 0;
}

struct Problem
{
	std::vector<uint64_t> numbers;
	Operation operation;
};

std::vector<Problem> parse_file( std::string_view file )
{
	std::vector<Problem> problems;
	bool first = true;
	auto lines = std::views::split( file, '\n' ) |
	             std::views::transform( []( auto line ) { return std::string_view( line.begin(), line.end() - 1 ); } ) |
	             std::ranges::to<std::vector>();
	auto operators = std::views::split( lines.back(), ' ' ) | std::views::transform( []( auto str ) {
		                 auto sv = std::string_view( str.begin(), str.end() );
		                 if( sv == "+" )
			                 return Operation::ADD;
		                 else if( sv == "*" )
			                 return Operation::MULTIPLY;
		                 return Operation::UNKNOWN;
	                 } ) |
	                 std::views::filter( []( auto op ) { return op != Operation::UNKNOWN; } );
	std::ranges::for_each( operators, [&]( auto op ) { problems.push_back( { {}, op } ); } );

	for( auto line : lines | std::views::take( lines.size() - 1 ) )
	{
		auto numbers = std::views::split( line, ' ' ) | std::views::filter( []( auto str ) {
			               auto sv = std::string_view( str.begin(), str.end() );
			               return !sv.empty() && sv != "\r";
		               } );

		size_t counter = 0;
		for( auto number : numbers )
		{
			problems[counter++].numbers.push_back( std::stoull( std::string( number.begin(), number.end() ) ) );
		}
	}

	return problems;
}

std::vector<Problem> parse_file_col( std::string_view file )
{
	std::vector<Problem> problems;
	bool first = true;
	auto lines = std::views::split( file, '\n' ) |
	             std::views::transform( []( auto line ) { return std::string_view( line.begin(), line.end() - 1 ); } ) |
	             std::ranges::to<std::vector>();
	auto operators = std::views::split( lines.back(), ' ' ) | std::views::transform( []( auto str ) {
		                 auto sv = std::string_view( str.begin(), str.end() );
		                 if( sv == "+" )
			                 return Operation::ADD;
		                 else if( sv == "*" )
			                 return Operation::MULTIPLY;
		                 return Operation::UNKNOWN;
	                 } ) |
	                 std::views::filter( []( auto op ) { return op != Operation::UNKNOWN; } );
	std::ranges::for_each( operators, [&]( auto op ) { problems.push_back( { {}, op } ); } );

	size_t counter = 0;
	for( int i = 0; i < lines[0].size(); ++i )
	{
		size_t num = 0;
		size_t pow10 = 1;
		for( int j = lines.size() - 2; j >= 0; --j )
		{
			char c = lines[j][i];
			if( c == ' ' )
			{
				continue;
			}
			else if( c >= '0' && c <= '9' )
			{
				num += ( c - '0' ) * pow10;
				pow10 *= 10;
			}
		}
		if( num > 0 )
		{
			problems[counter].numbers.push_back( num );
		}
		else
		{
			counter++;
		}
	}

	return problems;
}

int main( int argc, char *argv[] )
{
	args::Parser parser( argc, argv );
	auto file1 = reader::read_file<std::string>( parser.get_or( "file", "day6/in.txt" ) );
	std::vector<Problem> problems = parse_file( file1.value() );

	uint64_t part1 = std::ranges::fold_left( problems, 0ull, []( auto acc, auto problem ) {
		return acc + std::ranges::fold_left(
		                 problem.numbers | std::views::drop( 1 ), problem.numbers.front(),
		                 [&]( auto acc, auto num ) { return apply_operation( problem.operation, acc, num ); } );
	} );
	std::println( "Part1 {} ", part1 );

	std::vector<Problem> problems2 = parse_file_col( file1.value() );
	uint64_t part2 = std::ranges::fold_left( problems2, 0ull, []( auto acc, auto problem ) {
		return acc + std::ranges::fold_left(
		                 problem.numbers | std::views::drop( 1 ), problem.numbers.front(),
		                 [&]( auto acc, auto num ) { return apply_operation( problem.operation, acc, num ); } );
	} );
	std::println( "Part2 {} ", part2 );

	return 0;
}
