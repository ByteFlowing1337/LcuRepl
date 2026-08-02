#include <stdio.h>
#include <string.h>
#include <stdlib.h> // free()
#include <windows.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "request.h"
#include "repl.h"

static BOOL WINAPI console_handler(DWORD signal)
{
    if (signal == CTRL_C_EVENT)
    {
        printf("\n");           // Print a new line
        rl_on_new_line();       // Inform readline we moved down
        rl_replace_line("", 0); // Clear currently typed buffer
        rl_redisplay();         // Refresh prompt
        return TRUE;            // Tell Windows we handled Ctrl+C (do NOT exit process!)
    }
    return FALSE;
}

int repl(void)
{
    char *input;
    char *response;
    char *method;
    char *endpoint;
    char *data;
    char *token;
    if (!SetConsoleCtrlHandler(console_handler, TRUE))
    {
        fprintf(stderr, "Error: Could not set control handler\n");
        return 1;
    }
    rl_catch_signals = 0;
    while ((input = readline(PROMPT)) != NULL)
    {
        input[strcspn(input, "\n")] = '\0';
        if (input[0] == '\0')
        {
            free(input);
            continue;
        }
        token = strtok(input, " ");
        if (token == NULL)
        {
            free(input);
            continue;
        }
        if (METHOD_IN_TOKEN)
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
            free(input);
            continue;
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
        free(input);
    }
    return 0;
}