#include <stdio.h>
#include <string.h>
#include <stdlib.h> // free()
#include <windows.h>
#include "isocline.h"
#include "request.h"
#include "repl.h"

int repl(void)
{
    char *input;
    char *response;
    char *method;
    char *endpoint;
    char *data;
    char *token;

    printf(WELCOME);

    ic_set_history(NULL, -1);
    while ((input = ic_readline(NULL)) != NULL)
    {
        input[strcspn(input, "\n")] = '\0';
        if (input[0] == '\0')
        {
            goto cleanup;
        }
        if (_stricmp(input, "/intro") == 0)
        {
            printf(INTRO);
            goto cleanup;
        }
        if (_stricmp(input, "/exit") == 0 || _stricmp(input, "/quit") == 0)
        {
            free(input);
            break;
        }
        if (_stricmp(input, "/clear") == 0 || _stricmp(input, "/cls") == 0)
        {
            // ANSI sequence: \033[2J (Clear Screen) + \033[H (Move cursor to row 1, col 1)
            printf("\033[2J\033[H");
            fflush(stdout);
            goto cleanup;
        }

        // Get the first token
        token = strtok(input, " ");
        if (token == NULL)
        {
            goto cleanup;
        }
        if (IS_HTTP_METHOD(token))
        {
            method = token;
            endpoint = strtok(NULL, " ");
        }
        else
        {
            method = "GET";
            endpoint = token;
        }
        if (endpoint == NULL)
        {
            printf("Error: missing endpoint\n");
            goto cleanup;
        }
        data = strtok(NULL, "");
        if (data != NULL)
        {
            while (*data == ' ')
            {
                data++;
            }
            if (*data == '\0')
            {
                data = NULL;
            }
        }

        response = lcu_request(method, data, endpoint);
        if (response == NULL)
            printf("%s", "LCU request failed.");

        else
        {
            printf("%s", response);
            free(response);
            response = NULL;
        }
    cleanup:
        printf("\n");
        free(input);
    }
    return 0;
}