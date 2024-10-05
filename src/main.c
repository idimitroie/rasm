#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>

#include "decode.h"
#include "label.h"
#include "main.h"
#include "parser.tab.h"
#include "progc.h"
#include "scanner.h"

extern void loclab_init(void);
extern void loclab_deinit(void);

int debug = 0;
int pass_number = 1; /* Start with the first pass */
static FILE *output_stream = NULL;

/* Custom printf function */
void dmp_print(const char *format, ...)
{
    if (output_stream == NULL)
    {
        output_stream = stdout; /* Default to stdout if no file is set */
    }

    va_list args;
    va_start(args, format);
    vfprintf(output_stream, format, args); /* Use vfprintf to handle the format */
    va_end(args);
}

static void print_usage()
{
    printf("Usage: program [options] [source file]\n");
    printf("Options:\n");
    printf("  -d            Enable debugging (use -dpl to specify parser and lexer debugging)\n");
    printf("  -v            Enable verbose output\n");
    printf("  -f <format>   Set output format\n");
    printf("  -o <file>     Set output file\n");
    printf("  -h, --help    Display this help message and exit\n");
}

int compile(FILE* stream)
{
    loclab_init();
    label_init();
    yyin = stream;
    pass_number = 1; /* Ensure pass_number is initialized */

    while (pass_number <= 2)
    {
      yyset_lineno(1); /* Reset line number */
      reset_pc();      /* Reset program counter */
      dmp_print("# *** RASM: pass_number = %d\n", pass_number);

      /* Call the parser for the current pass */
      if (yyparse() != 0)
      {
        fprintf(stderr, "Error during parsing in pass %d\n", pass_number);
        loclab_deinit();
        label_deinit();
        if (stream != stdin)
        {
          fclose(stream);
        }
        return -1; /* Return a non-zero value on error */
      }

      pass_number++;
      rewind(yyin); /* Reset the file pointer to the start */
    }

    loclab_deinit();
    label_deinit();

    /* Close the stream if it's not stdin */
    if (stream != stdin && fclose(stream) != 0)
    {
      perror("Failed to close stream");
      return -1; /* Return a non-zero value if closing fails */
    }

    return 0; /* Indicate success */
}

int __attribute__((weak)) main(int argc, char **argv)
{
  int c;
  const char arglst[] = "vfd:o:";
  char *output_format __attribute__((unused)) = NULL;

  dmp_print("# *** RISCV Assembler V 0.0.1\n");
  opterr = 0;
  yydebug = 0;
  while ((c = getopt (argc, argv, arglst)) != -1)
    switch (c)
    {
      case 'o':
        fprintf(stderr, "create %s\n", optarg);
        output_stream = fopen(optarg, "w");
        if (!output_stream)
        {
          perror(optarg);
          return 1;
        }
        break;
      case 'd':
        /* Check if there are additional characters after -d */
        if (optarg != NULL)
        {
          /* Enable specific debugging options */
          for (char *p = optarg; *p != '\0'; p++)
          {
            switch (*p)
            {
              case 'p':
                yydebug = 1; /* Enable debugging for the Bison parser */
                break;
              case 'l':
                yyset_debug(1); /* Enable debugging for the Flex lexer */
                break;
              default:
                fprintf(stderr, "Unknown option after -d: -%c\n", *p);
                print_usage();
                return 1;
            }
          }
        }
        break;
      case 'v':
        debug = 1; /* verbose ON */
        break;
      case 'f':
        output_format = optarg;
        break;
      case '?':
        {
          int known = 0;
          for (int i=0;arglst[i];i++)
          {
            known |= optopt == arglst[i];
          }
          if (known)
            fprintf (stderr, "Option -%c requires an argument.\n", optopt);
          else if (isprint (optopt))
            fprintf (stderr, "Unknown option `-%c'.\n", optopt);
          else
            fprintf (stderr,
                "Unknown option character `\\x%x'.\n",
                optopt);
        }
        return 1;
      default:
        print_usage(); /* Print usage information */
        return 1;
    }
  _Bool has_remaining_args = (optind < argc);

  if (has_remaining_args)
  {
    FILE *source;
    for (int i = optind; i < argc; i++)
    {
      if (strcmp(argv[i], "-") == 0)
      {
        source = stdin; /* Read from standard input */
        dmp_print("# Reading from standard input (stdin).\n");
      }
      else
      {
        /* Open the file and set source */
        source = fopen(argv[i], "r");
        if (source == NULL)
        {
          perror(argv[i]);
          return 1;
        }
        dmp_print("# Reading from file: %s\n", argv[i]);
      }
      compile(source);
    }
  }
  else
  {
    compile(stdin);
  }

}

/* deprecated */
void add_instr(instr_t *instr)
{
  char *buff = 0;
  dmp_print("%08x  # line %u\n", *(unsigned int*)(instr->data), yyget_lineno());
    if (debug)
    {
        sprint_instr(&buff, instr);
        dmp_print("%s\n", buff);
        free(buff);
    }
    free(instr->data);
    free(instr);
}

void yyerror()
{
    dmp_print("syntax error on line: %d\nI give up!\n", yyget_lineno());
    exit(1);
}

/* vim: se et sw=2 ts=2: */
