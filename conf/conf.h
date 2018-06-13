/* Contains Weaver configuration rules */

// BASIC CONFIGURATION

// The program name as a legible and human-friendly string:
//#define W_PROGRAM_NAME "Program Name"

// The ammount of details printed by weaver functions:
// 0: No warning is printed.
// 1: Info about non-fatal errors will be printed
// 2: General warnings will be printed
// Before installing and distributing this game, set the macro to 0
#define W_DEBUG_LEVEL 0

// This macro determines if we are building a native program (W_ELF) or
// a web program to be run in a browser (W_WEB). In the first case,
// running "make" creates an executable with the project name. In the
// second, creates a "web" directory with a web application
#define W_TARGET W_WEB

// This macro determines if we are writting a C program (W_C) or a C++
// program (W_CPP):
#define W_SOURCE W_C

// The maximum ammount of memory for the game
#define W_MAX_MEMORY 10000000
#define W_INTERNAL_MEMORY 200000
// When compiling for the web, we usually need aditional memory. So we
// sum the memory above with the value below:
#define W_WEB_MEMORY 100971520

// If you wont use multiple threads, you can comment this:
//#define W_MULTITHREAD

// If W_MULTITHREAD is defined above, this sets the size of the pool
// of threads used to read and parse files. If the value is zero or
// less, we don't use a pool of threads, but create a new thread each
// time we read and parse a file with audio, font or texture
#define W_THREAD_POOL 0

/* Various Limits */
#define W_MAX_SUBLOOP 2 // Max number of subloops
#define W_MAX_SCHEDULING       7// Max periodic functions in a loop
#define W_MAX_INTERFACES       290 // Max number of interface elements
#define W_MAX_MUSIC            2
