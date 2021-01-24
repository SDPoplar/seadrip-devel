#ifndef __SD_KVFILE_READER_H__
#define __SD_KVFILE_READER_H__

#include "IniFileReader.h"

namespace SeaDrip
{
    class KvFileReader : public IniFileReader
    {
        public:
            KvFileReader( std::string file_path, char note_flag );

            const bool Next( std::string& key, std::string& val );

        protected:
            bool ParseLine( std::string line ) override;

            std::string m_s_last_key;
            std::string m_s_last_val;
    };
};

#endif

