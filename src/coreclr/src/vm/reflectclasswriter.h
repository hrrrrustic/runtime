//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
//
//

//

#ifndef _REFCLASSWRITER_H_
#define _REFCLASSWRITER_H_

#include "iceefilegen.h"

// RefClassWriter
// This will create a Class
class RefClassWriter {
protected:
    friend class COMDynamicWrite;
	IMetaDataEmit2*			m_emitter;			// Emit interface.
	IMetaDataImport*		m_importer;			// Import interface.
	IMDInternalImport*		m_internalimport;	// Scopeless internal import interface
	ICeeGen*				m_pCeeGen;
    ICeeFileGen*            m_pCeeFileGen;
    HCEEFILE                m_ceeFile;
	IMetaDataEmitHelper*	m_pEmitHelper;
	ULONG					m_ulResourceSize;
    mdFile                  m_tkFile;
    IMetaDataEmit*          m_pOnDiskEmitter;

public:
    RefClassWriter() {
        LIMITED_METHOD_CONTRACT;
        m_pOnDiskEmitter = NULL;
    }

	HRESULT		Init(ICeeGen *pCeeGen, IUnknown *pUnk, LPCWSTR szName);

	IMetaDataEmit2* GetEmitter() {
        LIMITED_METHOD_CONTRACT;
		return m_emitter;
	}

	IMetaDataEmitHelper* GetEmitHelper() {
        LIMITED_METHOD_CONTRACT;
		return m_pEmitHelper;
	}

	IMetaDataImport* GetRWImporter() {
        LIMITED_METHOD_CONTRACT;
		return m_importer;
	}

	IMDInternalImport* GetMDImport() {
        LIMITED_METHOD_CONTRACT;
		return m_internalimport;
	}

	ICeeGen* GetCeeGen() {
        LIMITED_METHOD_CONTRACT;
		return m_pCeeGen;
	}

	ICeeFileGen* GetCeeFileGen() {
        LIMITED_METHOD_CONTRACT;
		return m_pCeeFileGen;
	}

	HCEEFILE GetHCEEFILE() {
        LIMITED_METHOD_CONTRACT;
		return m_ceeFile;
	}

    IMetaDataEmit* GetOnDiskEmitter() {
        LIMITED_METHOD_CONTRACT;
        return m_pOnDiskEmitter;
    }

    void SetOnDiskEmitter(IMetaDataEmit *pOnDiskEmitter) {
        CONTRACTL {
            NOTHROW;
            GC_TRIGGERS;
            // we know that the com implementation is ours so we use mode-any to simplify
            // having to switch mode 
            MODE_ANY; 
            FORBID_FAULT;
        }
        CONTRACTL_END;
        if (pOnDiskEmitter) 
            pOnDiskEmitter->AddRef();
        if (m_pOnDiskEmitter)
            m_pOnDiskEmitter->Release();
        m_pOnDiskEmitter = pOnDiskEmitter;
    }

#ifndef FEATURE_CORECLR
    //HRESULT EnsureCeeFileGenCreated(DWORD corhFlags = COMIMAGE_FLAGS_ILONLY, DWORD peFlags = ICEE_CREATE_FILE_PURE_IL);
    HRESULT EnsureCeeFileGenCreated(DWORD corhFlags, DWORD peFlags);
    HRESULT DestroyCeeFileGen();
#endif

	~RefClassWriter();
};

#endif	// _REFCLASSWRITER_H_
