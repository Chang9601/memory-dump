#include <stdio.h>

int
is_little_endian()
{
  // 00000000 00000000 00000000 00001001
  // 메모리 주소는 왼쪽에서 오른쪽으로 증가한다.
  // 작은 끝(little-endian): 00001001 00000000 00000000 00000000
  // 큰 끝(big-endian): 00000000 00000000 00000000 00001001
  int num = 0x09;

  // 작은 끝: 00001001
  // 큰 끝: 00000000
  char *ptr = (char *)&num;

  // 작은 끝
  if (*ptr == 0x09) 
  {
    return 1;
  }

  // 큰 끝
  return 0;
}

int
main(int argc, char *argv[]) 
{
  if (is_little_endian()) {
    printf("작은 끝\n");
  } else {
    printf("큰 끝\n");
  }

  return 0;
}