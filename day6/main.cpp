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
}

struct Problem
{
	std::vector<uint64_t> numbers;
	Operation operation;
};

inline std::string_view trim_line_ending( std::string_view sv )
{
	if( !sv.empty() && sv.back() == '\r' )
		sv.remove_suffix( 1 );
	return sv;
}

inline Operation parse_operator( std::string_view sv )
{
	if( sv == "+" )
		return Operation::ADD;
	else if( sv == "*" )
		return Operation::MULTIPLY;
	return Operation::UNKNOWN;
}

struct ParsedInput
{
	std::vector<std::string_view> lines;
	std::vector<Problem> problems;
};

ParsedInput parse_common( std::string_view file )
{
	ParsedInput result;

	result.lines = std::views::split( file, '\n' ) | std::views::transform( []( auto line ) {
		               return trim_line_ending( std::string_view( line.begin(), line.end() ) );
	               } ) |
	               std::ranges::to<std::vector>();

	auto operators = std::views::split( result.lines.back(), ' ' ) | std::views::transform( []( auto str ) {
		                 return parse_operator( std::string_view( str.begin(), str.end() ) );
	                 } ) |
	                 std::views::filter( []( auto op ) { return op != Operation::UNKNOWN; } );

	std::ranges::for_each( operators, [&]( auto op ) { result.problems.push_back( { {}, op } ); } );

	return result;
}

uint64_t compute_result( const std::vector<Problem> &problems )
{
	return std::ranges::fold_left( problems, 0ull, []( auto acc, const auto &problem ) {
		return acc + std::ranges::fold_left(
		                 problem.numbers | std::views::drop( 1 ), problem.numbers.front(),
		                 [&]( auto acc, auto num ) { return apply_operation( problem.operation, acc, num ); } );
	} );
}

std::vector<Problem> parse_file( std::string_view file )
{
	auto [lines, problems] = parse_common( file );

	for( auto line : lines | std::views::take( lines.size() - 1 ) )
	{
		auto numbers = std::views::split( line, ' ' ) |
		               std::views::filter( []( auto str ) { return !std::string_view( str ).empty(); } );

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
	auto [lines, problems] = parse_common( file );

	size_t counter = 0;
	for( size_t i = 0; i < lines[0].size(); ++i )
	{
		size_t num = 0;
		size_t pow10 = 1;
		for( size_t j = lines.size() - 1; j-- > 0; )
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

	std::println( "Part1 {} ", compute_result( parse_file( file1.value() ) ) );
	std::println( "Part2 {} ", compute_result( parse_file_col( file1.value() ) ) );

	return 0;
}
