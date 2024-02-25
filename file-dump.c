#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void
file_dump(FILE *fp, char *ptr, int len) 
{
  int i, c;
  unsigned char buf[20];

  for (i = 0; i < len; i++) 
  {
    c = ptr[i] & 0xFF;

    if (i % 16 == 0) 
    {
      fprintf(fp, "0x%016lX: ", (unsigned long)&ptr[i]);
    }

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
  FILE *in_fp, *out_fp;
  char *in_file;
  long file_size;

  if ((out_fp = fopen(argv[1], "wb")) == NULL) 
  {
    fprintf(stderr, "오류 발생! 파일 \"%s\"을 열 수 없음.\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  // 이진 모드를 사용하여 MS-DOS 텍스트 및 Unix 파일 모두를 처리할 수 있다.
  if ((in_fp = fopen(argv[2], "rb")) == NULL) 
  {
    fprintf(stderr, "오류 발생! 파일 \"%s\"을 열 수 없음.\n", argv[2]);
    exit(EXIT_FAILURE);
  }

  // fseek() 함수를 사용하면 fopen()으로 열린 파일을 배열처럼 취급하고 파일에서 특정한 바이트로 직접 이동할 수 있다.
  // 1번 인자는 파일을 찾는 데 사용되는 파일 포인터로 파일은 fopen()을 사용하여 열려야 한다.
  // 2번 인자는 오프셋이라고 불리며 시작점으로부터 얼마나 멀리 이동할지를 나타내며 long 타입이어야 한다.
  //   양수: 앞으로 이동.
  //   음수: 뒤로 이동.
  //   0: 현재 위치에 정지.
  // 3번 인자는 모드로 시작점을 식별한다.
  //   SEEK_SET: 파일의 시작.
  //   SEEK_CUR: 현재 위치.
  //   SEEK_END: 파일의 끝.
  fseek(in_fp, 0L, SEEK_END);
  // ftell() 함수는 파일 내 현재 위치를 long 타입으로 반환한다.
  // 파일 위치를 지정하기 위해 시작점부터의 바이트 수를 반환하며 1번째 바이트는 바이트 0이며 그 다음은 바이트 1과 같이 진행된다.
  // ANSI C에서는 이 정의가 이진 모드로 열린 파일에 적용되지만, 텍스트 모드로 열린 파일에는 꼭 적용되지는 않을 수 있다.
  file_size = ftell(in_fp);
  fseek(in_fp, 0L, SEEK_SET);

  in_file = (char *)calloc(1, file_size);
  // 데이터가 프로그램이 사용하는 표현과 동일한 형식으로 파일에 저장될 때 해당 데이터는 이진 형식으로 저장된다.
  // 표준 I/O에서는 fread() 함수가 fwrite() 함수가 이진 서비스를 제공한다.
  // 모든 데이터는 이진 형식으로 저장된다. 문자도 문자 코드의 이진 표현을 사용하여 저장된다. 
  // 파일의 모든 데이터가 문자 코드로 해석된다면 해당 파일은 텍스트 데이터를 포함한다.
  // 일부 또는 모든 데이터가 이진 형식의 숫자 데이터, 기계어 명령 등으로 해석된다면 해당 파일은 이진 데이터를 포함한다.
  // int num = 13579 <=> 00110101 00001011
  // fprintf(fp, "%d", num): 파일에 문자 '1', '3', '5', '7', '9'의 이진 코드를 작성한다. 즉, 00110001 00110011 00110101 00110111 00111001.
  // fwrite(&num, sizeof(int), 1, fp): 값 12345의 이진 코드를 파일에 작성한다. 즉, 00110101 00001011.
  // 정보는 이진 데이터로 저장되거나 읽힐 수 있고, 또는 텍스트 데이터로 저장되거나 읽힐 수 있다. 
  // 텍스트 형식 파일을 이진 모드로 열 수 있고 텍스트를 이진 형식 파일에 저장할 수 있다. 
  // 일반적으로 이진 모드를 사용하여 이진 형식 파일에 이진 데이터를 저장하고 텍스트 데이터는 텍스트 형식으로 열린 텍스트 파일에서 사용한다.
  fread(in_file, sizeof(char), file_size, in_fp);

  file_dump(out_fp, in_file, file_size);

  free(in_file);
  if (fclose(out_fp) != 0 || fclose(in_fp) != 0)
  {
    fprintf(stderr, "오류 발생! 파일을 닫을 수 없음.\n");
  }

  exit(EXIT_SUCCESS);
}