#define PROMPT "\n>>> "
#define VERSION "0.1.0"
#define PRINT_PROMPT          \
    do                        \
    {                         \
        printf("%s", PROMPT); \
        fflush(stdout);       \
    } while (0)
#define IS_HTTP_METHOD(token) _stricmp((token), "GET") == 0 || _stricmp((token), "POST") == 0 ||       \
                                  _stricmp((token), "PUT") == 0 || _stricmp((token), "DELETE") == 0 || \
                                  _stricmp((token), "PATCH") == 0
#define WELCOME "| Welcome to LcuRepl -- Version " VERSION "\n" \
                "| For an introduction type: /intro\n\n"

#define INTRO "| The LcuRepl tool allows you to debug and leverage the LCU APIs of the League of Legends Client. \n" \
              "|\n"                                                                                                  \
              "| To use, type the command at the prompt in the format:\n"                                            \
              "|     [METHOD] <ENDPOINT> [DATA]\n"                                                                   \
              "|\n"                                                                                                  \
              "| For more information, see the project README:\n"                                                    \
              "| https://github.com/ByteFlowing1337/LcuRepl/blob/main/README.md\n"

int repl(void);