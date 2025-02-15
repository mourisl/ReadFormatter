#include <stdio.h>

#include "ReadFormatter.hpp"

// Usage: a.out format

int main(int argc, char *argv[])
{
  ReadFormatter formatter ;
  char read[] = "ATCGATCGATCGATCG" ;
	char comment[] = "CB:Z:ACGT-1 CB:Z:TGCA-2 UB:Z:GGGG" ;
  char qual[] = "ABCDEFGHIJKLMOPQ" ;

  formatter.Init(argv[1]) ;
  printf("@r1\n%s\n-\n%s\n", formatter.Extract(read, FORMAT_READ1, true, true),
      formatter.Extract(qual, FORMAT_READ1, false, true, 1)) ;
  printf("@r2\n%s\n-\n%s\n", formatter.Extract(read, FORMAT_READ2, true, true),
      formatter.Extract(qual, FORMAT_READ2, false, true, 1)) ;
  
  if (!formatter.IsInComment(FORMAT_BARCODE))
    printf("@bc\n%s\n-\n%s\n", formatter.Extract(read, FORMAT_BARCODE, true, true),
      formatter.Extract(qual, FORMAT_BARCODE, false, true, 1)) ;
  else
    printf("@bc\n%s\n-\n%s\n", formatter.Extract(comment, FORMAT_BARCODE, true, true),
      formatter.Extract(comment, FORMAT_BARCODE, false, true, 1)) ;

  if (!formatter.IsInComment(FORMAT_UMI))
    printf("@um\n%s\n-\n%s\n", formatter.Extract(read, FORMAT_UMI, true, true),
      formatter.Extract(qual, FORMAT_UMI, false, true, 1)) ;
  else
    printf("@um\n%s\n-\n%s\n", formatter.Extract(comment, FORMAT_UMI, true, true),
      formatter.Extract(comment, FORMAT_UMI, false, true, 1)) ;
  
  return 0 ;
}
