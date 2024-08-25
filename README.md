# Simple Shell Project

This project is a simple shell written in C as part of a course on Operating Systems. The shell supports basic functionalities such as `cd`, `exit`, background processes, input-output redirections, pipes, auto-completion of file names, and a manual `man` page.

## Features

- **Basic Commands:** Supports `cd` and `exit` commands with similar behavior to Bash.
- **Background Processes:** Execute commands in the background using `&`.
- **Input-Output Redirections:** Handle various forms of redirection including `>`, `>>`, `2>`, `2>>`, `>&`, and `<`.
- **Pipes:** Allows the chaining of commands using pipes (`|` and `||`).
- **Auto-Completion:** Provides auto-completion for file names by pressing `TAB`.
- **Custom Manual:** A `man` page is available for the custom commands.

## File Structure

- **projet_co-main.c:** The main file containing the core loop and command handling.
- **cn-decouper.c:** Contains a function to split input lines into tokens.
- **and_monexit.c:** Handles the custom `exit` command.
- **moncd.c:** Implements the `cd` command.
- **pipe.c:** Manages pipes and the execution of piped commands.
- **redir_in_pipe.c:** Handles redirections within piped commands.
- **redirection.c:** Manages all forms of input-output redirections.
- **man.c:** Provides a custom manual (`man`) for the shell.

## Compilation

To compile the project, use the provided `Makefile`:

```bash
make
```

## Usage

Run the shell with:

./projet

Use `monexit` to exit the shell, and `moncd` to change directories.

### Example Commands

```bash
moncd /path/to/directory
echo "Hello, World!" > output.txt
ls | wc -l &
```

## License

This project was created as part of an Operating Systems course and is licensed for educational use.

## Author

Avrile Floro, 2024.
