#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int main()
{
	char *str;
	int page;
	int block;
	char c;
	char buf[0];
	char buf2[10];
	int offset;
	int address;
	int blockSize = 10;
	int present_count = 0;
	int i;
	int p = -1;
	int repage; 
	struct ram_Buffer{
		char page[3][10];
	}*ram;
	
	struct entry{
	int present;
	int raddress;
	}page_table[10];
	
	 for (i = 0; i <10; i = i + 1)
	 {
	 	page_table[i].present = 0;
	 	page_table[i].raddress = -1;
	 }
	
	ram = malloc(sizeof(struct ram_Buffer));
	
	int fd;
	int fd2;

	const char filename[] = "text_file";
	const char filename2[] = "backing_store";
	fd = open(filename,  O_RDWR);
	fd2 = open( filename2,O_RDWR);

	
   for (address = 0; address< 100; address++)
   {
	   block = (address/10);
    	offset = address % 10;

	   read(fd,buf , 1);
	   c = buf[0];



	   if (page_table[block].present == 0)//page is not in ram
	   {
	      for (i = 0; i<10; i=i+1)
	      {
	         if(page_table[i].present == 1)//checks for how many page frames are used
	         {
	         	present_count++;
	         	if (present_count == 1)//picks the first page it finds
	         	{
	         	   repage = i;
	         	}
	         }
	         
	         if (present_count == 3)//ram is full
	         {
	            lseek(fd2, blockSize*(repage), SEEK_SET);
	            write(fd2,ram->page[page_table[repage].raddress], blockSize);
	            page_table[repage].present = 0;
	            page_table[repage].raddress = -1;
	            i = 11;

	         } 
	      }
	   }

	   present_count = 0;
	   //update page_table
	   if (offset == 0)
	   {
	   p++;
	      if (p==3)
	      {
	         p = 0;
	      }	   
	   }
	   
	  
	   page_table[block].raddress = p;
	   page_table[block].present = 1;
	   
	   //put into ram
		ram->page[page_table[block].raddress][offset] = c;

		//printf("block is %i\n", block);
		//printf("offset is %i\n", offset);
		//printf("\n");
	   //printf("ram->page[page_table[block].raddress][offset] %c\n", ram->page[page_table[block].raddress][offset]);
	   //printf ("ram->page[page_table[repage].raddress] %s\n",ram->page[page_table[repage].raddress]);
      
	   	      
	}
	repage++;
	for(;repage<10;repage++)
	{
	write(fd2,ram->page[page_table[repage].raddress], blockSize);
	}
	

	close(fd);
	close(fd2);
	free(ram);
	
	return (0);
}
