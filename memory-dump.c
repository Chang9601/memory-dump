#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct dummy {
  char a;
  int i;
  char b;
  int *p;
  double y;
};

struct node {
  char *str;
  struct node *next;
};

void
mem_dump(FILE *fp, char *ptr, int len) 
{
  int i, c;
  unsigned char buf[20];

  for (i = 0; i < len; i++) 
  {
    // 0xFF는 인덱스 i의 위치에 있는 값의 가장 낮은(least significant) 바이트를 추출한다.
    c = ptr[i] & 0xFF;

    if (i % 16 == 0) 
    {
      // %016lX는 64비트 부호 없는 long 정수의 형식을 지정한다(x는 소문자, X는 대문자.).
      // 메모리 주소는 32비트 시스템에서 8자리의 16진수, 64비트 시스템에서 16자리의 16진수로 표현된다.
      // %016는 16자리까지 0으로 채우며 lX는 인수를 부호 없는 long으로 해석하고 16진수로 출력한다.
      fprintf(fp, "0x%016lX: ", (unsigned long)&ptr[i]);
    }

    // %02x는 정수를 적어도 2자리의 16진수로 출력하도록 지정하며 필요한 경우 선행하는 0으로 채운다.
    fprintf(fp, " %02X", c);

    buf[i % 16] = (c >= 32 && c < 127) ? c : '.';

    if ((i + 1) % 16 == 0) 
    {
      buf[(i % 16) + 1] = '\0';
      fprintf(fp, " %s\n", buf);
    }
  }
}

int
main(int argc, char *argv[]) 
{
  char str[30];
  double y;
  struct node *head;
  struct dummy x;
  FILE *fp;

  y = 10;
  x.a = 'O';
  x.i = 111;
  x.b = '0';
  x.p = &x.i;
  strcpy(str, "메모리 덤프.\n");

  // 파일 포인터(FILE)는 실제 파일을 가리키지 않으며 파일에 관한 정보를 포함하는 데이터 객체를 가리킨다. 
  // 정보에는 파일 I/O에 사용되는 버퍼에 관한 정보도 포함되는데 표준 라이브러리의 I/O 함수는 버퍼를 사용하므로 버퍼의 위치를 알아야 하고 버퍼가 얼마나 차 있는지와 어떤 파일을 사용하는지를 알아야 한다. 
  // 이를 통해 함수들은 필요할 때 버퍼를 다시 채우거나 비울 수 있다. 
  // 파일 포인터에 의해 가리키는 데이터 객체에는 이러한 정보가 모두 포함되어 있다.
  if ((fp = fopen(argv[1], "wb")) == NULL)
  {
    fprintf(stderr, "오류 발생! 파일 \"%s\"을 열 수 없음.\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  fprintf(fp, "&x=0x%lX\n", (unsigned long)&x.a);
  fprintf(fp, "&y=0x%lX\n", (unsigned long)&y);


  mem_dump(fp, (char *)&x.a, 80);
  head = (struct node *)calloc(1, sizeof(*head));
  head->str = strdup("Whoa! ");
  head->next = (struct node *)calloc(1, sizeof(*head));
  head->next->str = strdup("Did you see ");
  head->next->next = (struct node *)calloc(1, sizeof(*head));
  head->next->next->str = strdup("that UFO across the sky?");
  head->next->next->next = NULL;
  fprintf(fp, "head=0x%lx\n", (unsigned long)head);
  mem_dump(fp, (char *)head, 512);

  for (struct node *curr_node = head, *next_node = curr_node->next; curr_node; curr_node = next_node, next_node = next_node ? next_node->next : NULL) 
  {
    free(curr_node->str);
    free(curr_node);
  }

  if (fclose(fp) != 0)
  {
    fprintf(stderr, "오류 발생! 파일을 닫을 수 없음.\n");
  }
  
  exit(EXIT_SUCCESS);
}