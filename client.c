#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include"records.h"

//This is success function is a helper function that outputs whether an operation was successfull or not
void success(int flag) {
	if(flag == 1) 
		printf("Operation successful\n");
	else if(flag == 0)
		printf("Operation unsuccessful\n");
	else
		printf("Unexpected behaviour\n");
}

//Based on the userId's first characters determines user type
int user_type(char* loginId) {
	if(!strncmp(loginId,"admin",5)) 
		return 1;
	else if(!strncmp(loginId,"normal",6))
		return 2;
	else if(!strncmp(loginId,"joint",5))
		return 3;
	return 0;
}

//The login function. Checks if userId is valid and then forwards query to server
int login(int sfd) {
	int result;
	char userId[64] = {0}, password[64] = {0};
	printf("Enter user id:\n");
	scanf("%s",userId);
	printf("Enter password:\n");
	scanf("%s",password);
	int type = user_type(userId);
	write(sfd,&type,sizeof(int));
	write(sfd,userId,64);
	write(sfd,password,64);
	if(!type) {
		printf("Invalid user\n");
		return 0;
	}
	read(sfd,&result,sizeof(int));
	if(result)
		printf("Login successful\n");
	else {
		printf("Invalid login\n");
		return 0;
	}
	return type;
}

//Displays the login portal 
int option(int utype) {
	int chose;
	if(utype == 2 || utype == 3) {
		printf("Option\t\tAction\n");
		printf("0\t\tDeposit\n");
		printf("1\t\tWithdraw\n");
		printf("2\t\tBalance Enquiry\n");
		printf("3\t\tPassword Change\n");
		printf("4\t\tView Details\n");
		printf("5\t\tExit\n");
	}
	else if(utype == 1) {
		printf("Option\t\tAction\n");
		printf("1\t\tAdd account\n");
		printf("2\t\tDelete account\n");
		printf("3\t\tModify account\n");
		printf("4\t\tSearch account\n");
		printf("5\t\tExit\n");
	}
	printf("Enter your choice : ");
	scanf("%d",&chose);
	if(chose > 6 || chose < 0 || (chose == 0 && utype == 1)) {
		printf("Invalid choice\n");
		return -1;
	}
	return chose;
}

//when user type = 1 i.e. admin add account function
void addAccount(int sfd) {
	char userId[64] = {0}, password[64] = {0};
	int utype = 0,flag;
	while(1) {
		printf("Enter userId of new record : ");
		scanf("%s",userId);
		utype = user_type(userId);
		if(utype != 2 && utype != 3) {
			printf("Enter a valid userId\nMust start with either 'normal' or 'joint'\n");
			memset(userId,0,64);
		}
		else break;
	}
	printf("Enter password of new record : ");
	scanf("%s",password);
	write(sfd,&utype,sizeof(int));
	if(utype == 2) {
		normal record;
		strcpy(record.userId,userId);
		strcpy(record.password,password);
		printf("Enter name of account holder : ");
		scanf("%s",record.name);
		printf("Enter account number assigned : ");
		scanf("%s",record.accountNumber);
		write(sfd,&record,sizeof(normal));
	}	
	else if(utype == 3) {
		joint record;
		strcpy(record.userId1,userId);
		strcpy(record.password,password);
		printf("Enter name of account holder1 : ");
		scanf("%s",record.name1);
		printf("Enter account number assigned : ");
		scanf("%s",record.accountNumber);
		printf("Enter userId of account holder2(should start with joint) : ");
		scanf("%s",record.userId2);
		printf("Enter name of account holder2 : ");
		scanf("%[^\n]",record.name2);
		write(sfd,&record,sizeof(joint));
	}
	read(sfd,&flag,sizeof(int));
	success(flag);
	return;
}

//when user type = 1 i.e. admin delete  account function
void deleteAccount(int sfd) {
	printf("Enter userId to be deleted\n");
	char userId[64] = {0};
	scanf("%s",userId);
	int utype = user_type(userId);
	printf("%d\n",utype);
	if(utype != 2 && utype != 3) 
		utype = 0;	
	int flag;
	write(sfd,&utype,sizeof(int));
	write(sfd,userId,64);
	read(sfd,&flag,sizeof(int));
	success(flag);
}

void modifyAccount(int sfd) {
	char userId[64];
	printf("Enter UserId to search for\n");
	scanf("%s",userId);
	int utype = user_type(userId),flag=0;
	if(utype != 2 && utype != 3) 
		utype == 0;
	write(sfd,&utype,sizeof(int));
	write(sfd,userId,64);
	if(utype == 2) {
		normal record;
		printf("Enter userID : ");
		scanf("%s",record.userId);
		printf("Enter password : ");
		scanf("%s",record.password);
		printf("Enter name of account holder : ");
		scanf("%s",record.name);
		printf("Enter account number assigned : ");
		scanf("%s",record.accountNumber);
		write(sfd,&record,sizeof(normal));
	}	
	else if(utype == 3) {
		joint record;
		printf("Enter userID of holder1 : ");
		scanf("%s",record.userId1);
		printf("Enter password: ");
		scanf("%s",record.password);
		printf("Enter name of account holder1 : ");
		scanf("%s",record.name1);
		printf("Enter account number assigned : ");
		scanf("%s",record.accountNumber);
		printf("Enter userId of account holder2(should start with joint) : ");
		scanf("%s",record.userId2);
		printf("Enter name of account holder2 : ");
		scanf("%s",record.name2);
		write(sfd,&record,sizeof(joint));
	}
	read(sfd,&flag,sizeof(int));
	success(flag);
}

void searchAccount(int sfd) {
	char userId[64];
	printf("Enter UserId to search for\n");
	scanf("%s",userId);
	int utype = user_type(userId),flag=0;
	if(utype != 2 && utype != 3) 
		utype == 0;
	write(sfd,&utype,sizeof(int));
	write(sfd,userId,64);
	read(sfd,&flag,sizeof(int));
	success(flag);
	return;
}

void deposit(int sfd){
	int amount,  flag;
	printf("Entered the amount to be deposited\n");
	scanf("%d",&amount);
	if(amount < 0) {
		amount = 0;
		printf("Effective amount being changed to 0\n");
	}
	write(sfd,&amount,sizeof(int));
	read(sfd,&flag,sizeof(int)); 
	if(flag >= 0)
		printf("Balance after deposit %d\n",flag);
	else
		printf("Error occured while deposit\n");
	return;
}

void withdraw(int sfd) {
	int amount, flag;
	printf("Select amount to withdraw\n");
	scanf("%d",&amount);
	if(amount < 0) {
		amount = 0;
		printf("Effective amount being changed to 0\n");
	}
	write(sfd,&amount,sizeof(int));
	read(sfd,&flag,sizeof(int)); 
	if(flag >= 0)
		printf("Balance after withdraw %d\n",flag);
	else
		printf("Not enough funds\n");
	return;
}

void balanceEnquiry(int sfd){
	int flag;
	read(sfd,&flag,sizeof(int));
	if(flag >= 0)
		printf("Balance available is %d\n",flag);
	else 
		printf("Query failed\n");
	return;
}

void changePassword(int sfd){
	char newPassword[64];
	int flag;
	printf("Enter the new password : ");
	scanf("%s",newPassword);
	write(sfd,newPassword,64);
	read(sfd,&flag,sizeof(int));
	if(!flag){
		printf("Password update failed\n");
	}
	else{
		printf("Password updated successfully\n");
	}
	return;
}


void viewDetails(int sfd, int utype){
	if(utype == 2){
		normal record;
		read(sfd,&record,NORMAL_RECORD_SIZE);
		printf("Name : %s\n",record.name);
		printf("UserId : %s\n",record.userId);
		printf("Account Number : %s\n",record.accountNumber);
	}
	else if(utype == 3){
		joint record;
		read(sfd,&record,JOINT_RECORD_SIZE);
		printf("Name a/c holder 1 : %s\n",record.name1);
		printf("Name a/c holder 2 : %s\n",record.name2);
		printf("UserId a/c holder 1 : %s\n",record.userId1);
		printf("UserId a/c holder 2 : %s\n",record.userId2);
		printf("Account Number : %s\n",record.accountNumber);
	}
	return;
}

void query(int sfd, int utype, int chose) {
	write(sfd, &chose, sizeof(int));
	if(chose == 5) {
		printf("Session Ending\n");
	}
	else if(utype == 1) {
		switch(chose) {
			case 1:
				addAccount(sfd);
				break;
			case 2:
				deleteAccount(sfd);
				break;
			case 3:
				modifyAccount(sfd);
				break;
			case 4:
				searchAccount(sfd);
				break;
			case 5:
				printf("Session Ending\n");
				break;
		}
	}
	else if(utype == 2 || utype == 3) {
		switch(chose) {
			case 0:
				deposit(sfd);
				break;
			case 1:
				withdraw(sfd);
				break;
			case 2:
				balanceEnquiry(sfd);
				break;
			case 3:
				changePassword(sfd);
				break;
			case 4:
				viewDetails(sfd, utype);
				break;
			case 5:
				printf("Session Ending\n");
				break;			
		}
	}
}

int main() {
	int err;
	//Setting up socket
	struct sockaddr_in server;
	int sfd = socket(AF_UNIX,SOCK_STREAM,0);
	server.sin_family = AF_UNIX;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port=htons(6767);
	err = connect(sfd,(struct sockaddr*)&server,sizeof(server));
	if(err == -1) {
		printf("Connection failed\nTry later\n");
	       	return -1;	
	}
	//Socket setup completed
	int utype = login(sfd);
	if(!utype)
		return -1;
	while(1) {
		int chose = option(utype);
		if(chose == -1)
			continue;
		query(sfd, utype, chose);		
		if(chose == 5)
			return 0;
	}
}
