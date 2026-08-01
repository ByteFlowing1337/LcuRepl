#include <stdio.h>
#include <string.h>
#include <stdlib.h> // free()
#include "request.h"
#define PROMPT ">>> "

int main(void)
{
    char input[512];
    char *response;
    printf("%s", PROMPT);
    fflush(stdout);
    while (fgets(input, sizeof(input), stdin) != NULL)
    {
        input[strcspn(input, "\n")] = '\0';
        if (input[0] == '\0')
        {
            printf("%s", PROMPT);
            fflush(stdout);
            continue;
        }
        response = lcu_get_request(input);
        if (response != NULL)
        {
            printf("%s", response);
            free(response);
            response = NULL;
        }
        else
        {
            printf("%s", "LCU request failed.");
        }
        printf("%s", "\n");
        printf("%s", PROMPT);
    }
    return 0;
}