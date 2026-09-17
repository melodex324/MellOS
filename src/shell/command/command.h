#ifndef COMMAND_H
#define COMMAND_H

typedef void (*command_func)(int argc, char **argv);

typedef struct
{
    const char *name;
    const char *description;
    command_func function;
} _command;

typedef struct      // only to make a "list" of functions the command can execute | not the same as a normal argument 
{
    const char *name;
    void (*function)(int argc, char **argv);
} _subcommand;

// void test(int argc, char **argv);

void help(int argc, char **argv);
void clear(int argc, char **argv);
void debug(int argc, char **argv);

extern _command command[];
extern size_t num_commands;

#endif