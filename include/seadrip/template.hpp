#ifndef     __SD_CPPCODE_TEMPLATE_H__
#define     __SD_CPPCODE_TEMPLATE_H__

#include <string>
#include <boost/algorithm/string.hpp>
#include <iostream>

#define _SDTPL_EMPTY_MACRO

#define TEMPLATE_START( tpl_name ) _SDTPL_EMPTY_MACRO
#define TPL_REPLACE( flag ) _SDTPL_EMPTY_MACRO
#define TPL_REPLACE_KEEPSPACE( flag, space_keeper ) space_keeper

namespace SeaDrip
{
    namespace Template
    {
        std::string GetFrontSpace( std::string src, std::string FromFlag )
        {
            auto last = src.find( FromFlag );
            if( last == std::string::npos )
            {
                return "";
            }
            auto first = src.find_last_not_of( ' ', last -1 ) + 1;
            return src.substr( first, last - first );
        }

        std::string FillIndent( std::string src, std::string space )
        {
            std::string s = "\n" + space;
            return boost::replace_all_copy( src, "\n", s );
        }
    }
}

#endif
