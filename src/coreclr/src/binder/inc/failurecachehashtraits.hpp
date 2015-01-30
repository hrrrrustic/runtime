//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
//
// ============================================================
//
// FailureCache.hpp
//


//
// Defines the FailureCache class
//
// ============================================================

#ifndef __BINDER__FAILURE_CACHE_HASH_TRAITS_HPP__
#define __BINDER__FAILURE_CACHE_HASH_TRAITS_HPP__

#include "bindertypes.hpp"
#include "utils.hpp"
#include "sstring.h"
#include "shash.h"

namespace BINDER_SPACE
{
    class FailureCacheEntry
    {
    public:
        inline FailureCacheEntry()
        {
            m_hrBindingResult = S_OK;
        }
        inline ~FailureCacheEntry()
        {
            // Nothing to do here
        }

        // Getters/Setters
        inline SString &GetAssemblyNameOrPath()
        {
            return m_assemblyNameOrPath;
        }
        inline HRESULT GetBindingResult()
        {
            return m_hrBindingResult;
        }
        inline void SetBindingResult(HRESULT hrBindingResult)
        {
            m_hrBindingResult = hrBindingResult;
        }

    protected:
        SString m_assemblyNameOrPath;
        HRESULT m_hrBindingResult;
    };

    class FailureCacheHashTraits : public DefaultSHashTraits<FailureCacheEntry *>
    {
    public:
        typedef SString& key_t;
 
        // GetKey, Equals, and Hash can throw due to SString
        static const bool s_NoThrow = false;

        static key_t GetKey(element_t pFailureCacheEntry)
        {
            return pFailureCacheEntry->GetAssemblyNameOrPath();
        }
        static BOOL Equals(key_t pAssemblyNameOrPath1, key_t pAssemblyNameOrPath2)
        {
            return EqualsCaseInsensitive(pAssemblyNameOrPath1, pAssemblyNameOrPath2);
        }
        static count_t Hash(key_t pAssemblyNameOrPath)
        {
            return HashCaseInsensitive(pAssemblyNameOrPath);
        }
        static const element_t Null()
        {
            return NULL;
        }
        static bool IsNull(const element_t &propertyEntry)
        {
            return (propertyEntry == NULL);
        }

    };
};

#endif
