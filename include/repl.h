#define PROMPT "\n>>> "
#define VERSION "0.1.0"
#define IS_HTTP_METHOD(token) _stricmp((token), "GET") == 0 || _stricmp((token), "POST") == 0 ||       \
                                  _stricmp((token), "PUT") == 0 || _stricmp((token), "DELETE") == 0 || \
                                  _stricmp((token), "PATCH") == 0
#define WELCOME "| Welcome to LcuRepl -- Version " VERSION "\n" \
                "| For help type: .help\n\n"

#define COMMAND ".help, .clear, .exit."

#define HELP "| The LcuRepl tool allows you to debug and leverage the LCU APIs of the League of Legends Client. \n" \
             "|\n"                                                                                                  \
             "| To use, ensure the LeagueClientUx.exe is running,"                                                  \
             "| then type the command at the prompt in the format:\n"                                               \
             "|     [METHOD] <ENDPOINT> [DATA]\n"                                                                   \
             "|\n"                                                                                                  \
             "| Available commands: " COMMAND "\n\n"                                                                \
             "| For more information, see the project README:\n"                                                    \
             "| https://github.com/ByteFlowing1337/LcuRepl/blob/main/README.md\n"

int repl(void);