#include "base/environment.h"

#include <iostream>
#include <memory>
#include <optional>
#include <string>

#include "base/test/unittest.h"
#include "ui/console/console.h"

using namespace base;
namespace Test
{
    TEST_FILE
    {
        TEST_CATEGORY(L"environment test")
        {
            // Semantics  语义
            TEST_CASE(L"GetVar")
            {
                std::unique_ptr<Environment> env(Environment::Create());
                std::string env_value;
                TEST_ASSERT(env->GetVar("PATH", &env_value) == true);
                TEST_ASSERT(env_value != "");
                std::cout << env_value << std::endl;
            });

            TEST_CASE(L"GetVarReverse")
            {
                std::unique_ptr<Environment> env(Environment::Create());
                const char kFooUpper[] = "FOO";
                const char kFooLower[] = "foo";

                // Set a variable in UPPER case.
                TEST_ASSERT(env->SetVar(kFooUpper, kFooLower) == true);

                // And then try to get this variable passing the lower case.
                std::string env_value;
                TEST_ASSERT(env->GetVar(kFooLower, &env_value) == true);

                TEST_ASSERT(env_value == kFooLower);

                TEST_ASSERT(env->UnSetVar(kFooUpper) == true);

                const char kBar[] = "bar";
                // Now do the opposite, set the variable in the lower case.
                TEST_ASSERT(env->SetVar(kFooLower, kBar) == true);

                // And then try to get this variable passing the UPPER case.
                TEST_ASSERT(env->GetVar(kFooUpper, &env_value) == true);

                TEST_ASSERT(env_value == kBar);

                TEST_ASSERT(env->UnSetVar(kFooLower) == true);
            });

            TEST_CASE(L"HasVar")
            {
                std::unique_ptr<Environment> env(Environment::Create());
                TEST_ASSERT(env->HasVar("PATH") == true);
            });

            TEST_CASE(L"SetVar")
            {
                std::unique_ptr<Environment> env(Environment::Create());
                const char kFooUpper[] = "FOO";
                const char kFooLower[] = "foo";
                TEST_ASSERT(env->SetVar(kFooUpper, kFooLower));

                // Now verify that the environment has the new variable.
                TEST_ASSERT(env->HasVar(kFooUpper));

                std::string var_value;
                TEST_ASSERT(env->GetVar(kFooUpper, &var_value));
                TEST_ASSERT(var_value == kFooLower);
            });

            TEST_CASE(L"UnSetVar")
            {
                std::unique_ptr<Environment> env(Environment::Create());

                const char kFooUpper[] = "FOO";
                const char kFooLower[] = "foo";
                // First set some environment variable.
                TEST_ASSERT(env->SetVar(kFooUpper, kFooLower));

                // Now verify that the environment has the new variable.
                TEST_ASSERT(env->HasVar(kFooUpper));

                // Finally verify that the environment variable was erased.
                TEST_ASSERT(env->UnSetVar(kFooUpper));

                // And check that the variable has been unset.
                TEST_ASSERT(env->HasVar(kFooUpper) == false);
            });

            TEST_CASE(L"AlterEnvironment")
            {
                const wchar_t empty[] = L"\0";
                const wchar_t a2[] = L"A=2\0";
                EnvironmentMap changes;
                std::wstring e;

                e = AlterEnvironment(empty, changes);
                TEST_ASSERT(0 == e[0]);

                changes[L"A"] = L"1";
                e = AlterEnvironment(empty, changes);
                TEST_ASSERT(std::wstring(L"A=1\0\0", 5) == e);

                changes.clear();
                changes[L"A"] = std::wstring();
                e = AlterEnvironment(empty, changes);
                TEST_ASSERT(std::wstring(L"\0\0", 2) == e);

                changes.clear();
                e = AlterEnvironment(a2, changes);
                TEST_ASSERT(std::wstring(L"A=2\0\0", 5) == e);

                changes.clear();
                changes[L"A"] = L"1";
                e = AlterEnvironment(a2, changes);
                TEST_ASSERT(std::wstring(L"A=1\0\0", 5) == e);

                changes.clear();
                changes[L"A"] = std::wstring();
                e = AlterEnvironment(a2, changes);
                TEST_ASSERT(std::wstring(L"\0\0", 2) == e);
            });
        });
    }
}  // namespace Test