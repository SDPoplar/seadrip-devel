#ifndef __SD_INIFILE_READER_H__
#define __SD_INIFILE_READER_H__

#include <string>
#include <fstream>

namespace SeaDrip
{
    class IniFileReader : public std::ifstream
    {
    public:
        IniFileReader( std::string file_path, char note_flag );
        virtual ~IniFileReader();

        bool TryGetLine( std::string& out );

        bool NextLine( std::string& valid_line = *((std::string*)nullptr) );

    protected:
        virtual bool ParseLine( std::string ) { return true; };

        char m_c_note_flag;
    };
};

#endif

