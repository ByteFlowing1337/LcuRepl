#define PROMPT "\n>>> "
#define PRINT_PROMPT          \
    do                        \
    {                         \
        printf("%s", PROMPT); \
        fflush(stdout);       \
    } while (0)
#define METHOD_IN_TOKEN _stricmp(token, "GET") == 0 || _stricmp(token, "POST") == 0 ||       \
                            _stricmp(token, "PUT") == 0 || _stricmp(token, "DELETE") == 0 || \
                            _stricmp(token, "PATCH") == 0
int repl(void);