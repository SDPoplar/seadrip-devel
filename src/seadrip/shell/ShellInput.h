#ifndef     __SD_SHELL_SHELL_INPUT_H__
#define     __SD_SHELL_SHELL_INPUT_H__

#include <string>
#include <map>
//  #include <unistd.h>

namespace SeaDrip
{
    class ShellDictionary
    {
    public:
        ShellDictionary( std::string name, std::string value );
        bool IsBoolean() const;

    private:
        std::string name;
        std::string value;
    };

    class ShellInput
    {
    public:
        std::map<std::string, std::string> Parse( int argc, char **argv );
        ShellInput* AppendInput( int flag, std::string alias );
        ShellInput* AppendBool( int flag, std::string alias, bool value );

    private:
        std::map<int, ShellDictionary> items;
    };
};

#endif
