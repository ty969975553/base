// cooy from Vlpp
#include <exception>
#include <functional>
#include <string>

#include "base/basic.h"

namespace Test
{
    using UnitTestFileProc = void (*)();

    class UnitTest
    {
       private:
       public:
        UnitTest() = delete;
        enum class MessageType
        {
            Info,
            Error,
            File,
            Category,
            Case
        };

        static void PrintMessage(const std::wstring& string, MessageType kind);
#ifdef _MSVC
        static int RunAndDisposeTests(int argc, wchar_t* argv[]);
#else
        static int RunAndDisposeTests(int argc, char* argv[]);
#endif

        static void RegisterTestFile(const char* fileName,
                                     UnitTestFileProc testProc);
        static void RunCategoryOrCase(const std::wstring& description,
                                      bool isCategory,
                                      std::function<void()>&& callback);
        static void EnsureLegalToAssert();
        ~UnitTest();
    };

    class UnitTestFile
    {
       public:
        UnitTestFile(const char* fileName, UnitTestFileProc testProc)
        {
            UnitTest::RegisterTestFile(fileName, testProc);
        }
    };

    struct UnitTestAssertError
    {
        const wchar_t* message;

        UnitTestAssertError(const wchar_t* _message) : message(_message) {}
    };

    struct UnitTestConfigError
    {
        const wchar_t* message;

        UnitTestConfigError(const wchar_t* _message) : message(_message) {}
    };

#define TEST_FILE                                                             \
    static void VLPPTEST_TESTFILE();                                          \
    static Test::UnitTestFile VLPPTEST_TESTFILE_INSTANCE(__FILE__,            \
                                                         &VLPPTEST_TESTFILE); \
    static void VLPPTEST_TESTFILE()

#define TEST_CATEGORY(DESCRIPTION) \
        Test::UnitTest::RunCategoryOrCase((DESCRIPTION), true, [&]()

#define TEST_CASE(DESCRIPTION) \
        Test::UnitTest::RunCategoryOrCase((DESCRIPTION), false, [&]()

#define TEST_ASSERT(CONDITION)                      \
    do                                              \
    {                                               \
        Test::UnitTest::EnsureLegalToAssert();      \
        if (!(CONDITION))                           \
            throw Test::UnitTestAssertError(        \
                L"Assertion failure: " #CONDITION); \
    } while (0)

#define TEST_ERROR(STATEMENT)                                          \
    do                                                                 \
    {                                                                  \
        Test::UnitTest::EnsureLegalToAssert();                         \
        try                                                            \
        {                                                              \
            STATEMENT;                                                 \
            throw Test::UnitTestAssertError(                           \
                L"Expect an error but nothing occurred: " #STATEMENT); \
        }                                                              \
        catch (const Test::UnitTestAssertError&)                       \
        {                                                              \
            throw;                                                     \
        }                                                              \
        catch (const Test::UnitTestConfigError&)                       \
        {                                                              \
            throw;                                                     \
        }                                                              \
    } while (0)

#define TEST_EXCEPTION(STATEMENT, EXCEPTION, ASSERT_FUNCTION)                  \
    do                                                                         \
    {                                                                          \
        auto __ASSERT_FUNCTION__ = ASSERT_FUNCTION;                            \
        try                                                                    \
        {                                                                      \
            STATEMENT;                                                         \
            throw Test::UnitTestAssertError(                                   \
                L"Expect [" #EXCEPTION "] but nothing occurred: " #STATEMENT); \
        }                                                                      \
        catch (const std::exception& e)                                        \
        {                                                                      \
            __ASSERT_FUNCTION__(e.what());                                     \
        }                                                                      \
        catch (...)                                                            \
        {                                                                      \
            throw Test::UnitTestAssertError(                                   \
                L"Expect [" #EXCEPTION                                         \
                "] but get unexpected exception: " #STATEMENT);                \
        }                                                                      \
    } while (0)

#define TEST_PRINT(MESSAGE) \
    Test::UnitTest::PrintMessage((MESSAGE), Test::UnitTest::MessageType::Info)

#define TEST_CASE_ASSERT(CONDITION) \
    TEST_CASE(L## #CONDITION) { TEST_ASSERT(CONDITION); })

}  // namespace Test
