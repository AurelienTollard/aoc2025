#pragma once

#include <expected>
#include <filesystem>
#include <fstream>
#include <string>

namespace reader
{

	enum class ReadError
	{
		FileNotFound,
		CannotOpen,
		ReadFailed,
	};

	template <typename T> inline std::expected<T, ReadError> read_file( const std::filesystem::path &path )
	{
		if( !std::filesystem::exists( path ) )
		{
			return std::unexpected( ReadError::FileNotFound );
		}

		std::ifstream file( path, std::ios::in | std::ios::binary );
		if( !file )
		{
			return std::unexpected( ReadError::CannotOpen );
		}

		file.seekg( 0, std::ios::end );
		const auto size = file.tellg();
		if( size < 0 )
		{
			return std::unexpected( ReadError::ReadFailed );
		}

		T content;
		content.resize( static_cast<size_t>( size ) );
		file.seekg( 0, std::ios::beg );

		if constexpr( std::same_as<T, std::string> )
		{
			if( !file.read( content.data(), size ) )
			{
				return std::unexpected( ReadError::ReadFailed );
			}
		}
		else
		{
			if( !file.read( reinterpret_cast<char *>( content.data() ), size ) )
			{
				return std::unexpected( ReadError::ReadFailed );
			}
		}

		return content;
	}

} // namespace reader
