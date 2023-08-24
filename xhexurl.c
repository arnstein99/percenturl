#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static int char_to_num(unsigned char chr);

int main(int argc, char* argv[])
{
    int verbose = 0;
    switch (argc)
    {
    case 1:
        break;
    case 2:
        if ((strcmp(argv[1], "-v") == 0) ||
            (strcmp(argv[1], "--verbose") == 0))
        {
            verbose = 1;
        }
        else
        {
            fprintf(stderr, "Usage: xhexurl [-v|--verbose]\n");
            exit(1);
        }
        break;
    default:
        fprintf(stderr, "Usage: xhexurl [-v|--verbose]\n");
        exit(1);
        break;
    }

    setvbuf(stdout, NULL, _IONBF, 0);

    size_t index = 0;
    int chr, old_chr, old_old_chr;
    int value, partial_value;

    while (1)
    {
        if ((chr = getchar()) == EOF)
        {
            break;
        }
        ++index;
        if (chr != '\\')
        {
            putchar(chr);
            continue;
        }
        /* Now holding "\" */
        if ((chr = getchar()) == EOF)
        {
            putchar('\\');
            break;
        }
        ++index;
        if ((chr != 'x') && (chr != 'X'))
        {
            putchar('\\');
            putchar(chr);
            continue;
        }
        /* Now holding "\x" */
        old_chr = chr;
        if ((chr = getchar()) == EOF)
        {
            putchar('\\');
            putchar(old_chr);
            break;
        }
        value = char_to_num(chr);
        if (value == -1)
        {
            putchar('\\');
            putchar(old_chr);
            putchar(chr);
            continue;
        }
        /* Now holding "\nx" */
        old_old_chr = old_chr;
        old_chr = old_chr;
        value *= 16;
        if ((chr = getchar()) == EOF)
        {
            putchar('\\');
            putchar(old_old_chr);
            putchar(old_chr);
            break;
        }
        ++index;
        partial_value = char_to_num(chr);
        if (partial_value == -1)
        {
            putchar('\\');
            putchar(old_old_chr);
            putchar(old_chr);
            putchar(chr);
            continue;
        }
        /* Now holding "\nxx" */
        value += partial_value;
        putchar(value);
    }
    if (verbose) fprintf(stderr, "Processed %lu characters\n", index);

    return 0;
}

static int char_to_num(unsigned char chr)
{
    if ((chr >= '0') && (chr <= '9')) return chr - '0';
    if ((chr >= 'a') && (chr <= 'f')) return 10 + (chr - 'a');
    if ((chr >= 'A') && (chr <= 'F')) return 10 + (chr - 'A');
    return -1;
}
