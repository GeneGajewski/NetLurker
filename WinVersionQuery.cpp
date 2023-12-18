/*
 *   Copyright (C) 2023 Gene Gajewski WG5ENE
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
//---------------------------------------------------------------------------
#pragma hdrstop

#include "WinVersionQuery.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

namespace Util
{
    // ctor for default executing module
    WinVersionQuery::WinVersionQuery() : pBlock(NULL), lp_size(0), lp(NULL)
    {
        modulename[0] = 0;
        DWORD modulename_size =
            GetModuleFileNameW(NULL, modulename, maxmodulename);
        GetpBlock();
    }

    // ctor for a specifically named file module
    WinVersionQuery::WinVersionQuery(const wchar_t* _modulename) :
        pBlock(NULL), lp_size(0), lp(NULL)
    {
        SetModuleName(_modulename);
    }

    // copy wide modulename
    void WinVersionQuery::copy(const wchar_t* mname)
    {
        wcsncpy(modulename, mname, maxmodulename);
        modulename[maxmodulename - 1] = 0;
    }

    // dynamic re-use
    bool WinVersionQuery::SetModuleName(const wchar_t* _modulename)
    {
        if (pBlock) {
            delete pBlock;
            pBlock = NULL;
            lp_size = 0;
            lp = NULL;
        }
        copy(_modulename);
        return GetpBlock();
    }

    // get the version parameter block thingy we need for queries
    bool WinVersionQuery::GetpBlock(void)
    {
        DWORD block_size, dummy;
        block_size =
            GetFileVersionInfoSizeExW(FILE_VER_GET_NEUTRAL, modulename, &dummy);
        if (block_size) {
            pBlock = new char(block_size);
            BOOL retval = GetFileVersionInfoExW(
                FILE_VER_GET_NEUTRAL, modulename, NULL, block_size, pBlock);
            if (retval) {
                // get ptr to translation block array
                HRESULT result = VerQueryValueW(pBlock,
                    L"\\VarFileInfo\\Translation", (LPVOID*)&lp, &lp_size);
                if (SUCCEEDED(result)) {
                    SetLCP(0); // default
                    return true;
                }
            }
            if (pBlock)
                delete pBlock;
            pBlock = NULL;
        }
        return false;
    }
    // return a string value for current lang/codepge/stringname
    const wchar_t* WinVersionQuery::GetStr(const String &name)
    {
        if (pBlock) {
            UINT str_size;
            wchar_t *str, *qry = QStr(name);

            HRESULT result =
                VerQueryValueW(pBlock, qry, (LPVOID*)&str, &str_size);
            if (SUCCEEDED(result) && str_size)
                return str;
        }
        return NULL;
    }

    // select lang/codepage array entry  0-based
    bool WinVersionQuery::SetLCP(unsigned index)
    {
        int count = LCPCount();

        if (count && index < count) {
            language = lp[index].wLanguage;
            codepage = lp[index].wCodePage;
            return true;
        }
        return false;
    }

    // number of language/codepage entries
    unsigned WinVersionQuery::LCPCount()
    {
        return lp != NULL ? lp_size / sizeof(LANGANDCODEPAGE) : 0;
    }

    // return ptr to windows fixedfileinfo struct or NULL if failed.
    // the returned ptr is only valid for this obj's lifetime
    const VS_FIXEDFILEINFO* WinVersionQuery::GetFixedInfo()
    {
        if (pBlock) {
            UINT size;
            VS_FIXEDFILEINFO* vsfixed;
            HRESULT result =
                VerQueryValueW(pBlock, L"\\", (LPVOID*)&vsfixed, &size);

            if (SUCCEEDED(result) && vsfixed->dwSignature == magic_number)
                return vsfixed;
        }
        return NULL;
    }

    // dtor
    WinVersionQuery::~WinVersionQuery()
    {
        if (pBlock)
            delete pBlock;
    }

    // build string for VerQueryValue string-value query
    wchar_t* WinVersionQuery::QStr(const String &member)
    {
        query.sprintf(fmt, language, codepage) += member;
        return query.c_str();
    }

} // namespace Util

