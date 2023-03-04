#define _GNU_SOURCE

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <unistd.h>

const size_t BUFFER_SIZE = 32;


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

char *build_query(const char *host, size_t *len)
{
	char *request;
	int res = asprintf(&request, "GET http://www.%s/ HTTP/1.0\r\n\r\n", host);
	if (res == -1)
	{
		errx(1, "asprintf error");
	}
	*len = res;
	return request;
}

char* get_page(const char *host)
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
	int err = getaddrinfo(host, "80", &hints, &result);

	// Check if getaddrinfo performed
	if (err != 0)
	{
		errx(1, "getaddrinfo error");
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

	// If p == NULL, no connection was made
	if (p == NULL)
	{
		errx(EXIT_FAILURE, "Couldn't connect");
	}

	// Get the length of the query
	size_t len = strlen(host);
	char *query = build_query(host, &len);

	// Send the query
	rewrite(sfd, query, len);

	// Receive the response
	size_t a;
	char *buffer = calloc(BUFFER_SIZE, sizeof(int));
	char *webpage = calloc(BUFFER_SIZE, sizeof(int));

	do
	{
		a = read(sfd, buffer, BUFFER_SIZE);
		// Realloc webpage to store new data
		webpage = realloc(webpage, strlen(webpage) + a);
		// Merge the new data with the old data
		strcat(webpage, buffer);
		// Clear the buffer
		memset(buffer, 0, BUFFER_SIZE);
	} while (a > 0);

	close(sfd);
	free(query);
	free(buffer);
	freeaddrinfo(result);

	return webpage;
}

