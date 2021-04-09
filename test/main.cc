 #include "base/test/unittest.h"
 //#include "config.h"

 int wmain(int argc, wchar_t* argv[])
 {
     int result = Test::UnitTest::RunAndDisposeTests(argc, argv);
     return result;
 }

//#include <Windows.h>
//
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
//{
//    MessageBox(0,0,0,0);
//    return 0;
//}