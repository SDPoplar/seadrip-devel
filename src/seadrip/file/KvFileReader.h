#ifndef __SD_KVFILE_READER_H__
#define __SD_KVFILE_READER_H__

#include "IniFileReader.h"
#include <boost/regex.hpp>

namespace SeaDrip
{
    class KvFileReader : public IniFileReader
    {
        public:
            KvFileReader( std::string file_path, char note_flag );

            bool Next( std::string& key, std::string& val );

        protected:
            bool ParseLine( std::string line ) override;

            const boost::regex m_regex_pattern;

            std::string m_s_last_key;
            std::string m_s_last_val;
    };
};

#endif

