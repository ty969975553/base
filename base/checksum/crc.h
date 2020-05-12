#pragma once

namespace base
{
    class Crc32
    {
    public:
        static unsigned int Calc(const unsigned char* data, size_t size);

    private:
        static unsigned int Update(unsigned int init, const unsigned char* data,
                                   size_t size);
    };

}  // namespace base
