/*---------------------------------------------------------*\
| wmi.cpp                                                   |
|                                                           |
|   WMI interface functionality                             |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "wmi.h"

IWbemLocator* Wmi::pLoc = nullptr;
IWbemServices* Wmi::pSvc = nullptr;
HRESULT WmiInit = Wmi::init();

// Taken from https://stackoverflow.com/questions/215963/
// Convert a wide Unicode string to an UTF8 string
std::string utf8_encode(const std::wstring& wstr)
{
    if (wstr.empty()) return std::string();

    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int) wstr.size(), nullptr, 0, nullptr, nullptr);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int) wstr.size(), &strTo[0], size_needed, nullptr, nullptr);

    return strTo;
}

// Convert an UTF8 string to a wide Unicode String
std::wstring utf8_decode(const std::string& str)
{
    if (str.empty())
    {
        return std::wstring();
    }

    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int) str.size(), nullptr, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int) str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

bool isMatch(const std::string& value, const std::regex& re)
{
    return std::regex_match(value, re);
}

Wmi::Wmi()
{

};

Wmi::~Wmi()
{

}

HRESULT Wmi::init()
{
    HRESULT hres;

    if(pLoc != nullptr && pSvc != nullptr)
    {
        return S_OK;
    }

    // Initialize COM. ------------------------------------------
    hres = CoInitializeEx(0, COINIT_APARTMENTTHREADED);
    if (FAILED(hres))
    {
        return hres;
    }

    // Set general COM security levels --------------------------
    hres = CoInitializeSecurity(
        nullptr,
        -1,                          // COM authentication
        nullptr,                     // Authentication services
        nullptr,                     // Reserved
        RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication
        RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation
        nullptr,                     // Authentication info
        EOAC_NONE,                   // Additional capabilities
        nullptr                      // Reserved
    );

    if (FAILED(hres))
    {
        CoUninitialize();
        return hres;
    }

    // Obtain the initial locator to WMI -------------------------
    hres = CoCreateInstance(
        CLSID_WbemLocator,
        nullptr,
        CLSCTX_INPROC_SERVER,
        IID_IWbemLocator, (LPVOID*) &pLoc
    );

    if (FAILED(hres))
    {
        CoUninitialize();
        return hres;
    }

    hres = pLoc->ConnectServer(
        _bstr_t(L"ROOT\\CIMV2"), // Object path of WMI namespace
        nullptr,                    // User name. NULL = current user
        nullptr,                    // User password. NULL = current
        nullptr,                 // Locale. NULL indicates current
        0,                    // Security flags.
        nullptr,                 // Authority (for example, Kerberos)
        nullptr,                 // Context object
        &pSvc                    // pointer to IWbemServices proxy
    );

    if (FAILED(hres))
    {
        pLoc->Release();
        CoUninitialize();
        return hres;
    }

    // Set security levels on the proxy -------------------------
    hres = CoSetProxyBlanket(
        pSvc,                        // Indicates the proxy to set
        RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
        RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
        nullptr,                        // Server principal name
        RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx
        RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
        nullptr,                        // client identity
        EOAC_NONE                    // proxy capabilities
    );

    if (FAILED(hres))
    {
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return hres;
    }

    // Initialised WMI successfully
    return S_OK;
}

HRESULT Wmi::query(std::string queryStr, std::vector<QueryObj>& queryVectorOut, const AdditionalFilters* filters)
{
    if (pSvc == nullptr)
    {
        return E_FAIL;
    }

    HRESULT hres;
    int nIdx = 0;
    IEnumWbemClassObject* pEnumerator = nullptr;

    // Initialize COM. ------------------------------------------
    hres = CoInitializeEx(0, COINIT_APARTMENTTHREADED);
    if (FAILED(hres))
    {
        return hres;
    }

    pSvc->Release();
    // Reconnect to server before each query as we were seeing disconnected failures
    hres = pLoc->ConnectServer(
        _bstr_t(L"ROOT\\CIMV2"), // Object path of WMI namespace
        nullptr,                    // User name. NULL = current user
        nullptr,                    // User password. NULL = current
        nullptr,                 // Locale. NULL indicates current
        0,                    // Security flags.
        nullptr,                 // Authority (for example, Kerberos)
        nullptr,                 // Context object
        &pSvc                    // pointer to IWbemServices proxy
    );

    // Make the WMI query
    hres = pSvc->ExecQuery(
        bstr_t("WQL"),
        bstr_t(queryStr.c_str()),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        nullptr,
        &pEnumerator
    );

    if (FAILED(hres))
    {
        return hres;
    }

    IWbemClassObject* pclsObj = nullptr;
    ULONG uReturn = 0;

    while (pEnumerator)
    {
        hres = pEnumerator->Next(WMI_WAIT_TIME, 1, &pclsObj, &uReturn);

        if (0==uReturn)
        {
        break;
        }

        VARIANT vtProp;
        if (filters)
        {
            for (auto filter: *filters)
            {
                hres = pclsObj->Get(utf8_decode(filter.first).c_str(), 0, &vtProp, nullptr, nullptr);

                if (FAILED(hres))
                {
                    continue;
                }

                auto val = utf8_encode(vtProp.bstrVal);

                if (!std::regex_match(val, filter.second))
                {
                    goto _NextElement;
                }
            }
        }

        SAFEARRAY* sfArray;
        LONG lstart, lend;

        //Get Wmi objects names
        hres = pclsObj->GetNames(0, WBEM_FLAG_ALWAYS, nullptr, &sfArray);

        if (FAILED(hres))
        {
            continue;
        }

        // Find safe array boundaries
        SafeArrayGetLBound(sfArray, 1, &lstart);
        SafeArrayGetUBound(sfArray, 1, &lend);

        BSTR* pbstr;
        hres = SafeArrayAccessData(sfArray, (void HUGEP**) &pbstr);
        nIdx = 0;

        if (FAILED(hres))
        {
            continue;
        }

        {
            CIMTYPE pType;
            QueryObj item;

            for (nIdx = lstart; nIdx < lend; nIdx++)
            {
                hres = pclsObj->Get(pbstr[nIdx], 0, &vtProp, &pType, 0);

                if (FAILED(hres))
                {
                    continue;
                }

                if (vtProp.vt == VT_NULL)
                {
                    continue;
                }

                if ((pType == CIM_STRING || pType == CIM_REFERENCE) && pType != CIM_EMPTY && pType != CIM_ILLEGAL)
                {
                    item.emplace(utf8_encode(pbstr[nIdx]), utf8_encode(vtProp.bstrVal));
                }

                VariantClear(&vtProp);
            }

            hres = pclsObj->Get(L"Dependent", 0, &vtProp, &pType, nullptr);

            if (pType != CIM_EMPTY && pType != CIM_ILLEGAL && SUCCEEDED(hres))
            {
                item.emplace(utf8_encode(pbstr[nIdx]), utf8_encode(vtProp.bstrVal));
            }

            // Push item to vector
            queryVectorOut.emplace_back(item);

            // Empty sfArray
            SafeArrayUnaccessData(sfArray);

            SafeArrayDestroy(sfArray);  // Delete sfArray
            sfArray = nullptr;          // Avoid dangling pointers
        }

        _NextElement:
        VariantClear(&vtProp);      // Clear vtProp
        pclsObj->Release();         // Release pclsObj
    }

    pEnumerator->Release();
    pEnumerator = nullptr;

    return S_OK;
}
