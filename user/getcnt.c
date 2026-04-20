#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

static int
is_decimal(const char *s)
{
    int i;

    if (s == 0 || s[0] == '\0')
        return 0;

    i = 0;
    if (s[0] == '-')
    {
        if (s[1] == '\0')
            return 0;
        i = 1;
    }

    for (; s[i] != '\0'; i++)
    {
        if (s[i] < '0' || s[i] > '9')
            return 0;
    }
    return 1;
}

int main(int argc, char *argv[])
{
    int num;
    int count;

    if (argc != 2)
    {
        fprintf(2, "usage: getcnt syscall_number\n");
        exit(1);
    }

    if (!is_decimal(argv[1]))
    {
        fprintf(2, "getcnt: syscall_number must be an integer\n");
        exit(1);
    }

    num = atoi(argv[1]);
    count = getcnt(num);
    if (count < 0)
    {
        fprintf(2, "getcnt: invalid syscall number %d\n", num);
        exit(1);
    }

    printf("syscall %d has been called %d times\n", num, count);
    exit(0);
}
