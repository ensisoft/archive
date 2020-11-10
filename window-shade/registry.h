
// $Id: registry.h,v 1.2 2007/01/01 19:08:09 enska Exp $

#ifndef STEALTH_REGISTRY_H
#define STEALTH_REGISTRY_H

#include <cassert>
#include <string>
#include <windows.h>
#include <boost/scoped_array.hpp>
#include <base_exception.h>

namespace shade
{
    class registry
    {
        public:
            HKEY key;
        public: 
            registry() : key(0)
            {
            }
           ~registry()
            {
                close();
            }
            bool create(const std::string& path)
            {
                HKEY hkey = 0;
                LONG ret = RegCreateKeyEx(HKEY_CURRENT_USER,
                    path.c_str(),
                    0, 
                    NULL,
                    REG_OPTION_NON_VOLATILE,
                    KEY_ALL_ACCESS,
                    NULL,
                    &hkey,
                    NULL);

                if (ret == ERROR_SUCCESS)
                {
                    if (key)
                        RegCloseKey(key);
                    key = hkey;
                    return true;
                }
                return false;
            }
            bool open(const std::string& path)
            {
                HKEY hkey = 0;

                LONG ret = RegOpenKeyEx(
                      HKEY_CURRENT_USER,
                      path.c_str(),
                      0, 
                      KEY_QUERY_VALUE,
                      &hkey);

                if (ret == ERROR_SUCCESS)
                {
                    if (key)
                        RegCloseKey(key);
                    
                    key = hkey;
                    return true;
                }
                return false;
            }
            void close()
            {
                RegCloseKey(key);
                key = 0;
            }
        private:
            registry(const registry&);
            registry& operator=(const registry&);

    };

inline
int read_int(HKEY key, const std::string& name)
{
    assert(key);

    BYTE buffer[sizeof(DWORD)] = {};
    DWORD size = sizeof(DWORD);

    if (RegQueryValueEx(key,
            name.c_str(),
            NULL,
            NULL,
            buffer,
            &size) != ERROR_SUCCESS)
        throw nstd::system_exception("RegQueryValueEx", GetLastError());

    int i;
    memcpy(&i, buffer, sizeof(int));
    return i;
}

inline 
std::string read_string(HKEY key, const std::string& name)
{
    assert(key);

    std::string dest;

    DWORD regsize = 0;
    RegQueryValueEx(key,
        name.c_str(),
        NULL,
        NULL,
        NULL, 
        &regsize);
    
    boost::scoped_array<char> buff(new char[regsize]);
    if (RegQueryValueEx(key,
            name.c_str(),
            NULL,
            NULL,
            reinterpret_cast<BYTE*>(buff.get()),
            &regsize) != ERROR_SUCCESS)
        throw nstd::system_exception("RegQueryValueEx", GetLastError());

    dest.append(buff.get(), regsize);
    return dest;

}

inline 
bool has_value(HKEY key, const std::string& name)
{
    assert(key);
    return RegQueryValueEx(key,
        name.c_str(),
        NULL,
        NULL, 
        NULL,
        NULL) == ERROR_SUCCESS;
}

inline 
void write_int(HKEY key,
               const std::string& name,
               int ival)
{
    BYTE buffer[sizeof(int)];
    memcpy(buffer, &ival, sizeof(int));

    RegSetValueEx(key,
        name.c_str(),
        NULL,
        REG_DWORD,
        buffer,
        sizeof(int));
}

inline 
void write_string(HKEY key,
                  const std::string& name,
                  const std::string& sval)
{
    RegSetValueEx(key,
        name.c_str(),
        NULL,
        REG_SZ,
        reinterpret_cast<const BYTE*>(sval.c_str()),
        sval.length());
}

inline
void delete_value(HKEY key, const std::string& name)
{
    RegDeleteValue(key, name.c_str());
}
} //

#endif
