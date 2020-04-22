#include "test/Test.h"
#ifdef _MSVC
#include <excpt.h>
#endif
#include "UI/Console/Console.h"

namespace Test
{
    using namespace UI::Console;
    namespace detail
    {
        struct UnitTestLink
        {
            const char* fileName;
            UnitTestFileProc testProc = nullptr;
            UnitTestLink* next = nullptr;
        };

        UnitTestLink* testHead = nullptr;
        UnitTestLink** testTail = &testHead;

        enum class UnitTestContextKind
        {
            File,
            Category,
            Case,
        };

        struct UnitTestContext
        {
            UnitTestContext* parent = nullptr;
            std::wstring indentation;
            UnitTestContextKind kind = UnitTestContextKind::File;
            bool failed = false;
        };

        UnitTestContext* testContext = nullptr;
        int totalFiles = 0;
        int passedFiles = 0;
        int totalCases = 0;
        int passedCases = 0;
        bool suppressFailure = false;

        template<typename TMessage>
        void RecordFailure(TMessage errorMessage)
        {
            UnitTest::PrintMessage(errorMessage, UnitTest::MessageType::Error);
            auto current = testContext;
            while (current)
            {
                current->failed = true;
                current = current->parent;
            }
        }

        template<typename TCallback>
        void SuppressCppFailure(TCallback&& callback)
        {
            try
            {
                callback();
            }
            catch (const UnitTestAssertError& e)
            {
                RecordFailure(e.message);
            }
            catch (const UnitTestConfigError& e)
            {
                RecordFailure(e.message);
            }
            catch (const std::exception& e)
            {
                RecordFailure(e.what());
            }
            catch (...)
            {
                RecordFailure(L"Unknown exception occurred!");
            }
        }

        template<typename TCallback>
        void SuppressCFailure(TCallback&& callback)
        {
#ifdef _MSVC
            __try
            {
                SuppressCppFailure(std::forward<TCallback&&>(callback));
            }
            __except (EXCEPTION_EXECUTE_HANDLER)
            {
                RecordFailure(L"Runtime exception occurred!");
            }
#else
            SuppressCppFailure(callback);
#endif
        }

    }

    using namespace detail;

    void UnitTest::PrintMessage(const std::std::wstring& string, MessageType kind)
    {
        if (kind != MessageType::Error && !testContext)
        {
            throw UnitTestConfigError(L"Cannot print message when unit test is not running.");
        }

        switch (kind)
        {
        case MessageType::Error:
            Console::SetColor(true, false, false, true);
            break;
        case MessageType::Info:
            Console::SetColor(true, true, true, true);
            break;
        case MessageType::File:
            Console::SetColor(true, false, true, true);
            break;
        case MessageType::Category:
            Console::SetColor(true, true, false, true);
            break;
        case MessageType::Case:
            Console::SetColor(false, true, false, true);
            break;
        }
        Console::WriteLine((testContext ? testContext->indentation : L"") + string);
        Console::SetColor(true, true, true, false);
    }

#ifdef _MSVC
    int UnitTest::RunAndDisposeTests(int argc, wchar_t* argv[])
#else
    int UnitTest::RunAndDisposeTests(int argc, char* argv[])
#endif
    {
        if (argc < 3)
        {
            if (argc == 2)
            {
#ifdef _MSVC
                std::wstring option = argv[1];
#else
                std::string option = atow(argv[1]);
#endif
                if (option == L"/D")
                {
                    suppressFailure = false;
                }
                else if (option == L"/R")
                {
                    suppressFailure = true;
                }
                else
                {
                    goto PRINT_USAGE;
                }
            }
            else
            {
#ifdef _MSVC
                if (IsDebuggerPresent())
                {
                    suppressFailure = false;
                }
                else
                {
                    suppressFailure = true;
                }
#else
                suppressFailure = true;
#endif
            }

            auto current = testHead;
            testHead = nullptr;
            testTail = nullptr;

            UnitTestContext context;
            testContext = &context;
            totalFiles = 0;
            passedFiles = 0;
            totalCases = 0;
            passedCases = 0;

            if (suppressFailure)
            {
                PrintMessage(L"Failures are suppressed.", MessageType::Info);
            }
            else
            {
                PrintMessage(L"Failures are not suppressed.", MessageType::Info);
            }

            while (current)
            {
                context.failed = false;
                PrintMessage(atow(current->fileName), MessageType::File);
                context.indentation = L"    ";
                ExecuteAndSuppressFailure(current->testProc);
                if (!testContext->failed)
                    passedFiles++;
                totalFiles++;
                context.indentation = L"";
                auto temp = current;
                current = current->next;
                delete temp;
            }

            bool passed = totalFiles == passedFiles;
            auto MessageType = passed ? MessageType::Case : MessageType::Error;
            PrintMessage(L"Passed test files: " + itow(passedFiles) + L"/" + itow(totalFiles), MessageType);
            PrintMessage(L"Passed test cases: " + itow(passedCases) + L"/" + itow(totalCases), MessageType);
            testContext = nullptr;
            return passed ? 0 : 1;
        }
    PRINT_USAGE:
        PrintMessage(L"Usage: [/D | /R]", MessageType::Error);
        return 1;
    }

    void UnitTest::RegisterTestFile(const char* fileName, UnitTestFileProc testProc)
    {
        auto link = new UnitTestLink;
        link->fileName = fileName;
        link->testProc = testProc;
        *testTail = link;
        testTail = &link->next;
    }

    void UnitTest::RunCategoryOrCase(const std::wstring& description, bool isCategory, Func<void()>&& callback)
    {
        if (!testContext)
            throw UnitTestConfigError(L"TEST_CATEGORY is not allowed to execute outside of TEST_FILE.");
        if (testContext->kind == UnitTestContextKind::Case)
            throw UnitTestConfigError(
                isCategory ?
                    L"TEST_CATEGORY is not allowed to execute inside TEST_CASE" :
                    L"TEST_CASE is not allowed to execute inside TEST_CASE");

        PrintMessage(description, (isCategory ? MessageType::Category : MessageType::Case));

        UnitTestContext context;
        context.parent = testContext;
        context.indentation = testContext->indentation + L"    ";
        context.kind = isCategory ? UnitTestContextKind::Category : UnitTestContextKind::Case;

        testContext = &context;
        ExecuteAndSuppressFailure(callback);
        if (!isCategory)
        {
            if (!testContext->failed)
                passedCases++;
            totalCases++;
        }
        testContext = context.parent;
    }

    void UnitTest::EnsureLegalToAssert()
    {
        if (!testContext)
            throw UnitTestConfigError(L"Assertion is not allowed to execute outside of TEST_CASE.");
        if (testContext->kind != UnitTestContextKind::Case)
            throw UnitTestConfigError(L"Assertion is not allowed to execute outside of TEST_CASE.");
    }
} // namespace Test
