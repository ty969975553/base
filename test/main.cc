#include "base/test/test.h"

int wmain(int argc, wchar_t* argv[])
{
    int result = Test::UnitTest::RunAndDisposeTests(argc, argv);
    return result;
}