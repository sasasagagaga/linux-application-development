#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <libintl.h>
#include <locale.h>

#define _(STRING) gettext(STRING)

int main()
{
    setlocale (LC_ALL, "");
    bindtextdomain ("guesser", ".");
    textdomain ("guesser");

    const char *yes_answer = _("y");
    const char *no_answer = _("n");

    puts(_("Think of number between 1 and 100 exclusively."));

    int l = 0, r = 100;
    while (r - l > 1) {
        int m = (l + r) / 2;

        char ans[16];
        do {
            printf(_("Is your number greater than %d (y/n)?\n"), m);

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

    printf(_("Your guessed number is %d!\n"), r);
}
