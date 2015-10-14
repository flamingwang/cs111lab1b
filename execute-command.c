// UCLA CS 111 Lab 1 command execution

#include "command.h"
#include "command-internals.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
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
    if (c->input != NULL)
      freopen(c->input, "r", stdin);
    if (c->output != NULL)
      freopen(c->output, "w", stdout);
    execvp(c->u.word[0], c->u.word);
    fclose(stdin);
    fclose(stdout);
  }
  else {
    int return_pid = waitpid(pid, &child_status, 0);
    c->status = WEXITSTATUS(child_status);
  }
}

void execute_pipe (command_t c, int time_travel) {
  int child_status;
  int first_pid, second_pid, return_pid;
  int mypipe[2];
  pipe(mypipe);
  first_pid = fork();
  if (first_pid == 0) {
    close(mypipe[0]);
    dup2(mypipe[1], 1);
    close(mypipe[1]);
    execute_command(c->u.command[0], time_travel);
  }
  else {
    second_pid = fork();
    if (second_pid == 0) {
      close(mypipe[1]);
      dup2(mypipe[0], 0);
      close(mypipe[0]);
      execute_command(c->u.command[1], time_travel);
    }
    else {
      close(mypipe[0]);
      close(mypipe[1]);
      return_pid = waitpid(-1, &child_status, 0);
      /*if (return_pid == first_pid)
	waitpid(second_pid, &child_status, 0);
      else
      waitpid(first_pid, &child_status, 0);*/
      return_pid = waitpid(first_pid, &child_status, 0);
      waitpid(second_pid, &child_status, 0);
      c->status = WEXITSTATUS(child_status);
    }
  }
}

void
execute_command (command_t c, int time_travel)
{
  /* FIXME: Replace this with your implementation.  You may need to
     add auxiliary functions and otherwise modify the source code.
     You can also use external functions defined in the GNU C Library.  */
  switch(c->type) {
  case PIPE_COMMAND:
    execute_pipe(c, time_travel);
    break;
  case SIMPLE_COMMAND:
    execute(c);
    break;
  case AND_COMMAND:
  case SEQUENCE_COMMAND:
  case OR_COMMAND:
    execute_command(c->u.command[0], time_travel);
    execute_command(c->u.command[1], time_travel);
    break;
  case SUBSHELL_COMMAND:
    execute_command(c->u.subshell_command, time_travel);    
    break;
  }
  //error (1, 0, "command execution not yet implemented");
}
