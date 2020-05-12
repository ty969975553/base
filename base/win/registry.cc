//
// WinRegistryKey.cpp
//
#include "base/win/registry.h"

namespace base
{
    template <class T>
    class Buffer
    {
    public:
        Buffer(int size) : _size(size) { _ptr = new T[size]; }

        ~Buffer() { delete[] _ptr; }

        T* begin() { return _ptr; }

        const T* begin() const { return _ptr; }

        T& operator[](int index) { return _ptr[index]; }

        const T& operator[](int index) const { return _ptr[index]; }

        operator T*() { return _ptr; }

    private:
        T* _ptr;
        int _size;
    };

    namespace
    {
        class AutoHandle
        {
        public:
            AutoHandle(HMODULE h) : _h(h) {}

            ~AutoHandle() { FreeLibrary(_h); }

            HMODULE handle() { return _h; }

        private:
            HMODULE _h;
        };
    }  // namespace

    namespace win
    {
        Registry::Registry(const std::string& key, bool readOnly,
                           REGSAM extraSam)
            : _hKey(0), _readOnly(readOnly), _extraSam(extraSam)
        {
            std::string::size_type pos = key.find('\\');
            if (pos != std::string::npos)
            {
                std::string rootKey = key.substr(0, pos);
                _hRootKey = handleFor(rootKey);
                _subKey = key.substr(pos + 1);
            }
        }

        Registry::Registry(HKEY hRootKey, const std::string& subKey,
                           bool readOnly, REGSAM extraSam)
            : _hRootKey(hRootKey),
              _subKey(subKey),
              _hKey(0),
              _readOnly(readOnly),
              _extraSam(extraSam)
        {
        }

        Registry::~Registry() { close(); }

        bool Registry::setString(const std::string& name,
                                 const std::string& value)
        {
            if (!open()) return false;

            if (RegSetValueExA(_hKey, name.c_str(), 0, REG_SZ,
                               (CONST BYTE*)value.c_str(),
                               (DWORD)value.size() + 1) != ERROR_SUCCESS)
                return false;

            return true;
        }

        std::string Registry::getString(const std::string& name)
        {
            if (!open()) return std::string();

            DWORD type;
            DWORD size;

            if (RegQueryValueExA(_hKey, name.c_str(), NULL, &type, NULL,
                                 &size) != ERROR_SUCCESS ||
                (type != REG_SZ && type != REG_EXPAND_SZ && type != REG_LINK))
                return std::string();

            if (size > 0)
            {
                Buffer<char> buffer(size + 1);
                size += 1;
                RegQueryValueExA(_hKey, name.c_str(), NULL, NULL,
                                 (BYTE*)buffer.begin(), &size);
                buffer[size] = 0;
                std::string result(buffer.begin());
                return result;
            }

            return std::string();
        }

        bool Registry::setStringExpand(const std::string& name,
                                       const std::string& value)
        {
            if (!open()) return false;

            if (RegSetValueExA(_hKey, name.c_str(), 0, REG_EXPAND_SZ,
                               (CONST BYTE*)value.c_str(),
                               (DWORD)value.size() + 1) != ERROR_SUCCESS)
                return false;

            return true;
        }

        bool Registry::getStringExpand(const std::string& name,
                                       std::string& value)
        {
            if (!open()) return false;

            DWORD type;
            DWORD size;

            if (RegQueryValueExA(_hKey, name.c_str(), NULL, &type, NULL,
                                 &size) != ERROR_SUCCESS ||
                (type != REG_SZ && type != REG_EXPAND_SZ && type != REG_LINK))
                return false;

            if (size > 0)
            {
                Buffer<char> buffer(size + 1);
                RegQueryValueExA(_hKey, name.c_str(), NULL, NULL,
                                 (BYTE*)buffer.begin(), &size);
                buffer[size] = 0;
                char temp;
                DWORD expSize = ExpandEnvironmentStringsA(buffer, &temp, 1);
                Buffer<char> expBuffer(expSize);
                ExpandEnvironmentStringsA(buffer.begin(), expBuffer.begin(),
                                          expSize);
                std::string result(expBuffer.begin());

                value = result;
            }

            return true;
        }

        bool Registry::setBinary(const std::string& name,
                                 const std::vector<char>& value)
        {
            if (!open()) return false;

            if (RegSetValueExA(_hKey, name.c_str(), 0, REG_BINARY,
                               (CONST BYTE*)&value[0],
                               (DWORD)value.size()) != ERROR_SUCCESS)
                return false;

            return true;
        }

        std::vector<char> Registry::getBinary(const std::string& name)
        {
            if (!open()) return std::vector<char>();

            DWORD type;
            DWORD size;
            std::vector<char> result;

            if (RegQueryValueExA(_hKey, name.c_str(), NULL, &type, NULL,
                                 &size) != ERROR_SUCCESS ||
                type != REG_BINARY)
                return std::vector<char>();

            if (size > 0)
            {
                result.resize(size);
                RegQueryValueExA(_hKey, name.c_str(), NULL, NULL,
                                 (BYTE*)&result[0], &size);
            }

            return result;
        }

        bool Registry::setInt(const std::string& name, int value)
        {
            if (!open()) return false;

            DWORD data = value;
            if (RegSetValueExA(_hKey, name.c_str(), 0, REG_DWORD,
                               (CONST BYTE*)&data,
                               sizeof(data)) != ERROR_SUCCESS)
                return false;

            return true;
        }

        bool Registry::getInt(const std::string& name, int& value)
        {
            if (!open()) return false;

            DWORD type;
            DWORD data;
            DWORD size = sizeof(data);

            if (RegQueryValueExA(_hKey, name.c_str(), NULL, &type, (BYTE*)&data,
                                 &size) != ERROR_SUCCESS ||
                (type != REG_DWORD && type != REG_DWORD_BIG_ENDIAN))
                return false;
            value = data;

            return true;
        }

        bool Registry::setInt64(const std::string& name, Int64 value)
        {
            if (!open()) return false;

            if (RegSetValueExA(_hKey, name.c_str(), 0, REG_QWORD,
                               (CONST BYTE*)&value,
                               sizeof(value)) != ERROR_SUCCESS)
                return false;

            return true;
        }

        bool Registry::getInt64(const std::string& name, Registry::Int64& value)
        {
            if (!open()) return false;

            DWORD type;
            Registry::Int64 data;
            DWORD size = sizeof(data);

            if (RegQueryValueExA(_hKey, name.c_str(), NULL, &type, (BYTE*)&data,
                                 &size) != ERROR_SUCCESS ||
                type != REG_QWORD)
                return false;

            value = data;

            return true;
        }

        bool Registry::deleteValue(const std::string& name)
        {
            if (!open()) return false;
            if (RegDeleteValueA(_hKey, name.c_str()) != ERROR_SUCCESS)
                return false;
            return true;
        }

        bool Registry::deleteKey()
        {
            Keys keys;
            subKeys(keys);
            close();
            for (Keys::iterator it = keys.begin(); it != keys.end(); ++it)
            {
                std::string subKey(_subKey);
                subKey += "\\";
                subKey += *it;
                Registry subRegKey(_hRootKey, subKey);
                subRegKey.deleteKey();
            }

            // NOTE: RegDeleteKeyEx is only available on Windows XP 64-bit SP3,
            // Windows Vista or later. We cannot call it directly as this would
            // prevent the code running on Windows XP 32-bit. Therefore, if we
            // need to call RegDeleteKeyEx (_extraSam != 0) we need to check for
            // its existence in ADVAPI32.DLL and call it indirectly.
            typedef LONG(WINAPI * RegDeleteKeyExAFunc)(
                HKEY hKey, const char* lpSubKey, REGSAM samDesired,
                DWORD Reserved);
            if (_extraSam != 0)
            {
                AutoHandle advAPI32(LoadLibraryA("ADVAPI32.DLL"));
                if (advAPI32.handle())
                {
                    RegDeleteKeyExAFunc pRegDeleteKeyExA =
                        reinterpret_cast<RegDeleteKeyExAFunc>(GetProcAddress(
                            advAPI32.handle(), "RegDeleteKeyExA"));
                    if (pRegDeleteKeyExA)
                    {
                        if ((*pRegDeleteKeyExA)(_hRootKey, _subKey.c_str(),
                                                _extraSam, 0) != ERROR_SUCCESS)
                            return false;
                        return true;
                    }
                }
            }

            if (RegDeleteKeyA(_hRootKey, _subKey.c_str()) != ERROR_SUCCESS)
                return false;
            return true;
        }

        bool Registry::exists()
        {
            HKEY hKey;
            if (RegOpenKeyExA(_hRootKey, _subKey.c_str(), 0,
                              KEY_READ | _extraSam, &hKey) == ERROR_SUCCESS)
            {
                RegCloseKey(hKey);
                return true;
            }
            return false;
        }

        Registry::Type Registry::type(const std::string& name)
        {
            open();
            DWORD type = REG_NONE;
            DWORD size;

            if (RegQueryValueExA(_hKey, name.c_str(), NULL, &type, NULL,
                                 &size) != ERROR_SUCCESS)
                return REGT_ERROR;

            Type aType = (Type)type;
            return aType;
        }

        bool Registry::exists(const std::string& name)
        {
            bool exists = false;
            HKEY hKey;
            if (RegOpenKeyExA(_hRootKey, _subKey.c_str(), 0,
                              KEY_READ | _extraSam, &hKey) == ERROR_SUCCESS)
            {
                exists = RegQueryValueExA(hKey, name.c_str(), NULL, NULL, NULL,
                                          NULL) == ERROR_SUCCESS;
                RegCloseKey(hKey);
            }
            return exists;
        }

        bool Registry::open()
        {
            if (!_hKey)
            {
                if (_readOnly)
                {
                    if (RegOpenKeyExA(_hRootKey, _subKey.c_str(), 0,
                                      KEY_READ | _extraSam,
                                      &_hKey) != ERROR_SUCCESS)
                        return false;
                }
                else
                {
                    if (RegCreateKeyExA(_hRootKey, _subKey.c_str(), 0, NULL,
                                        REG_OPTION_NON_VOLATILE,
                                        KEY_READ | KEY_WRITE | _extraSam, NULL,
                                        &_hKey, NULL) != ERROR_SUCCESS)
                        return false;
                }
            }

            return true;
        }

        void Registry::close()
        {
            if (_hKey)
            {
                RegCloseKey(_hKey);
                _hKey = 0;
            }
        }

        std::string Registry::key() const
        {
            std::string result;
            if (_hRootKey == HKEY_CLASSES_ROOT) result = "HKEY_CLASSES_ROOT";
#if defined(HKEY_CURRENT_CONFIG)
            else if (_hRootKey == HKEY_CURRENT_CONFIG)
                result = "HKEY_CURRENT_CONFIG";
#endif
            else if (_hRootKey == HKEY_CURRENT_USER)
                result = "HKEY_CURRENT_USER";
            else if (_hRootKey == HKEY_LOCAL_MACHINE)
                result = "HKEY_LOCAL_MACHINE";
            else if (_hRootKey == HKEY_USERS)
                result = "HKEY_USERS";
#if defined(HKEY_PERFORMANCE_DATA)
            else if (_hRootKey == HKEY_PERFORMANCE_DATA)
                result = "HKEY_PERFORMANCE_DATA";
#endif
            else
                result = "(UNKNOWN)";
            result += '\\';
            result += _subKey;
            return result;
        }

        std::string Registry::key(const std::string& valueName) const
        {
            std::string result = key();
            if (!valueName.empty())
            {
                result += '\\';
                result += valueName;
            }
            return result;
        }

        HKEY Registry::handle()
        {
            open();
            return _hKey;
        }

        HKEY Registry::handleFor(const std::string& rootKey)
        {
            if (rootKey == std::string("HKEY_CLASSES_ROOT"))
                return HKEY_CLASSES_ROOT;
#if defined(HKEY_CURRENT_CONFIG)
            else if (rootKey == "HKEY_CURRENT_CONFIG")
                return HKEY_CURRENT_CONFIG;
#endif
            else if (rootKey == "HKEY_CURRENT_USER")
                return HKEY_CURRENT_USER;
            else if (rootKey == "HKEY_LOCAL_MACHINE")
                return HKEY_LOCAL_MACHINE;
            else if (rootKey == "HKEY_USERS")
                return HKEY_USERS;
#if defined(HKEY_PERFORMANCE_DATA)
            else if (rootKey == "HKEY_PERFORMANCE_DATA")
                return HKEY_PERFORMANCE_DATA;
#endif
            else
                return 0;
        }

        void Registry::subKeys(Registry::Keys& keys)
        {
            open();

            DWORD subKeyCount = 0;
            DWORD valueCount = 0;

            if (RegQueryInfoKey(_hKey, NULL, NULL, NULL, &subKeyCount, NULL,
                                NULL, &valueCount, NULL, NULL, NULL,
                                NULL) != ERROR_SUCCESS)
                return;

            char buf[256];
            DWORD bufSize = sizeof(buf);
            for (DWORD i = 0; i < subKeyCount; ++i)
            {
                if (RegEnumKeyExA(_hKey, i, buf, &bufSize, NULL, NULL, NULL,
                                  NULL) == ERROR_SUCCESS)
                {
                    std::string name(buf);
                    keys.push_back(name);
                }
                bufSize = sizeof(buf);
            }
        }

        void Registry::values(Registry::Values& vals)
        {
            open();

            DWORD valueCount = 0;

            if (RegQueryInfoKey(_hKey, NULL, NULL, NULL, NULL, NULL, NULL,
                                &valueCount, NULL, NULL, NULL,
                                NULL) != ERROR_SUCCESS)
                return;

            char buf[256];
            DWORD bufSize = sizeof(buf);
            for (DWORD i = 0; i < valueCount; ++i)
            {
                if (RegEnumValueA(_hKey, i, buf, &bufSize, NULL, NULL, NULL,
                                  NULL) == ERROR_SUCCESS)
                {
                    std::string name(buf);
                    vals.push_back(name);
                }
                bufSize = sizeof(buf);
            }
        }
    }  // namespace win
}  // namespace base