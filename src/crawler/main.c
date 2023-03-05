#include "crawler.h"

int main(int argc, const char **argv)
{

  int err_code;
  Memory page;

  page.data = malloc(1);
  page.size = 0;

  err_code = GetPage(argv[1], NULL, &page);

  if (err_code < 0)
  {
    printf("Error: %d\n", err_code);
  }
  else
  {
    printf("%s\n", page.data);
  }

  free(page.data);
  return 0;
}
