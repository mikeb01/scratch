#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>

int main(int argc, char** argv)
{
  const char* data = "wibble";
  struct iovec v;

  v.iov_base = data;
  v.iov_len  = strlen(data);
  printf("sizeof(iovec) = %d\n", sizeof(struct iovec));
  printf("sizeof(size_t) = %d\n", sizeof(size_t));
  printf("sizeof(char*) = %d\n", sizeof(char*));
  printf("sizeof(socklen_t) = %d\n", sizeof(socklen_t));

  char* s = (char*) &v;

  printf("iovec %x\n", s);

}
