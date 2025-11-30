#include <expected>
#include <print>

#include "common.hpp"

int main( int argc, char *argv[] )
{
	args::Parser parser( argc, argv );
	auto file = reader::read_file<std::string>( "pixi.toml" );
	std::print( "file: {}", file.value() );
	return 0;
}
