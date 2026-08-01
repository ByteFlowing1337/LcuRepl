# LcuRepl

A **lightweight** C REPL (Read–Eval–Print Loop) for interacting with the LCU APIs.

## Usage

Launch `LcuRepl.exe`, then type an LCU API endpoint at the prompt.

For example:

```text
>>> /riotclient/app-name
"LeagueClient"
>>>
```

## Build

### Prerequisites

- Windows OS
- [MSYS2](https://www.msys2.org/), with the following packages installed:
  - `mingw-w64-x86_64-gcc`
  - `mingw32-make`

### Compilation

Open PowerShell or an MSYS2 MinGW terminal in the project directory and run:

```powershell
mingw32-make.exe
```

To remove build artifacts:

```powershell
mingw32-make.exe clean
```
