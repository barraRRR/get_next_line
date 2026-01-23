_This project was developed as part of the 42 curriculum by jbarreir._

# Get Next Line

Get Next Line receives a file descriptor and reads its content using the `read()` function from the standard C library (**libc**), storing the data in a static buffer (stash).

## Description

````
char *get_next_line(int fd);
````
**Get Next Line** is a C function that reads and returns a single line from a file descriptor.
Calling the function repeatedly allows reading a file line by line, until reaching the end of the file.

The main goal of this project is not only to implement a useful utility, but also to understand
and properly use **static variables** in C, especially in the context of preserving state between
function calls.

The function respects the following rules:
- It returns one line per call.
- The returned line includes the newline character `\n`, except if the end of file is reached and
  the file does not end with a newline.
- It works both with files and with standard input (`stdin`).


## Instructions

### Compilation

The project must be compiled using the `BUFFER_SIZE` macro, which defines the size of the buffer
used in each `read()` call.

Example:

```bash
cc -Wall -Werror -Wextra -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c
```
#### Script
To streamline the testing process, I use a simple Bash script (`test.sh`) to recompile with different `BUFFER_SIZE` values:

```bash
#!/usr/bin/env bash

BUF=$1

cc -Wall -Werror -Wextra -g -D BUFFER_SIZE=$BUF get_next_line.c get_next_line_utils.c main.c
```

#### How to use the script:
1. Give execution permissions:
	```bash
	chmod +x test.sh
	```
2. Run it with your desired buffer size:
	```Bash
	./test.sh 42
	```
The -g flag is included in the script to allow for debugging with GDB or LLDB, which is essential for tracking down memory issues.

## Algorithm & Design Decisions

### Static Buffer Choice

One of the first design decisions was choosing how to store data between calls to `get_next_line()`.

After researching how static variables work in C, I considered two main approaches:

1. **Dynamically allocated memory** that would be allocated and freed between calls.
2. **A static buffer**, whose memory persists for the entire lifetime of the program.

I decided to use a **static buffer** for the following reasons:

- The subject explicitly encourages the use of static variables.
- Avoiding repeated `malloc` / `free` cycles simplifies memory management.
- It prevents delegating the responsibility of freeing internal memory to the caller.
- It guarantees that no memory is leaked between calls, while keeping ownership clearly inside  
  `get_next_line()`.

In my opinion, a function should fully manage the memory it allocates internally. Relying on the
caller to clean internal state would be poor design in this context.

---

### Reading Strategy & Data Structures

The core of this implementation relies on two main structures: a **stash** to persist data between calls and a **linked list** to accumulate line fragments.

#### 1. Persistent State: `t_stash`
Data is read from the file descriptor using `read()` into a static buffer. The `stash` structure maintains the state for each file descriptor:

```c
typedef struct s_stash
{
    t_state         state;   			// Current status (UNINIT, PROCESSING, NEW_LINE_FOUND, EOF_READ or MALLOC_ERROR)
    ssize_t         bytes;   			// Number of bytes actually read in the last read() call
    ssize_t         i;       			// Current index/position within the buffer 'buf'
    char            buf[BUFFER_SIZE]; 	// Temporary storage for data read from disk
}   t_stash;
```
**How it works:**

* **Filling**: Each call to `read()` fills the `buf` with data up to the limit defined by `BUFFER_SIZE`.
* **Consumption**: The data in the buffer is consumed progressively, character by character, guided by the internal index `i`.
* **Exhaustion**: When the index `i` reaches the value of `bytes` (the actual amount read), it indicates the buffer is exhausted.
* **Refilling**: If no newline character has been found yet, the function triggers another `read()` call to refill the stash and continue the search.


#### 2. Line Accumulation: `t_lst`

To handle lines of unknown length without multiple realloc calls, fragments are stored in a singly linked list:

```c
typedef struct s_lst
{
    char            *buf;    			// Pointer to the specific fragment of the line
    size_t          len;     			// Length of this specific fragment
    int             nl;      			// Boolean flag (1 if this fragment contains '\n', 0 otherwise)
    struct s_lst    *next;   			// Pointer to the next fragment in the sequence
}   t_lst;
```
**How it works:**

* **Fragmentation**: As the `stash` is consumed, the data is divided into `BUFFER_SIZE` fragments and copied into new nodes of the linked list.
* **Detection**: During this process, the function checks for a newline character (`\n`). If identified, the `nl` flag in the current node is set to `1`.
* **Termination**: Once a node is marked with `nl = 1` or the end of the file (EOF) is reached, the accumulation of new nodes stops immediately.
* **Assembly**: Finally, the function calculates the total length, allocates memory for the full string, and joins all `buf` contents from the list into a single, contiguous line.

#### Implementation Highlights

This dual-structure approach, combining a static stash with a dynamic linked list, ensures the following:

- **Zero Data Loss**: The static `stash` array preserves the execution context and leftover characters between function calls for up to **1024 FDs** _(bonus)_. This is crucial for maintaining the integrity of the data stream when jumping between different files.
- **Memory Efficiency**: By utilizing a linked list (`t_lst`), we only allocate memory for line fragments as they are discovered. This avoids the need for large pre-allocated buffers or frequent, expensive `realloc` calls, making it highly scalable for lines of any length.
- **Reliability & Versatility**: The implementation is stress-tested to behave correctly under any `BUFFER_SIZE` configuration:
    - **Small buffers** (e.g., `1`): Correctly handles character-by-character reading.
    - **Standard buffers** (e.g., `42`).
    - **Large buffers** (e.g., `1,000,000+`): Efficiently manages large chunks of data.
- **Edge Case Handling**: Robust logic to manage partial reads, files ending without a trailing newline, and empty files, ensuring that `get_next_line` always returns valid data or `NULL` exactly when expected.

---

### State Flow

The internal behavior of `get_next_line()` is driven by a simple state machine defined by
the `t_state` enum:

```c
typedef enum e_state
{
    UNINIT,
    PROCESSING,
    NEW_LINE_FOUND,
    EOF_READ,
    MALLOC_ERROR
}   t_state;
```

The state flow follows this logic:

- On each call, `set_state()` restores or resets the stash depending on the previous state.
- While in **PROCESSING**, the buffer is consumed and new data is read as needed.
- Transition to **NEW_LINE_FOUND** occurs when a newline is detected during buffer parsing.
- Transition to **EOF_READ** occurs when `read()` returns 0.
- Any allocation failure immediately triggers **MALLOC_ERROR**, aborting the process safely.

This explicit state handling makes the control flow predictable, avoids undefined behavior, and simplifies error management across multiple calls.

---

### Bonus Implementation: Array of Stash Structures for Multiple File Descriptors

For the bonus part, the function is capable of handling multiple file descriptors simultaneously. Each file descriptor maintains its own independent state without mixing data between them, allowing calls such as:

```c
get_next_line(fd1);
get_next_line(fd2);
get_next_line(fd1);
```

This is achieved while respecting the constraint of using static storage and without using global variables. To manage multiple file descriptors (FDs) simultaneously, the function utilizes a **static array of structures**:

```c
static t_stash stash[1024];
```

Each element in the array represents a unique state for a specific file descriptor. Here is how it works:

- **Independent State**: When `get_next_line(fd)` is called, the function accesses `stash[fd]`. This structure contains the buffer, current index, and reading state specific to that file.
- **Persistent Memory**: Since the array is static, the data for `fd=3` remains intact even if the function is subsequently called for `fd=5`.
- **No Cross-Contamination**: Each file descriptor "remembers" its own leftover characters (the stash) from previous reads, allowing the program to jump between different files seamlessly.

#### ⚠️ BUFFER_SIZE Limitations

While the logic of the code is robust and handles any size, this specific implementation has a physical constraint due to the use of static memory allocation:

* **The Limit**: It is **not recommended** to use a `BUFFER_SIZE` greater than **8000**.
* **The Reason**: By declaring `static t_stash stash[1024]`, the program reserves persistent memory for 1024 structures simultaneously.
* **The Math**: The total memory occupied in the Data/BSS segment is approximately:
    $$1024 \times \text{BUFFER\_SIZE}$$
* **The Risk**: If the `BUFFER_SIZE` is excessively large, the program will attempt to allocate several Megabytes of static memory at once. This can lead to a **Stack Overflow** or a **Segmentation Fault** upon initialization, depending on the operating system's memory limits for the process.


> **Note for the evaluator**: For performance benchmarks or large file testing, a standard `BUFFER_SIZE` of **42** or **4096** is recommended to ensure stability.


## Resources

### Documentation & Manuals
- **Man Pages**: `man 2 read`, `man 3 malloc`, `man 3 free`.
- **GNU C Library**: [The GNU C Library Documentation](https://www.gnu.org/software/libc/manual/).
- **42 Intra**: Project subject PDF.

### Community Testers
- **Francinette**: [xicodomingues/francinette](https://github.com/xicodomingues/francinette) - Essential for checking memory leaks and edge cases.

### Video Tutorials
- **System Calls**: [Learn How to Use the read() System Call in C](https://www.youtube.com/watch?v=jYQPGkJHFr8)
- **Memory & Storage**: [Static Variables In C Programming](https://www.youtube.com/watch?v=MVeN3GDwjUg) | [C structs 🏠](https://youtu.be/oKXP1HZ8xIs)
- **File Management**: [System Programming - Opening and Reading Files](https://www.youtube.com/watch?v=scXWLP8uhDU)


## AI Usage Disclosure

In compliance with 42's evaluation standards regarding Artificial Intelligence:

- **Concept Clarification**: AI was used as a pedagogical tool to understand the behavior of static variables and the internal management of File Descriptors.
- **Translation & Localization**: AI assisted in translating technical explanations from Spanish to English to ensure professional documentation standards.
- **Debugging & Strategy**: It helped in identifying the cause of timeouts in "giant line" scenarios and brainstorming memory management within linked lists.
- **Strict Policy**: 
  - **No Copy-Paste**: No code was directly copied from AI. All implementations were written manually.
  - **Ownership**: Every line of code was understood and is capable of being replicated or modified by the author during a defense.
  - **Verification**: AI-generated suggestions were always cross-referenced with official documentation and verified using the testers mentioned in the Resources section.


## Annex: Testing Suite

To verify the correct behavior of `get_next_line`, I used a custom `main.c` and several test files covering different scenarios.

### 1. Test Main (`main.c`)
This main allows you to pass a file name as an argument and will print the file line by line, numbered, to easily spot missing newlines.

```c
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

static int	print_loop(int fd, int lines)
{
	char	*new_line;

	while (lines)
	{
		new_line = get_next_line(fd);
		if (!new_line)
			return (0);
		printf("%s", new_line);
		free(new_line);
		lines--;
	}
	return (1);
}

static int	interactive_loop(int fd)
{
	char		*line;
	char		c;
	int			ok;

	while ((line = get_next_line(fd)))
	{
        ok = 1;
        while (1)
        {
            if (read(0, &c, 1) != 1)
                return (0);
            if (c == '\n')
                break;
            ok = 0;
        }
        if (!ok)
        {
            free(line);
            return (0);
        }
		printf("%s", line);
        free(line);
	}
	return (1);
}

void	print_welcome(int option, int lines)
{
	printf(
	"\n                __                            __    .__  .__                \n"
	"   ____   _____/  |_    ____   ____ ___  ____/  |_  |  | |__| ____   ____   \n"
	"  / ___\\_/ __ \\   __\\  /    \\_/ __ \\   \\/   /\\  __\\ |  | |  |/    \\_/ __ \\  \n"
	" / /_/  >  ___/|  |   |   |  \\  ___/ >    <  |  |   |  |_|  |   |  \\  ___/  \n"
	" \\___  / \\___  >__|   |___|  /\\___  >__/\\_ \\ |__|   |____/__|___|  /\\___  > \n"
	"/_____/      \\/            \\/     \\/      \\/                     \\/     \\/  \n\n"
	);
	printf(
	"┌──────────────────────────────────────────┐\n"
	"│          WELCOME TO GNL TESTER           │\n"
	"└──────────────────────────────────────────┘\n"
	"                 jbarreir\n\n"
	);
	if (option == 1)
		printf("reading from stdin just one line...\n");
	else if (option == 2)
		printf("reading from stdin %i lines\n", lines);
	else if (option == 3)
		printf("reading from file %i lines\n", lines);
	else if (option == 4)
		printf("reading all lines from file...\n");
	else if (option == 5)
		printf("you are in interactive mode\npress enter to get next line...\n");
	printf("--------------------------------------------\n\n");
}

int main(int argc, char **argv)
{
    int fd;
    int lines;

    if (argc == 1)
    {   // NO ARG: reads one line from stdin
		print_welcome(1, 1);
        print_loop(0, 1);
    }
    else if (argc == 2)
    {	// 1 ARG: reads x num of lines from stdin
		lines = atoi(argv[1]);
		print_welcome(2, lines);
        print_loop(0, lines);
    }
	else if (argc == 3  && argv[1][0] == '-' && argv[1][1] == 'a')
    {	// 2 ARG: -a + file | reads whole file
		print_welcome(4, 0);
        fd = open(argv[2], O_RDONLY);
        if (fd < 0)
            return (1);
		char	*new_line = get_next_line(fd);
			if (!new_line)
				return (0);
		while (new_line)
		{
			printf("%s", new_line);
			free(new_line);
			new_line = get_next_line(fd);
			if (!new_line)
				break ;
		}
        close(fd);
    }
    else if (argc == 3 && argv[1][0] == '-' && argv[1][1] == 'i')
    {	// 2 ARG: -i + file | interactive mode
		print_welcome(5, 0);
        fd = open(argv[2], O_RDONLY);
        if (fd < 0)
            return (1);
		interactive_loop(fd);
        close(fd);
    }
    else if (argc == 3)
    {	// 2 ARG: reads x num of lines from file
		lines = atoi(argv[1]);
		print_welcome(3, lines);
        fd = open(argv[2], O_RDONLY);
        if (fd < 0)
            return (1);
		print_loop(fd, lines);
        close(fd);
    }
    printf("\n-----------------------------------------------");
    printf("\nThanks for evaluating get_next_line by jbarreir\n");
    printf("-----------------------------------------------\n");
    return (0);
}
```

The tester adapts its behavior based on the arguments provided:

**Standard Input (Single line)**

```Bash
./gnl_tester
```
Reads exactly one line from stdin.

**Standard Input (Multiple lines)**

```Bash
./gnl_tester 5
```
Reads the specified number of lines (e.g., 5) from stdin.

**File Reading (Specific lines)**

```Bash
./gnl_tester 10 tests/celta.txt
```
Opens the file and reads the first 10 lines.

**Full File Mode (-a)**

```Bash
./gnl_tester -a tests/sevilla.txt
```
Reads and prints the entire file until EOF.

**Interactive Mode (-i)**

```Bash
./gnl_tester -i tests/liverpool.txt
```
The program will wait for you to press Enter to fetch and display the next line, allowing you to trace the execution step by step.

### 2. Bonus Test Main (`main_bonus.c`)

This main is specifically designed to test the bonus functionality. It accepts multiple file paths as arguments, opens them all, and reads them one by one to verify that the static array manages different contexts correctly.

```c
#include "get_next_line_bonus.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int close_and_free(int *fd, int i, int error)
{
    while (i >= 0)
        close(fd[i--]);
    free(fd);
    return (error);
}

int main(int argc, char **argv)
{
    int     *fd;
    int     i;

    if (argc < 2)
        return (printf("Usage: %s <file1> <file2> ...\n", argv[0]), 1);
    fd = malloc(sizeof(int) * (argc - 1));
    if (!fd)
        return (1);
    i = 0;
    while (i < (argc - 1))
    {
        fd[i] = open(argv[i + 1], O_RDONLY);
        if (fd[i] < 0)
            return (close_and_free(fd, i, 1));
        i++;
    }
    printf(
    "\n┌──────────────────────────────────────────┐\n"
    "│        WELCOME TO GNL BONUS TESTER       │\n"
    "└──────────────────────────────────────────┘\n"
    "                 jbarreir\n"
    );
    i = 0;
    while (i < (argc - 1))
    {
        printf("\n\nreading from: %s\n\n", argv[i + 1]);
        char *new_line;
        while ((new_line = get_next_line(fd[i])) != NULL)
        {
            printf("%s", new_line);
            free(new_line);
        }
        i++;
    }
    printf("\n-----------------------------------------------");
    printf("\nThanks for evaluating get_next_line by jbarreir\n");
    printf("-----------------------------------------------\n");
    return (close_and_free(fd, i - 1, 0));
}
```

### 3. Quick Start Test (Multiple FDs)

To test the mandatory part or the bonus (interleaving reads from different files), you can create three test files with these iconic football anthems by running this single command:

```bash
mkdir -p tests && \
cat << 'EOF' > tests/celta.txt
Oliveira dos cen anos
Cría raíces de prata
Tomar amores non custa
Olvidalos si que mata
Olvidalos si que mata
Eu non os quero esquecer
Na ledicia son celeste
Celtista no padecer

Un escudo no meu peito
Sempre Celta!
Un escudo no meu peito
Sempre Celta!

Non sei se podo vivir sen ti
Pero iso éme igual
Non o quero probar
Cada vez que mires atrás
Sempre hei estar aquí

Sempre Celta!
EOF
cat << 'EOF' > tests/sevilla.txt
Familia roja y blanca del Sánchez Pizjuán
Mi corazón que late gritando ¡Sevilla!
Llevándolo en volandas por siempre a ganar
Y es por eso que hoy vengo a verte
Sevillista seré hasta la muerte
La Giralda presume orgullosa

De ver al Sevilla
en el Sánchez Pizjuán
Y Sevilla, Sevilla, Sevilla
Aquí estamos contigo Sevilla
Compartiendo la gloria en tu escudo
Orgullo del fútbol de nuestra ciudad
Y es por eso que hoy vengo a verte
Sevillista seré hasta la muerte
La Giralda presume orgullosa

De ver al Sevilla en el Sánchez Pizjuán
Y Sevilla, Sevilla, Sevilla
Aquí estamos contigo Sevilla
Compartiendo la gloria en tu escudo
Orgullo del fútbol de nuestra ciudad
EOF
cat << 'EOF' > tests/liverpool.txt
When you walk through a storm
Hold your head up high
And don't be afraid of the dark
At the end of the storm
There's a golden sky
And the sweet silver song of a lark

Walk on
Through the wind
Walk on
Through the rain
Though your dreams be tossed and blown

Walk on
Walk on
With hope
In your heart
And you'll never walk alone
You'll never walk alone

Walk on
Walk on
With hope
In your heart
And you'll never walk alone
You'll never walk alone
EOF
```
