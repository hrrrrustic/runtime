//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
//

// ---------------------------------------------------------------------------
// APIThreadStress.cpp  (API thread stresser)
// ---------------------------------------------------------------------------

#include "stdafx.h"

#include "apithreadstress.h"
#include "clrhost.h"
#include "ex.h"
#include "log.h"



// For now, thread stress is incompatible with hosting.  We need a host CreateThread
// to fix this.
#undef SetEvent
#undef ResetEvent

int APIThreadStress::s_threadStressCount = 0;

APIThreadStress::APIThreadStress()
{
    WRAPPER_NO_CONTRACT;

    m_threadCount = 0;

    // Don't "fork" stress threads
    if (ClrFlsGetValue(TlsIdx_StressThread) == NULL)
        m_threadCount = s_threadStressCount;

    if (m_threadCount != 0)
    {
        m_setupOK = TRUE;

        m_hThreadArray = new (nothrow) HANDLE [ m_threadCount ];
        if (m_hThreadArray == NULL)
            m_setupOK = FALSE;
        else
        {
            HANDLE *p = m_hThreadArray;
            HANDLE *pEnd = p + m_threadCount;
            
            while (p < pEnd)
            {
                DWORD id;
                *p = ::CreateThread(NULL, 0, StartThread, this, 0, &id);
                if (*p == NULL)
                    m_setupOK = FALSE;
                p++;
            }
        }

        m_syncEvent = ClrCreateManualEvent(FALSE);
        if (m_syncEvent == INVALID_HANDLE_VALUE)
            m_setupOK = FALSE;
    }
}

APIThreadStress::~APIThreadStress()
{
    WRAPPER_NO_CONTRACT;

    if (m_threadCount > 0)
    {
        HANDLE *p = m_hThreadArray;
        HANDLE *pEnd = p + m_threadCount;

        if (p != NULL)
        {
            while (p < pEnd)
            {
                if (*p != NULL)
                {
                    if (m_threadCount > 0 && m_setupOK)
                        WaitForSingleObjectEx(*p, INFINITE, FALSE);

                    ::CloseHandle(*p);
                }
                p++;
            }
            delete [] m_hThreadArray;
        }

        if (m_syncEvent != INVALID_HANDLE_VALUE)
            CloseHandle(m_syncEvent);
    }
}

void APIThreadStress::SetThreadStressCount(int threadCount)
{
    LIMITED_METHOD_CONTRACT;

    s_threadStressCount = threadCount;
}


DWORD WINAPI APIThreadStress::StartThread(void *arg)
{
    WRAPPER_NO_CONTRACT;
    STATIC_CONTRACT_SO_NOT_MAINLINE;    // not mainline so scenario

    APIThreadStress *pThis = (APIThreadStress *) arg;

    ClrFlsSetValue(TlsIdx_StressThread, pThis);

    EX_TRY
    {
        // Perform initial synchronization
        WaitForSingleObjectEx(pThis->m_syncEvent, INFINITE, FALSE);
        InterlockedIncrement(&pThis->m_runCount);

        LOG((LF_ALL, LL_INFO100, "Stressing operation on thread %d\n", GetCurrentThreadId()));
        ((APIThreadStress *)arg)->Invoke();
        LOG((LF_ALL, LL_INFO100, "End stress operation on thread %d\n", GetCurrentThreadId()));

        if (InterlockedDecrement(&pThis->m_runCount) == 0)
            ::SetEvent(pThis->m_syncEvent);
    }
    EX_CATCH
    {
        LOG((LF_ALL, LL_ERROR, "Exception during stress operation on thread %d\n", GetCurrentThreadId()));
    }
    EX_END_CATCH(SwallowAllExceptions);

    return 0;
}

BOOL APIThreadStress::DoThreadStress()
{
    WRAPPER_NO_CONTRACT;

    if (m_threadCount > 0 && m_setupOK)
    {
        HANDLE *p = m_hThreadArray;
        HANDLE *pEnd = p + m_threadCount;

        while (p < pEnd)
        {
            ::ResumeThread(*p);
            p++;
        }

        // Start the threads at the same time
        ::SetEvent(m_syncEvent);

        return TRUE;
    }
    else
    {
        SyncThreadStress();
        return FALSE;
    }
}

void APIThreadStress::SyncThreadStress()
{
    WRAPPER_NO_CONTRACT;

    APIThreadStress *pThis = (APIThreadStress *) ClrFlsGetValue(TlsIdx_StressThread);

    if (pThis != NULL)
    {
        LOG((LF_ALL, LL_INFO1000, "Syncing stress operation on thread %d\n", GetCurrentThreadId()));

        ::ResetEvent(pThis->m_syncEvent);

        if (InterlockedDecrement(&pThis->m_runCount) == 0)
            ::SetEvent(pThis->m_syncEvent);
        else
            WaitForSingleObjectEx(pThis->m_syncEvent, INFINITE, FALSE);
        InterlockedIncrement(&pThis->m_runCount);

        LOG((LF_ALL, LL_INFO1000, "Resuming stress operation on thread %d\n", GetCurrentThreadId()));
    }
}

