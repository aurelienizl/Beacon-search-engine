#include "find.h"

int main()
{
    char input[512];
    
    if (fgets(input, sizeof(input), stdin) != NULL) 
    {
        // Remove the trailing newline character, if present
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') 
        {
            input[len - 1] = '\0';
        }
    }
    
    char* query = strdup(input);
    find(&query);

    return 0;
}