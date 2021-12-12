/** @file guesser.c
 * @brief Guesses your number.
 *
 * There is a support of arabic and roman digits.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <libintl.h>
#include <locale.h>
#include <argp.h>
#include "config.h"

#define _(STRING) gettext(STRING)

const char *argp_program_version = PACKAGE_STRING;
const char *argp_program_bug_address = PACKAGE_BUGREPORT;

/* Program documentation. */
// static char doc[] = _("Guesser -- a program that guesses your number");

/* The options we understand. */
static struct argp_option options[] = {
    {"help", '?', 0, 0, 0 },
    {"usage", 'u', 0, 0, 0 },
    {"use_roman", 'r', 0, 0, 0 },
    { 0 }
};

/* Used by main to communicate with parse_opt. */
struct arguments
{
    int use_roman;  /**< Use roman digits or not */
};

/* Parse a single option. */
static error_t parse_opt (int key, char *arg, struct argp_state *state)
{
    /* Get the input argument from argp_parse, which we
       know is a pointer to our arguments structure. */
    struct arguments *arguments = state->input;

    switch (key) {
        case '?':
            printf(_("Usage: guesser [OPTION...]\n"
                     "Guesser -- a program that guesses your number\n"
                     "\n"
                     "  -r, --use_roman            Use roman digits, not arabic\n"
                     "  -?, --help                 Give this help list\n"
                     "  -u  --usage                Give a short usage message\n"
                     "  -V, --version              Print program version\n"
                     "\n"
                     "Report bugs to %s.\n"), PACKAGE_BUGREPORT);
            exit(0);
        case 'u':
            printf(_("Usage: guesser [-r?V] [--use_roman] [--help] [--usage] [--version]\n"));
            exit(0);
        case 'r':
            arguments->use_roman = 1;
            break;

        case ARGP_KEY_ARG:
            /* Too many arguments. */
            argp_usage(state);
            break;

        default:
            return ARGP_ERR_UNKNOWN;
    }

    return 0;
}

/* Our argp parser. */
static struct argp argp = { options, parse_opt, 0, 0 };  // doc };


#define MAX_NUMBER 3999  /**< Maximum number for guessing. */

/**
 * Determines length of arabic digit converted to roman.
 * @param x an arabic digit.
 * @return length of x as a roman digit.
 */
int roman_digit_length(int x)
{
    if (x < 1 || x > 9) {
        return 0;
    }
    if (x == 5 || x == 9) {
        return 2;
    }
    if (x < 5) {
        return x;
    }
    return x - 4;
}

/**
 * Determines length of arabic number converted to roman.
 * @param x an arabic number.
 * @return length of x as a roman number.
 */
int roman_length(int x)
{
    if (x < 1 || x > MAX_NUMBER) {
        return 0;
    }

    int length = 0;
    while (x) {
        length += roman_digit_length(x % 10);
        x /= 10;
    }
    return length;
}

/**
 * Prints to buf given arabic digit converted to roman.
 * @param buf a buffer to write result to.
 * @param x an arabic digit.
 * @param one a digit for ones for specific position in number.
 * @param five a digit for fives for specific position in number.
 * @param ten a digit for tens for specific position in number.
 * @return 1 if x is not a digit and 0 otherwise.
 */
int print_roman_digit(char **buf, int x, char one, char five, char ten)
{
    if (x < 1 || x > 9) {
        return 1;
    }

    if (x == 4) {
        *(*buf)++ = one;
        *(*buf)++ = five;
    } else if (x == 9) {
        *(*buf)++ = one;
        *(*buf)++ = ten;
    } else {
        if (x >= 5) {
            *(*buf)++ = five;
            x -= 5;
        }
        for (int i = 0; i < x; ++i) {
            *(*buf)++ = one;
        }
    }

    return 0;
}

/**
 * Converts given arabic number to roman.
 * @param x an arabic number.
 * @return pointer to allocated representation of given number. User have to free allocated memory.
 */
char *arabic2roman(int x)
{
    static const char *roman_digits = "IVXLCDM--";  /* Roman digits for ones, fives and tens in different positions in number. */

    if (x < 1 || x > MAX_NUMBER) {
        return NULL;
    }
    
    int length = roman_length(x);
    char *buf = (char *) calloc(length + 1, sizeof(char));
    
    char *p = buf;
    for (int pow10 = 1000, power = 3; pow10 > 0; pow10 /= 10, --power) {
        const char *digits = roman_digits + 2 * power;
        print_roman_digit(&p, x / pow10, digits[0], digits[1], digits[2]);
        x %= pow10;
    }
    return buf;
}

/**
 * Returns needed representation of given arabic number.
 * @param x an arabic number.
 * @param use_roman 1 if roman representation is needed and 0 if arabic representation is needed.
 * @return pointer to allocated representation of given number. User have to free allocated memory.
 */
char *number_representation(int x, int use_roman)
{
    if (use_roman) {
        return arabic2roman(x);
    }

    int length = snprintf(NULL, 0, "%d", x);
    char *buf = (char *) calloc(length + 1, sizeof(char));
    snprintf(buf, length + 1, "%d", x);
    return buf;
}

int main(int argc, char **argv)
{
    setlocale (LC_ALL, "");
    bindtextdomain ("guesser", ".");
    textdomain ("guesser");

    struct arguments arguments;
    /* Default values. */
    arguments.use_roman = 0;

    /* Parse our arguments; every option seen by parse_opt will
       be reflected in arguments. */
    argp_parse (&argp, argc, argv, 0, 0, &arguments);
    int use_roman = arguments.use_roman;

    const char *yes_answer = _("y");  /* String representation of yes answer of user */
    const char *no_answer = _("n");  /* String representation of no answer of user */

    char *min_repr = number_representation(1, use_roman);
    char *max_repr = number_representation(MAX_NUMBER, use_roman);
    printf(_("Think of number between %s and %s exclusively.\n"),
            min_repr, max_repr);
    free(min_repr);
    free(max_repr);

    int l = 0, r = MAX_NUMBER;
    while (r - l > 1) {
        int m = (l + r) / 2;

        char ans[16];
        do {
            char *m_repr = number_representation(m, use_roman);
            printf(_("Is your number greater than %s (y/n)?\n"), m_repr);
            free(m_repr);

            if (fgets(ans, 14, stdin) != ans) {
                return 1;
            }
            size_t len_ans = strlen(ans);
            if (len_ans == 0) {
                continue;
            }
            if (ans[len_ans - 1] == '\n') {
                ans[len_ans - 1] = '\0';
            }
        } while (strcmp(ans, yes_answer) && strcmp(ans, no_answer));

        if (strcmp(ans, yes_answer)) {
            r = m;
        } else {
            l = m;
        }
    }

    char *ans_repr = number_representation(r, use_roman);
    printf(_("Your guessed number is %s!\n"), ans_repr);
    free(ans_repr);
}
