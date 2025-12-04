#include <algorithm>
#include <cmath>
#include <cstdint>
#include <print>
#include <ranges>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "common.hpp"

std::vector<std::vector<uint64_t>> parseFile( std::string_view file )
{
	auto lines = std::views::split( file, '\n' );

	std::vector<std::vector<uint64_t>> packs;
	for( const auto &line : lines )
	{
		std::vector<uint64_t> pack;
		for( auto it = line.begin(); it != line.end(); ++it )
		{
			char c = *it;
			if( c > '9' || c < '0' )
				continue;
			pack.push_back( c - '0' );
		}
		packs.push_back( pack );
	}

	return packs;
}

template <size_t N>
std::array<std::pair<uint64_t, uint64_t>, N> findMaxIndexesGeneric( const std::vector<uint64_t> &packs )
{
	std::array<std::pair<uint64_t, uint64_t>, N> maxIndexes{};

	auto maxIt = std::ranges::max_element( packs.begin(), packs.end() - N + 1 );
	maxIndexes[0].first = std::distance( packs.begin(), maxIt );
	maxIndexes[0].second = *maxIt;
	for( size_t i = 1; i < N; ++i )
	{
		auto maxIt = std::ranges::max_element( packs.begin() + maxIndexes[i - 1].first + 1, packs.end() - N + i + 1 );
		maxIndexes[i].first = std::distance( packs.begin(), maxIt );
		maxIndexes[i].second = *maxIt;
	}
	return maxIndexes;
}

template <size_t N> constexpr std::array<uint64_t, N> makePowersOf10()
{
	std::array<uint64_t, N> result{};
	uint64_t power = 1;
	for( size_t i = 0; i < N; ++i )
	{
		result[N - 1 - i] = power;
		power *= 10;
	}
	return result;
}

template <size_t N> uint64_t sumIndexes( const std::array<std::pair<uint64_t, uint64_t>, N> &maxIndexes )
{
	constexpr auto powers = makePowersOf10<N>();
	uint64_t sum = 0;
	for( size_t i = 0; i < N; ++i )
	{
		sum += maxIndexes[i].second * powers[i];
	}
	return sum;
}

int main( int argc, char *argv[] )
{
	args::Parser parser( argc, argv );
	auto file1 = reader::read_file<std::string>( parser.get_or( "file", "day3/in.txt" ) );
	std::vector<std::vector<uint64_t>> packs = parseFile( file1.value() );

	auto start1 = std::chrono::high_resolution_clock::now();
	uint64_t part1 = std::ranges::fold_left(
	    packs | std::views::transform( []( const auto &pack ) {
		    auto indexes = findMaxIndexesGeneric<2>( pack );
		    uint64_t sum = sumIndexes( indexes );
		    return sum;
	    } ),
	    0, std::plus<>{} );
	auto end1 = std::chrono::high_resolution_clock::now();
	auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>( end1 - start1 );
	std::println( "Part 1: {} (took {} µs)", part1, duration1.count() );

	auto start2 = std::chrono::high_resolution_clock::now();
	uint64_t part2 = std::ranges::fold_left(
	    packs | std::views::transform( []( const auto &pack ) {
		    auto indexes = findMaxIndexesGeneric<12>( pack );
		    uint64_t sum = sumIndexes( indexes );
		    return sum;
	    } ),
	    0, std::plus<>{} );
	auto end2 = std::chrono::high_resolution_clock::now();
	auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>( end2 - start2 );
	std::println( "Part 2: {} (took {} µs)", part2, duration2.count() );

	return 0;
}
