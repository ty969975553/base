#include "base/win/scoped_handle.h"

#include <mutex>
#include <unordered_map>
#include <iostream>

namespace
{
    struct Info
    {
        const void* owner;
        const void* pc1;
        const void* pc2;
        DWORD thread_id;
    };
    typedef std::unordered_map<HANDLE, Info> HandleMap;

    class ActiveVerifier
    {
    public:
        explicit ActiveVerifier(bool enabled)
            : enabled_(enabled), closing_(false)
        {
        }

        // Retrieves the current verifier.
        static ActiveVerifier* Get();

        // The methods required by HandleTraits. They are virtual because we
        // need to forward the call execution to another module, instead of
        // letting the compiler call the version that is linked in the current
        // module.
        virtual bool CloseHandle(HANDLE handle);
        virtual void StartTracking(HANDLE handle, const void* owner,
                                   const void* pc1, const void* pc2);
        virtual void StopTracking(HANDLE handle, const void* owner,
                                  const void* pc1, const void* pc2);
        virtual void Disable();
        virtual void OnHandleBeingClosed(HANDLE handle);

    private:
        ~ActiveVerifier();  // Not implemented.

        static void InstallVerifier();

        bool enabled_;
        bool closing_;
        std::mutex lock_;
        HandleMap map_;
        DISALLOW_COPY_AND_ASSIGN(ActiveVerifier);
    };
    ActiveVerifier* g_active_verifier = NULL;

    // static
    ActiveVerifier* ActiveVerifier::Get()
    {
        if (!g_active_verifier)
        {
            //std::lock_guard<std::mutex> lck(lock_);
            g_active_verifier = new ActiveVerifier(true);
        }

        return g_active_verifier;
    }

    bool ActiveVerifier::CloseHandle(HANDLE handle)
    {
        if (!enabled_) return ::CloseHandle(handle);

        std::lock_guard<std::mutex> lck(lock_);
        closing_ = true;
        ::CloseHandle(handle);
        closing_ = false;

        return true;
    }

    void ActiveVerifier::StartTracking(HANDLE handle, const void* owner,
                                       const void* pc1, const void* pc2)
    {
        if (!enabled_) return;

        // Grab the thread id before the lock.
        DWORD thread_id = GetCurrentThreadId();

        std::lock_guard<std::mutex> lck(lock_);

        Info handle_info = {owner, pc1, pc2, thread_id};
        std::pair<HANDLE, Info> item(handle, handle_info);
        std::pair<HandleMap::iterator, bool> result = map_.insert(item);
        if (!result.second)
        {
            Info other = result.first->second;
            std::cout << other.thread_id;
        }
    }

    void ActiveVerifier::StopTracking(HANDLE handle, const void* owner,
                                      const void* pc1, const void* pc2)
    {
        if (!enabled_) return;

        std::lock_guard<std::mutex> lck(lock_);
        HandleMap::iterator i = map_.find(handle);
        if (i == map_.end()) std::cout << __FILE__;

        Info other = i->second;
        if (other.owner != owner)
        {
            std::cout << __FILE__;
        }

        map_.erase(i);
    }

    void ActiveVerifier::Disable() { enabled_ = false; }

    void ActiveVerifier::OnHandleBeingClosed(HANDLE handle)
    {
        if (!enabled_) return;

        std::lock_guard<std::mutex> lck(lock_);
        if (closing_) return;

        HandleMap::iterator i = map_.find(handle);
        if (i == map_.end()) return;

        Info other = i->second;
        std::cout << __FILE__;
    }

}  // namespace

namespace base
{
    namespace win
    {
        // Static.
        bool HandleTraits::CloseHandle(HANDLE handle)
        {
            return ActiveVerifier::Get()->CloseHandle(handle);
        }

        // Static.
        void VerifierTraits::StartTracking(HANDLE handle, const void* owner,
                                           const void* pc1, const void* pc2)
        {
            return ActiveVerifier::Get()->StartTracking(handle, owner, pc1,
                                                        pc2);
        }

        // Static.
        void VerifierTraits::StopTracking(HANDLE handle, const void* owner,
                                          const void* pc1, const void* pc2)
        {
            return ActiveVerifier::Get()->StopTracking(handle, owner, pc1, pc2);
        }

        void DisableHandleVerifier()
        {
            return ActiveVerifier::Get()->Disable();
        }

        void OnHandleBeingClosed(HANDLE handle)
        {
            return ActiveVerifier::Get()->OnHandleBeingClosed(handle);
        }

    }  // namespace win
}  // namespace base