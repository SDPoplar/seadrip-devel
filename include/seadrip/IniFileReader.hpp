#ifndef __SD_INIFILE_READER_H__
#define __SD_INIFILE_READER_H__

#ifndef MAX_LINE_LEN
#define MAX_LINE_LEN    128
#endif

#include <string>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>

namespace SeaDrip
{
    class IniFileReader : public std::ifstream
    {
    public:
        IniFileReader( std::string file_path, char note_flag ) : std::ifstream( file_path, std::ios::in ), m_c_note_flag( note_flag ) {}
        virtual ~IniFileReader()
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
            char line[ MAX_LINE_LEN ];
            this->getline( line, MAX_LINE_LEN );
            out = line;
            return true;
        }

        bool NextLine( std::string& valid_line = *((std::string*)nullptr) )
        {
            std::string line;
            while( this->TryGetLine( line ) )
            {
                std::string sline = boost::trim_copy( std::string( line ) );
                if( sline.empty() || (sline.c_str()[ 0 ] == this->m_c_note_flag) )
                {
                    continue;
                }
                if( this->ParseLine( sline ) )
                {
                    if( &valid_line )
                    {
                        valid_line = sline;
                    }
                    return true;
                }
            }
            return false;
        }

    protected:
        virtual bool ParseLine( std::string ) { return true; };

        char m_c_note_flag;
    };
};

#endif

