// UCLA CS 111 Lab 1 main program

#include <errno.h>
#include <error.h>
#include <getopt.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <time.h>
#include <unistd.h>

#include "command.h"
#include "command-internals.h"

static char const *program_name;
static char const *script_name;

extern int numProc;

static void
usage (void)
{
  error (1, 0, "usage: %s [-pt] SCRIPT-FILE", program_name);
}

static int
get_next_byte (void *stream)
{
  return getc (stream);
}

int
main (int argc, char **argv)
{
  int opt;
  int command_number = 1;
  int print_tree = 0;
  int time_travel = 0;
  program_name = argv[0];

  for (;;)
    switch (getopt (argc, argv, "pt"))
      {
      case 'p': print_tree = 1; break;
      case 't': time_travel = 1; break;
      default: usage (); break;
      case -1: goto options_exhausted;
      }
 options_exhausted:;

  // There must be exactly one file argument.
  if (optind != argc - 1)
    usage ();

  script_name = argv[optind];
  FILE *script_stream = fopen (script_name, "r");
  if (! script_stream)
    error (1, errno, "%s: cannot open", script_name);
  command_stream_t command_stream =
    make_command_stream (get_next_byte, script_stream);

  command_t last_command = NULL;
  command_t command;
  struct rusage usage;// = malloc(sizeof(struct rusage));
  //long start, end;
  //getrusage(RUSAGE_CHILDREN, &usage);
  //start = usage.ru_utime;
  clock_t begin, end;
  double time_spent;
  begin = clock();
  struct timeval tv1, tv2;
  //sleep(1);
  while ((command = read_command_stream (command_stream)))
    {
      if (print_tree)
	{
	  printf ("# %d\n", command_number++);
	  print_command (command);
	}
      else
	{
	  numProc = 0;
	  gettimeofday(&tv1, NULL);
	  last_command = command;
	  execute_command (command, time_travel);
	  //	  if (command->isTime) {
	  if (command->isTime) {
	    gettimeofday(&tv2, NULL);
 	    printf("number of subprocesses: %d\n", numProc);
    	    getrusage(RUSAGE_CHILDREN, &usage);
	    printf("memory usage for children: %ld\n", usage.ru_maxrss);
	    /*printf("integral shared memory usage: %ld\n", usage.ru_ixrss);
	    printf("data memory usage: %ld\n", usage.ru_idrss);
	    printf("stack memory usage: %ld\n", usage.ru_isrss);*/
	    
    	    getrusage(RUSAGE_SELF, &usage);
	    printf("memory usage for self: %ld\n", usage.ru_maxrss);
	    /*printf("maximum resident set size memory usage: %ld\n", usage.ru_maxrss);
	    printf("integral shared memory usage: %ld\n", usage.ru_ixrss);
	    printf("data memory usage: %ld\n", usage.ru_idrss);
	    printf("stack memory usage: %ld\n", usage.ru_isrss);*/

	    //getrusage(RUSAGE_CHILDREN, &usage);
	    //printf("time spent: %ld\n", usage.ru_utime.tv_usec);
	    printf ("Total time = %f seconds\n\n",
		    (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
		    (double) (tv2.tv_sec - tv1.tv_sec));
 
	  }
	}
    }
  if (!print_tree) {
    end = clock();
    /*time_spent = (double) (end - begin) / CLOCKS_PER_SEC;
    printf("number of subprocesses: %d\n", numProc);
    printf("time spent: %f\n", time_spent);
    printf("time beginning: %ld\n", begin);
    printf("time end: %ld\n", end);
    
    getrusage(RUSAGE_CHILDREN, &usage);
    
    //end = usage.ru_utime;
    //long time = end - start;
    //printf("CPU time of children: %ld\n", 42); 
    printf("memory usage: %ld\n", usage.ru_maxrss);
    getrusage(RUSAGE_CHILDREN, &usage);
    printf("time spent: %ld\n", usage.ru_utime.tv_usec);*/
  }


  return print_tree || !last_command ? 0 : command_status (last_command);
}
