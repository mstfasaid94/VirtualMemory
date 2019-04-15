//Moustapha Said  --- CWID 888907524 
//Project:: Virtual Memory 
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>


#define SIZE 256
#define FRAMES 256
#define TBUFF_SIZE 16 //cache size 
#define MASK 255 
#define O_MASK 255 
#define O_BITS 8 // offset number of bits
#define MEMSize FRAMES * SIZE
#define BUFLEN 256 
#define ARGC_ERROR 1
#define FILE_ERROR 1




struct TLB{
    unsigned char virt_address;
    unsigned char phys_address;
};

struct TLB tlb[TBUFF_SIZE];       //struct array for the cache is the size of the cache 
int PageTable[FRAMES];            //physical page number for the virtual address
signed char memory [MEMSize];




int positive(int a, int b)
{
    if(a>b)
    return a;
    return b;
}


int tlb_num = 0;

//pointer for backing file
signed char *back_store;

//translate virtual page to physical page
int translate(unsigned char virt_page){
    for(int i = positive((tlb_num - TBUFF_SIZE), 0); i < tlb_num; i++)
    {
        struct TLB *entry = &tlb[i % TBUFF_SIZE];
        if (entry -> virt_address == virt_page)
        {
            return entry -> phys_address;
        }
    }
    return -1;
}
int getOffset(int x) {return x & O_MASK; }   // getting offset 
 
int getPage(int x) {return (x >> O_BITS) & MASK;}   // getting page number 


void Fifo(unsigned char virtual, unsigned char physical){   //Fifo
    struct TLB *entry = &tlb[tlb_num % TBUFF_SIZE];
    tlb_num++;
    entry -> virt_address = virtual;
    entry -> phys_address = physical;
}


int main(int argc, const char*argv[])
{

    if(argc != 2){
        fprintf(stderr, "Please run code with ./virt_mem addresses.txt");
        exit(ARGC_ERROR);
    }
    char buff[BUFLEN];
    int Counter = 0;
    int tlbHits = 0;
    int PageFaults = 0;
    unsigned char free_pg = 0; //next unallocated page in memory

    const char *filename = argv[1];
    FILE *fp = fopen(filename, "r");
    if(fp == NULL)
    {
        fprintf(stderr, "Could not open file: \'%s \' \n", filename);
        exit(FILE_ERROR);
    }

    int backing = open("BACKING_STORE.bin", O_RDONLY);
    back_store = mmap(0, MEMSize, PROT_READ, MAP_PRIVATE, backing, 0);

    //fill page table with -1 if the table is empty
    
    for(int i = 0; i < FRAMES; i++)
    {
        PageTable[i] = -1;
    }

    while(fgets(buff, BUFLEN, fp) != NULL)
    {
        Counter++;
        int virtAdd = atoi(buff);
        int offset = getOffset(virtAdd);
        int virtPage = getPage(virtAdd);

        int physPage = translate(virtPage);

        //Check for TLB hit or miss

        if(physPage != -1)
        {
            tlbHits++; //when Hit occurs 
        }
        else 
        {
            physPage = PageTable[virtPage]; //when Miss occurs 
          


           //Check for page fault by comapring to wha is inside the page table 

            if(physPage == -1)
            {
                PageFaults++;
                physPage = free_pg;
                free_pg++;

               // -------------------------------------------------------
               //To copy page from the backing file into the phys memory
                memcpy(memory + physPage * SIZE, back_store + virtPage * SIZE, SIZE);
                PageTable[virtPage] = physPage;
            }

            Fifo(virtPage, physPage);
        }

        int physAdd = (physPage << O_BITS) | offset;
        signed char value = memory[physPage * SIZE + offset];
        printf("Virtual address: %d Physical Address: %d Value: %d \n", virtAdd, physAdd, value);
        


    }
   

    printf("\n\n"); 
    
    printf("Page Faults number is = %d\n", PageFaults);
    printf("Page Faults Rate is = %.3f\n", PageFaults/(1. * Counter));
    printf("TLB Hits number is = %d\n", tlbHits);
    printf("TLB Hit Rate is = %.3f\n", tlbHits/(1. * Counter));
    printf("The Number of Addresses is  = %d\n", Counter);
    return 0;
}









