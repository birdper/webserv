#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv, char **envp)
{
    printf("Content-Type: text/plain\n\n");
    for (int i = 0; envp[i] != NULL; i++)
    {
        printf("%d: %s\n", i, envp[i]);
    }
    printf("These were all the environment variables\n");
    return 0;
}
