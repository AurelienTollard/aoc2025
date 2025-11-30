#pragma once

#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace args
{

	class Parser
	{
	  public:
		Parser( int argc, char **argv )
		{
			for( int i = 1; i < argc; ++i )
			{
				std::string arg = argv[i];
				if( arg.starts_with( "--" ) )
				{
					auto eq_pos = arg.find( '=' );
					if( eq_pos != std::string::npos )
					{
						// --key=value
						auto key = arg.substr( 2, eq_pos - 2 );
						auto value = arg.substr( eq_pos + 1 );
						named_[key] = value;
					}
					else
					{
						// --key (check next arg for value or treat as flag)
						auto key = arg.substr( 2 );
						if( i + 1 < argc && !std::string( argv[i + 1] ).starts_with( "--" ) )
						{
							named_[key] = argv[++i];
						}
						else
						{
							flags_.insert( key );
						}
					}
				}
				else
				{
					positional_.push_back( arg );
				}
			}
		}

		std::optional<std::string> get( size_t index ) const
		{
			if( index < positional_.size() )
			{
				return positional_[index];
			}
			return std::nullopt;
		}

		template <typename T = std::string> std::optional<T> get( const std::string &key ) const
		{
			if( auto it = named_.find( key ); it != named_.end() )
			{
				if constexpr( std::is_same_v<T, std::string> )
				{
					return it->second;
				}
				else if constexpr( std::is_same_v<T, int> )
				{
					try
					{
						return std::stoi( it->second );
					}
					catch( const std::invalid_argument & )
					{
						return std::nullopt;
					}
				}
			}
			return std::nullopt;
		}

		bool has( const std::string &key ) const { return flags_.contains( key ); }

		std::string get_or( const std::string &key, const std::string &default_val ) const
		{
			return get( key ).value_or( default_val );
		}

		size_t count() const { return positional_.size(); }

	  private:
		std::vector<std::string> positional_;
		std::unordered_map<std::string, std::string> named_;
		std::unordered_set<std::string> flags_;
	};

} // namespace args
