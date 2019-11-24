#ifndef __SD_KVFILE_READER_H__
#define __SD_KVFILE_READER_H__

#include <string>
#include "IniFileReader.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>

namespace SeaDrip
{
    class KvFileReader : public IniFileReader
    {
        public:
            KvFileReader( std::string file_path, char note_flag ) : IniFileReader( file_path, note_flag ),
                m_regex_pattern( "([^=]+)=\\s*([^\\s]+)" ), m_s_last_key( "" ), m_s_last_val( "" ) {}

            bool Next( std::string key, std::string val )
            {
                if( this->NextLine() )
                {
                    key = this->m_s_last_key;
                    val = this->m_s_last_val;
                    return true;
                }
                return false;
            }
        protected:
            bool ParseKeyVal( std::string line )
            {
                boost::smatch matches;
                if( !boost::regex_search( line, matches, this->m_regex_pattern ) || (matches.size() < 3) )
                {
                    return false;
                }
                std::string _key = matches[ 1 ];
                this->m_s_last_key = boost::trim_copy( _key );
                std::string _val = matches[ 2 ];
                this->m_s_last_val = boost::trim_copy( _val );
                return true;
            }

            const boost::regex m_regex_pattern;

            std::string m_s_last_key;
            std::string m_s_last_val;
    };
};

#endif

