#ifndef __SD_KVFILE_READER_H__
#define __SD_KVFILE_READER_H__

#include <string>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>

namespace SeaDrip
{
    class KvFileReader : public std::ifstream
    {
        public:
            KvFileReader( std::string file_path, char note_flag ) : std::ifstream( file_path, std::ios::in ), m_c_note_flag( note_flag  ) {}
            ~KvFileReader()
            {
                if( this->is_open() )
                {
                    this->close();
                }
            }

            bool TryGetLine( std::string& out )
            {
                if( this->eof() )
                {
                    return false;
                }
                char line[ 128 ];
                this->getline( line, 128 );
                out = line;
                return true;
            }

            bool Next( std::string key, std::string val )
            {
                boost::regex pattern( "([^=]+)=\\s*([^\\s]+)" );
                std::string line;
                while( this->TryGetLine( line ) )
                {
                    std::string sline = boost::trim_copy( std::string( line ) );
                    if( sline.empty() || ( sline.c_str()[ 0 ] == this->m_c_note_flag ) )
                    {
                        continue;
                    }
                    boost::smatch matches;
                    if( !boost::regex_search( sline, matches, pattern ) || ( matches.size() < 3 ) )
                    {
                        continue;
                    }
                    key = boost::trim_copy( matches[ 1 ] );
                    val = boost::trim_copy( matches[ 2 ] );
                    return true;
                }
                return false;
            }
        protected:
            char m_c_note_flag;
    };
};

#endif

