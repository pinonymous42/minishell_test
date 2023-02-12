#include <unistd.h>
#include <stdio.h>
 
#define FILE_PATH "/sbin/cat"
 
int main (void)
{
  /* 読み込み許可と実効許可があるかをチェック */
  // if (access (FILE_PATH, X_OK))
  //   {
  //     perror ("access");
  //     return (1);
  //   }
 
  // printf ("Confirm result X_OK: PASS!\n");
  printf("%d\n", access(FILE_PATH, X_OK));
  return 0;
}