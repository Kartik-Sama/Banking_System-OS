#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/stat.h>
#include"records.h"

int main() {
	printf("Adds admin to the admin data file\n");
	printf("Enter admin id(should start with 'admin'):");
	admin record; //admin is struct defined in bank.h
	scanf("%s",record.userId);
	printf("Enter password for the admin : ");
	scanf("%s",record.password);
	int fd, bytes_written;
	fd = open("./admin.dat",O_WRONLY);
	lseek(fd,0,SEEK_END);
	if(fd == -1) {
		printf("Error occured while opening the file\n");
		return -1;
	}
	bytes_written = write(fd,&record,sizeof(record));
	if(!bytes_written) {
		printf("Error occured while writing record to the file\n");
		return -1;
	}
	close(fd);
	printf("Successfully added the admin record\n");
	return 0;
}	
