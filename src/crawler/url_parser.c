#include "crawler.h"

int is_domain(char *url, char *domain)
{
    if (strstr(url, domain) != NULL)
        return 1;
    else
        return 0;
}

char *get_domain(char *link)
{
    char start[5] = "http";
    char *domain = calloc(sizeof(char), strlen(link) + 1);
    size_t i = 0;

    while (i < 4)
    {
        if (link[i] < 97)
        {
            if (start[i] != (link[i] + 32))
            {
                break;
            }
        }
        else
        {
            if (start[i] != link[i])
            {
                break;
            }
        }
        i++;
    }

    if (i != 4)
        return NULL;

    if (link[i] == 's')
        i++;

    size_t j = 0;
    i += 3;
    while (link[i] != '\0')
    {
        if (link[i] == '/')
        {
            break;
        }
        domain[j] = link[i];
        i++;
        j++;
    }

    domain[j] = '\0';
    domain = realloc(domain, sizeof(char) * (j + 1));
    return domain;
}