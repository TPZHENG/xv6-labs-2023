#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
 int i;

 if(argc < 2){
    fprintf(2, "Usage: sleep <ticks>\n");
    exit(1);
  }

  int n;
  for(i = 1; i < argc; i++){
     n = atoi(argv[i]);
     fprintf(1, "Sleeping...%d ticks.\n", n);
     if(sleep(n) < 0){
      fprintf(2, "sleep: %s failed\n", argv[i]);
      break;
    }
  }
  exit(0);
}
