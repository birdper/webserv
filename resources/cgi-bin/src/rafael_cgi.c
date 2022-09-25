/*
 * Prints all ENV variables.
 * And do uppercase the <body>.
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>

int main(int argc, char **argv, char **envp)
{
    printf("Content-Type: text/plain\n\n");
    for (int i = 0; envp[i] != NULL; i++)
    {
        printf("%d: %s\n", i, envp[i]);
    }
    printf("These were all the environment variables.\n");

    printf("Here is the body with UPPERCASE.\n");
    char buf[1024];
    bzero(buf, sizeof(buf));
    int ret = 0;
    while( (ret = read(0, buf, 1022)) )
    {
        buf[ret+1] = '\0';
        //make body uppercase
        char *ch_ptr = buf;
        while (*ch_ptr != '\0')
        {
            *ch_ptr = toupper(*ch_ptr);
            ch_ptr++;
        }
        //print the body
        printf("%s", buf);
        bzero(buf, sizeof(buf));
    }

    return 0;
}
