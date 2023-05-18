#include "crawler.h"
#include <string.h>
#include <ctype.h>

int is_domain(const char *url, const char *domain)
{
    if (url == NULL || domain == NULL) {
        return 0;
    }
    return (strstr(url, domain) != NULL) ? 1 : 0;
}

char *get_domain(const char *link)
{
    if (link == NULL) {
        return NULL;
    }

    // Check if link starts with "http" or "https"
    if (strncasecmp(link, "http", 4) != 0) {
        return NULL;
    }

    size_t i = (link[4] == 's') ? 5 : 4;
    if (link[i] != ':' || link[i + 1] != '/' || link[i + 2] != '/') {
        return NULL;
    }

    i += 3;  // Skip "://"
    const char *domain_start = &link[i];
    const char *slash = strchr(domain_start, '/');

    size_t domain_len = (slash != NULL) ? (size_t)(slash - domain_start) : strlen(domain_start);
    char *domain = (char *)malloc(sizeof(char) * (domain_len + 1));

    if (domain != NULL) {
        strncpy(domain, domain_start, domain_len);
        domain[domain_len] = '\0';
    }

    return domain;
}
