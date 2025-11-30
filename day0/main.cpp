#include <print>

#include "args.hpp"

int main( int argc, char *argv[] )
{
	args::Parser parser( argc, argv );
	std::print( "Hello, World!" );
	return 0;
}
