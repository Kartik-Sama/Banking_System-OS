#include<stdio.h>
#include"records.h"

//Prints the length of all records in the system

int main() {
	printf("Admin record : %ld\nNormal record : %ld\nJoint record : %ld\nFund record : %ld\n",sizeof(admin),sizeof(normal),sizeof(joint),sizeof(fund));
}
