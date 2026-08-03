#include <windows.h>
#include <winhttp.h>
#include <stdio.h>  // snprintf()
#include <stdint.h> // uint32_t
#include "token.h"

char *lcu_request(char *method, char *data, char *endpoint)
{
    char *port = get_app_port();
    char *token = get_remoting_auth_token();
    void *request_data;
    DWORD length;
    char *response = NULL;
    LPCWSTR wmethod;

    if (!port || !token)
        return NULL;

    // Convert parameters to Wide strings using %hs for narrow-string safety
    wchar_t wport[16], wendpoint[512], wtoken[256];
    swprintf(wport, sizeof(wport) / sizeof(*wport), L"%hs", port);
    swprintf(wendpoint, sizeof(wendpoint) / sizeof(*wendpoint), L"%hs", endpoint);
    swprintf(wtoken, sizeof(wtoken) / sizeof(*wtoken), L"%hs", token);

    wchar_t auth_header[40];
    swprintf(auth_header, sizeof(auth_header) / sizeof(*auth_header),
             L"Content-Type: application/json\r\n");

    HINTERNET hSession = WinHttpOpen(L"LCU-Repl/1.0",
                                     WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                                     WINHTTP_NO_PROXY_NAME,
                                     WINHTTP_NO_PROXY_BYPASS, 0);
    if (!hSession)
        goto cleanup_tokens;

    HINTERNET hConnect = WinHttpConnect(hSession, L"127.0.0.1", (INTERNET_PORT)_wtoi(wport), 0);
    if (!hConnect)
        goto cleanup_session;

    if (_stricmp(method, "GET") == 0)
        wmethod = L"GET";
    else if (_stricmp(method, "POST") == 0)
        wmethod = L"POST";
    else if (_stricmp(method, "PUT") == 0)
        wmethod = L"PUT";
    else if (_stricmp(method, "DELETE") == 0)
        wmethod = L"DELETE";
    else if (_stricmp(method, "PATCH") == 0)
        wmethod = L"PATCH";
    else
        wmethod = L"GET";
    HINTERNET hRequest = WinHttpOpenRequest(hConnect, wmethod, wendpoint,
                                            NULL, WINHTTP_NO_REFERER,
                                            WINHTTP_DEFAULT_ACCEPT_TYPES,
                                            WINHTTP_FLAG_SECURE);
    if (!hRequest)
        goto cleanup_connect;

    // Bypass LCU Self-Signed SSL Certification Checks
    DWORD dwFlags = SECURITY_FLAG_IGNORE_UNKNOWN_CA |
                    SECURITY_FLAG_IGNORE_CERT_WRONG_USAGE |
                    SECURITY_FLAG_IGNORE_CERT_CN_INVALID |
                    SECURITY_FLAG_IGNORE_CERT_DATE_INVALID;
    WinHttpSetOption(hRequest, WINHTTP_OPTION_SECURITY_FLAGS, &dwFlags, sizeof(dwFlags));
    WinHttpSetCredentials(hRequest, WINHTTP_AUTH_TARGET_SERVER, WINHTTP_AUTH_SCHEME_BASIC,
                          L"riot", wtoken, NULL);

    if (data == NULL)
    {
        request_data = WINHTTP_NO_REQUEST_DATA;
        length = 0;
    }
    else
    {
        request_data = data;
        length = strlen(request_data);
    }
    // Send Headers & Execute Request
    BOOL bResults = WinHttpSendRequest(hRequest,
                                       auth_header, -1L,
                                       request_data, length, length, 0);

    if (bResults)
        bResults = WinHttpReceiveResponse(hRequest, NULL);

    // Accumulate Full Response Body across all chunks
    size_t total_bytes = 0;

    if (bResults)
    {
        DWORD dwSize = 0;
        DWORD dwDownloaded = 0;

        do
        {
            dwSize = 0;
            if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
                break;
            if (dwSize == 0)
                break;

            char *new_buf = realloc(response, total_bytes + dwSize + 1);
            if (!new_buf)
            {
                free(response);
                response = NULL;
                break;
            }
            response = new_buf;

            if (WinHttpReadData(hRequest, response + total_bytes, dwSize, &dwDownloaded))
            {
                total_bytes += dwDownloaded;
            }
        } while (dwSize > 0);

        if (response)
        {
            response[total_bytes] = '\0'; // Null-terminate string
        }
    }

    WinHttpCloseHandle(hRequest);
cleanup_connect:
    WinHttpCloseHandle(hConnect);
cleanup_session:
    WinHttpCloseHandle(hSession);
cleanup_tokens:
    free(port);
    free(token);

    return response;
}