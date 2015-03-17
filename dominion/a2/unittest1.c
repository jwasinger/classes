#include <stdio.h>

int compare(const void *a, const void* b)
{
  if (*(int*)a > *(int*)b)
    return 1;
  if (*(int*)a < *(int*)b)
    return -1;
  return 0;
}

int test_compare(void)
{
  int a = 1;
  int b = 2;
  int c = 2;

  if(compare(&a,&b) != -1)
    return 0;
  if(compare(&b,&a) != 1)
    return 0;
  if(compare(&b,&c) != 0)
    return 0;

  return 1;
}

int main(int argc, char **argv)
{
  if(test_compare())
    printf("test_compare() completed successfully\n");      
  else
    printf("test_compare() failed\n");

  return 0;
}
