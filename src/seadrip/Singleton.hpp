#ifndef __SD_SINGLETON_H__
#define __SD_SINGLETON_H__

#include <map>
#include <typeinfo>

namespace SeaDrip
{
    class Singleton
    {
        public:
            template<typename T> static T* Get()
            {
                auto hash = typeid( T ).hash_code();
                auto cursor = Singleton::ins.find( hash );
                if( cursor != Singleton::ins.end() )
                {
                    return (T*)cursor->second;
                }
                T* ret = new T();
                Singleton::ins[ hash ] = ( void * )ret;
                return ret;
            }
        private:
            Singleton() = delete;
            Singleton( const Singleton& ) = delete;
            Singleton& operator=( const Singleton& ) = delete;

            static std::map<size_t, void*> ins;
    };
};

std::map<size_t, void*> SeaDrip::Singleton::ins = {};

#define AS_SD_SINGLETON friend SeaDrip::Singleton
#define SD_SINGLETON_CLASS( class_name ) AS_SD_SINGLETON; private: class_name() = default

#endif
