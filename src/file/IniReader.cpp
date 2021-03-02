#include "../seadrip/file/KvFileReader.h"
#include <boost/algorithm/string.hpp>
using namespace SeaDrip;

#ifndef MAX_LINE_LEN
#define MAX_LINE_LEN    256
#endif

//  =================== IniFileReader   =======================================

IniFileReader::IniFileReader( std::string file_path, char note_flag )
    : std::ifstream( file_path, std::ios::in ), m_c_note_flag( note_flag ),
    m_u_line_index( 0 )
{}

IniFileReader::~IniFileReader()
{
    if( this->is_open() )
    {
        this->close();
    }
}

const bool IniFileReader::TryGetLine( std::string& out )
{
    if( this->eof() )
    {
        return false;
    }
    char line[ MAX_LINE_LEN ];
    this->getline( line, MAX_LINE_LEN );
    out = line;
    this->m_u_line_index++;
    return true;
}

const unsigned int IniFileReader::GetLineIndex( void ) const noexcept
{
    return this->m_u_line_index;
}

const bool IniFileReader::NextLine( std::string& valid_line )
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

KvFileReader::KvFileReader( std::string file_path, char note_flag )
    : IniFileReader( file_path, note_flag ), m_s_last_key( "" ), m_s_last_val( "" )
{}

const bool KvFileReader::Next( std::string& key, std::string& val )
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
    auto pos = line.find_first_of( '=' );
    this->m_s_last_key = boost::trim_copy( line.substr( 0, pos ) );
    this->m_s_last_val = boost::trim_copy( line.substr( pos + 1 ) );
    return true;
}
