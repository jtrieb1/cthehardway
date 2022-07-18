#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "dbg.h"

#define MAX_DATA 100

int read_string(char **out_string, int max_buffer)
{
    *out_string = calloc(1, max_buffer + 1);
    check_mem(*out_string);

    char *result = fgets(*out_string, max_buffer, stdin);
    check(result != NULL, "Input error.");

    return 0;

error:
    if (*out_string) free(*out_string);
    *out_string = NULL;
    return -1;
}

int read_int(int *out_int)
{
    char *input = NULL;
    int rc = read_string(&input, MAX_DATA);
    check(rc == 0, "Failed to read number.");

    *out_int = atoi(input);

    free(input);
    return 0;

error:
    if (input) free(input);
    return -1;
}

// Personal preference, "east const" style is easier to read.
int read_scan(char const *fmt, ...)
{
    int i = 0;
    int rc = 0;
    int *out_int = NULL;
    char *out_char = NULL;
    char **out_string = NULL;
    int max_buffer = 0;

    va_list argp;
    va_start(argp, fmt);

    for (i = 0; fmt[i] != '\0'; i++) {
        if (fmt[i] == '%') {
            i++;
            switch (fmt[i]) {
                case '\0':
                    sentinel("Invalid format, ended with %%.");
                    break;
                case 'd':
                    out_int = va_arg(argp, int *);
                    rc = read_int(out_int);
                    check(rc == 0, "Failed to read int.");
                    break;
                case 'c':
                    out_char = va_arg(argp, char *);
                    *out_char = fgetc(stdin);
                    break;
                case 's':
                    max_buffer = va_arg(argp, int);
                    out_string = va_arg(argp, char **);
                    rc = read_string(out_string, max_buffer);
                    check(rc == 0, "Failed to read string.");
                    break;
                default:
                    sentinel("Invalid format.");
            }
        } else {
            fgetc(stdin);
        }

        check(!feof(stdin) && !ferror(stdin), "Input error.");
    }

    va_end(argp);
    return 0;

error:
    va_end(argp);
    return -1;
}

char *num_to_string(unsigned int num) {
    static char *repr = "0123456789";
    static char buffer[50];
    char *ptr;

    ptr = &buffer[49];
    *ptr = '\0';

    do {
        *--ptr = repr[num % 10];
        num /= 10;
    } while (num != 0);

    return ptr;
}

int write_fmt(char const *fmt, ...)
{
    int i = 0;
    int out_numeric;
    char *out_s;

    va_list argp;
    va_start(argp, fmt);

    for (; fmt[i] != '\0'; i++) {
        if (fmt[i] == '%') {
            i++;
            switch (fmt[i]) {
                case '\0':
                    sentinel("Invalid format, ended with %%.");
                    break;
                case 'd':
                    out_numeric = va_arg(argp, int);
                    if (out_numeric < 0) {
                        out_numeric *= -1;
                        putchar('-');
                    }
                    fputs(num_to_string(out_numeric), stdout);
                    break;
                case 'c':
                    out_numeric = va_arg(argp, int);
                    putchar(out_numeric);
                    break;
                case 's':
                    out_s = va_arg(argp, char *);
                    out_s[strcspn(out_s, "\r\n")] = 0; // Remove trailing newline from fgets.
                    fputs(out_s, stdout);
                    break;
                default:
                    sentinel("Invalid format.");
            }
        } else {
            putchar(fmt[i]);
        }
        
        check(!ferror(stdout), "Output error.");
    }

    va_end(argp);
    return 0;

error:
    va_end(argp);
    return -1;
}

int main(int argc, char *argv[])
{
    char *first_name = NULL;
    char initial = ' ';
    char *last_name = NULL;
    int age = 0;

    write_fmt("What's your first name? ");
    int rc = read_scan("%s", MAX_DATA, &first_name);
    check(rc == 0, "Failed to read first name.");

    write_fmt("What's your middle initial? ");
    rc = read_scan("%c\n", &initial);
    check(rc == 0, "Failed to read initial.");

    write_fmt("What's your last name? ");
    rc = read_scan("%s", MAX_DATA, &last_name);
    check(rc == 0, "Failed to read last name.");

    write_fmt("How old are you? ");
    rc = read_scan("%d", &age);
    check(rc == 0, "Failed to read age.");

    write_fmt("---- RESULTS ----\n");
    write_fmt("First Name: %s\n", first_name);
    write_fmt("Middle Initial: %c.\n", initial);
    write_fmt("Last Name: %s\n", last_name);
    write_fmt("Age: %d\n", age);

    free(first_name);
    free(last_name);
    return 0;
error:
    return -1;
}