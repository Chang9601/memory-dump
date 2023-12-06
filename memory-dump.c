#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct _Dummy {
  char a;
  int i;
  char b;
  int *p;
} Dummy;

typedef struct _Node {
  char *str;
  struct _Node *next;
} Node;

int
is_little_endian() {
  int num = 0x05;
  char *ptr = (char *)&num;

  if (*ptr == 0x05) {
    return 1;
  }

  return 0;
}

void
mem_dump(FILE *fp, char *p, int len) {
  int i;
  unsigned char buf[20];
  int cnt = 0;

  for(i = 0; i < len; i++) {
    int c = p[i] & 0XFF;

    if(i % 16 == 0) {
      fprintf(fp, "0x%016lX: ", (unsigned long)&p[i]);
    }

    fprintf(fp, " %02X", c);

    buf[i % 16] = (c >= 32 && c < 127) ? c : '.';

    if ((i + 1) % 16 == 0) {
      buf[(i % 16) + 1] = '\0';
      fprintf(fp, " %s\n", buf);
    }
  }
  //fprintf(fp, " %s\n", buf);
}

int
main(int argc, char *argv[]) {
  double y = 10;
  char str[30];
  int a = 33;
  int b = -44;
  Dummy x;
  Node *head;

  x.a = 'O';
  x.i = 111;
  x.b = '0';
  x.p = &x.i;
  strcpy(str, "Exercise memory dump program\n");

  FILE *fp;

  if ((fp = fopen(argv[1], "a")) == NULL) {
    fprintf(stdout, "can't open \"%s\" file\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  fprintf(fp, "&x=0x%lX\n", (unsigned long)&x.a);
  fprintf(fp, "&y=0x%lX\n", (unsigned long)&y);

  mem_dump(fp, (char *)&x.a, 80);
  head = (Node *)malloc(sizeof(*head));
  head -> str = strdup("Whoa! ");
  head -> next = (Node *)malloc(sizeof(*head));
  head -> next -> str = strdup("Did you see ");
  head -> next -> next = (Node *)malloc(sizeof(*head));
  head -> next -> next -> str = strdup("that UFO across the sky?");
  head -> next -> next -> next = NULL;
  fprintf(fp, "head=0x%lX\n", (unsigned long)head);
  mem_dump(fp, (char*)head, 256);

  if (fclose(fp) != 0) {
    fprintf(stderr, "error closing file\n");
  }

  return 0;
}