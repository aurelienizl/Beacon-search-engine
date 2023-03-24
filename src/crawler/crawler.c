#define _GNU_SOURCE
#include "crawler.h"

void rewrite(int fd, const void *buf, size_t count)
{
	ssize_t res;
	int acc = 0;
	do
	{
		res = write(fd, (char *)buf + acc, count);
		if (res == -1)
		{
			errx(1, "write");
		}
		acc += res;
		count -= res;
	} while (count > 0);
}

char *get_request(const char *domain, const char *path, size_t *len)
{
	char *request;
	int res = asprintf(&request, "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", path, domain);
	if (res == -1)
	{
		errx(1, "asprintf error");
	}
	*len = res;
	return request;
}

char *get_content(const char *domain, const char *path)
{
	// Create the 'hints' structure.
	struct addrinfo hints;

	// Initialize all the fields to zero.
	memset(&hints, 0, sizeof(hints));

	// Specify the criteria of the hint structure
	hints.ai_family = AF_INET;		 // IPv4
	hints.ai_socktype = SOCK_STREAM; // TCP

	// Pointer to store the results
	struct addrinfo *result;

	// Get the linked list and the return value
	int err = getaddrinfo(domain, "80", &hints, &result);

	// Check if getaddrinfo performed
	if (err != 0)
	{
		printf("Error, cannot get the address info");
		return NULL;
	}

	// File descriptor for the socket
	int sfd;

	// Pointer used to iterate over the linked list
	struct addrinfo *p;

	for (p = result; p != NULL; p = p->ai_next)
	{
		// Try to create a socket
		sfd = socket(p->ai_family,
					 p->ai_socktype,
					 p->ai_protocol);

		// If an error occured, continue with the next operation
		if (sfd == -1)
		{
			continue;
		}

		// Try to connect the socket
		err = connect(sfd, p->ai_addr, p->ai_addrlen);

		// If successful, break the loop (p != 0)
		// If error, close the socket
		if (err != -1)
		{
			break;
		}
		else
		{
			close(sfd);
		}
	}

	if (p == NULL)
	{
		printf("Error, cannot connect to the socket");
		return NULL;
	}

	freeaddrinfo(result);

	size_t len = strlen(domain);
	char *query = get_request(domain, path, &len);

	rewrite(sfd, query, len);

	/*
	char tmp[BUFFER_SIZE];

	ssize_t a;

	char *buffer = calloc(BUFFER_SIZE, 1);
	while((a = read(sfd, tmp, BUFFER_SIZE)) > 0)
	{
		strcat(buffer, tmp);
		memset(tmp, 0, BUFFER_SIZE);
	}
*/
	ssize_t a;
	char tmp[BUFFER_SIZE];
	char *buffer = calloc(1, sizeof(int));
	do
	{

		memset(tmp, 0, BUFFER_SIZE);
		a = read(sfd, tmp, BUFFER_SIZE);
		if (a == -1)
		{
			printf("Error reading from socket");
			free(buffer);
			return NULL;
		}
		buffer = realloc(buffer, strlen(buffer) + a + 1);
		memcpy(buffer + strlen(buffer), tmp, a);
	} while (a > 0);


	
	close(sfd);
	free(query);

	return buffer;
}

