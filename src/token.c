#include <string.h>
#include <stdlib.h> // free()
#include "cmdline.h"

static char *extract_token_value(const char *cmdline, const char *key)
{
    if (!cmdline || !key)
        return NULL;

    // Locate the key in cmdline
    const char *start = strstr(cmdline, key);
    if (!start)
        return NULL;

    // Move pointer past the key name (e.g., past "--remotely-open-port=")
    start += strlen(key);

    // Determine end of token (ends at space, double-quote, or end of string)
    const char *end = start;
    while (*end != '\0' && *end != ' ' && *end != '"')
    {
        end++;
    }

    size_t val_len = end - start;
    if (val_len == 0)
        return NULL;

    // Allocate and copy the value string
    char *value = (char *)malloc(val_len + 1);
    if (!value)
        return NULL;

    strncpy(value, start, val_len);
    value[val_len] = '\0';

    return value;
}

char *get_app_port(void)
{
    char *cmdline = get_cmdline();
    if (!cmdline)
        return NULL;
    char *app_port = extract_token_value(cmdline, "--app-port=");
    free(cmdline);
    return app_port;
}

char *get_remoting_auth_token(void)
{
    char *cmdline = get_cmdline();
    if (!cmdline)
        return NULL;
    char *remoting_auth_token = extract_token_value(cmdline, "--remoting-auth-token=");
    free(cmdline);
    return remoting_auth_token;
}