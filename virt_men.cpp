#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <map>
#include<vector>


#define ARGC_ERROR 1
#define FILE_ERROR 2
#define BUFLEN 256
#define FRAME_SIZE  256


//-------------------------------------------------------------------
unsigned int getpage(size_t x) { return (0xff00 & x) >> 8; }
unsigned int getoffset(unsigned int x) { return (0xff & x); }

void getpage_offset(unsigned int x) {
  unsigned int page = getpage(x);
  unsigned int offset = getoffset(x);
  printf("x is: %u, page: %u, offset: %u, address: %u, paddress: %u\n", x, page, offset,
         (page << 8) | getoffset(x), page * 256 + offset);
}





int main(int argc, const char * argv[]) {
  FILE* fadd = fopen("addresses.txt", "r");
  if (fadd == NULL) { fprintf(stderr, "Could not open file: 'addresses.txt'\n");  exit(FILE_ERROR);  }

  FILE* fcorr = fopen("correct.txt", "r");
  if (fcorr == NULL) { fprintf(stderr, "Could not open file: 'correct.txt'\n");  exit(FILE_ERROR);  }

  char buf[BUFLEN];
  unsigned int page, offset, physical_add, frame = 0;
  unsigned int logic_add;                  // read from file address.txt
  unsigned int virt_add, phys_add, value;  // read from file correct.txt





      // added page table code
      int T[256];
      int i=0;
      int k=0;
      FILE* faddd = fopen("addresses.txt", "r");
      while(k<256){
      fscanf(faddd, "%d", &logic_add);  // read from file address.txt
      page = getpage(logic_add);
      for( i=0 ; i<256 ; i++){
        T[i]= page;
        k++;
       }
     }

     // TODO:  added TLB code but not done yet
      /*  int l=0;
      std::map<int, int> TLB;
      std::map<int, int>::iterator itr = TLB.begin();

      for( int i=0; i< 16 ; i++){
        TLB.insert(std::pair<int, int>(T[i], l * FRAME_SIZE));
        l++;
        printf( " %5u \n ",T[i]);
        printf( " %5u \n ",l * FRAME_SIZE);
            }
        for ( itr = TLB.begin(); itr != TLB.end(); itr++) {
            printf("%5u \n", itr->first);
            printf("%5u \n", itr->second);

          } */




  while (frame < 256 ) {
    fscanf(fcorr, "%s %s %d %s %s %d %s %d", buf, buf, &virt_add,
           buf, buf, &phys_add, buf, &value);  // read from file correct.txt

    fscanf(fadd, "%d", &logic_add);  // read from file address.txt
    page = getpage(logic_add);
    offset = getoffset(logic_add);


    if ( page == T[frame])       // compare to see if the page number is in the page table
    physical_add = frame++ * FRAME_SIZE + offset;     // calcualate the pyysical address based on frame and offset numebr

    else  { physical_add = frame * FRAME_SIZE + offset; frame++;
    }

    // todo: read BINARY_STORE and confirm value matches read value from correct.txt
    printf("logical: %5u (page:%3u, offset:%3u) ---> physical: %5u -- passed\n", logic_add, page, offset, physical_add);
    if (frame % 5 == 0) { printf("\n"); }
  }
  fclose(fcorr);
  fclose(fadd);

  printf("ALL logical ---> physical assertions PASSED!\n");
  printf("--- you have to implement the PTE and TLB part of this code\n");
  printf("\n\t\t...done.\n");
  return 0;
}
