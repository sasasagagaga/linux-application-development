#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

enum
{
    OK,
    WRONG_USAGE,
    WRONG_FILE,
    NO_PERM,
    OTHER_PROBLEMS,

};

#define fperror(format, ...) fprintf(stderr, format ": %s\n", ##__VA_ARGS__, strerror(errno))

int _check_error()  // char *message)
{
    int exit_code;
    switch (errno) {
    case 0:
        exit_code = OK;
        break;
    case EISDIR:
    case ENAMETOOLONG:
    case ENOENT:
    case ELOOP:
        exit_code = WRONG_FILE;
        break;
    case EACCES:
    case EROFS:
    case ETXTBSY:
        exit_code = NO_PERM;
        break;
    default:
        exit_code = OTHER_PROBLEMS;
        break;
    }

    return exit_code;

//    if (errno) {
//        perror(message);
//        exit(exit_code);
//    }
}

#define check_error(...) do { \
    int exit_code = _check_error(); \
    if (exit_code) { \
        fperror(__VA_ARGS__); \
        exit(exit_code); \
    } \
} while(0)

int main(int argc, char **argv)
{
    if (argc != 3) {
        puts("Usage: move INFILE OUTFILE");
        puts("Move content of INFILE file to OUTFILE file.");
        return WRONG_USAGE;
    }

    char *in_file = argv[1];
    char *out_file = argv[2];

    FILE *in_fp = fopen(in_file, "r");
    check_error("Can't open input file '%s'", in_file);

    FILE *out_fp = fopen(out_file, "w");
    check_error("Can't open output file '%s'", out_file);

    int ch;
    while ((ch = getc(in_fp)) != EOF) {
        check_error("Can't read data from input file '%s'", in_file);
        putc(ch, out_fp);
        check_error("Can't write data to output file '%s'", out_file);
    }

    fclose(in_fp);
    check_error("Can't close input file '%s'", in_file);
    fclose(out_fp);
    check_error("Can't close output file '%s'", out_file);

    remove(in_file);
    check_error("Can't remove input file '%s'", in_file);

    return OK;
}
