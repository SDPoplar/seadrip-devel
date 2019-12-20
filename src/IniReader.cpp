#include "seadrip/KvFileReader.h"
#include <boost/algorithm/string.hpp>
using namespace SeaDrip;

#ifndef MAX_LINE_LEN
#define MAX_LINE_LEN    256
#endif

//  =================== IniFileReader   =======================================

IniFileReader::IniFileReader( std::string file_path, char note_flag )
    : std::ifstream( file_path, std::ios::in ), m_c_note_flag( note_flag )
{}

IniFileReader::~IniFileReader()
{
    if( this->is_open() )
    {
        this->close();
    }
}

bool IniFileReader::TryGetLine( std::string& out )
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

bool IniFileReader::NextLine( std::string& valid_line )
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

//  =====================   KvFileReader    =========================================

KvFileReader::KvFileReader( std::string file_path, char note_flag ) : IniFileReader( file_path, note_flag ),
    m_regex_pattern( "([^=]+)=\\s*([^\\s]+)" ), m_s_last_key( "" ), m_s_last_val( "" )
{}

bool KvFileReader::Next( std::string& key, std::string& val )
{
    if( this->NextLine() )
    {
        key = this->m_s_last_key;
        val = this->m_s_last_val;
        return true;
    }
    return false;
}

bool KvFileReader::ParseLine( std::string line )
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
