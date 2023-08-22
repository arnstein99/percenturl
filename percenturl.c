#include <stdlib.h>
#include <stdio.h>

static void complain_short(size_t index);
static unsigned char char_to_num(size_t index, unsigned char chr);

int main(int argc, char* argv[])
{
    if (argc != 1)
    {
        fprintf(stderr, "No command line arguments or options\n");
        exit(1);
    }

    size_t index;
    int chr;
    unsigned char value;

    while (1)
    {
        if ((chr = getchar()) == EOF) break;
        ++index;
        if (chr == '%')
        {
            if ((chr = getchar()) == EOF) complain_short(index);
            ++index;
            value = 16 * char_to_num(index, chr);
            if ((chr = getchar()) == EOF) complain_short(index);
            ++index;
            value += char_to_num(index, chr);
            putchar((char)value);
        }
        else
        {
            putchar(chr);
        }
    }
    fprintf(stderr, "Processed %lu characters\n", index);

    return 0;
}

static void complain_short(size_t index)
{
    fprintf(stderr, "Incomplete hex expression at location %lu\n", index);
    exit(1);
}

static unsigned char char_to_num(size_t index, unsigned char chr)
{
    if ((chr >= '0') && (chr <= '9')) return chr - '0';
    if ((chr >= 'a') && (chr <= 'f')) return 10 + (chr - 'a');
    if ((chr >= 'A') && (chr <= 'F')) return 10 + (chr - 'A');
    fprintf(stderr, "Illegal hex character '%c' at location %lu\n", chr, index);
    exit(1);
}
