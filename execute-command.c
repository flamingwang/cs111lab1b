// UCLA CS 111 Lab 1 command execution

#include "command.h"
#include "command-internals.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <error.h>

/* FIXME: You may need to add #include directives, macro definitions,
   static function definitions, etc.  */

int
command_status (command_t c)
{
  return c->status;
}

//Execute simple command
void execute (command_t c) {
  int child_status;
  int pid = fork();
  if (pid == 0) {
    execvp(c->u.word[0], c->u.word);
  }
  else {
    int return_pid = waitpid(pid, &child_status, 0);
    c->status = WEXITSTATUS(child_status);
  }
}

void execute_pipe (command_t c) {
  int first_pid, second_pid, return_pid;
  int mypipe[2];
}

void
execute_command (command_t c, int time_travel)
{
  /* FIXME: Replace this with your implementation.  You may need to
     add auxiliary functions and otherwise modify the source code.
     You can also use external functions defined in the GNU C Library.  */
  switch(c->type) {
  case PIPE_COMMAND:
    execute_pipe(c);
    break;
  case SIMPLE_COMMAND:
    execute(c);
    break;
  case AND_COMMAND:
    break;
  case SEQUENCE_COMMAND:
    break;
  case OR_COMMAND:
  case SUBSHELL_COMMAND:
    break;
  }
  //error (1, 0, "command execution not yet implemented");
}
