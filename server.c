#include<stdio.h>
#include<fcntl.h>
#include<pthread.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#include"records.h"

int searchRecord(char* file, int utype, char* userId) {
	int fd = open(file, O_RDONLY, 0744), found = 0;
	struct flock lock;
	lock.l_pid = getpid();
	lock.l_whence = SEEK_SET;
	int recordNumber = 0, bytes_read = ADMIN_RECORD_SIZE;
	if(utype == 1) {
		admin record;
		lock.l_len = ADMIN_RECORD_SIZE;
		while(bytes_read) {
			lock.l_type = F_RDLCK;
			lock.l_start = recordNumber*ADMIN_RECORD_SIZE;
			fcntl(fd,F_SETLKW,&lock); //Record read lock
			bytes_read = read(fd,&record,ADMIN_RECORD_SIZE);
			lock.l_type = F_UNLCK;
			fcntl(fd,F_SETLK,&lock); //Record read unlock;
			if(!strcmp(record.userId,userId)) {
				found = 1;
				break;
			}
			recordNumber++;
		}
	}
	else if(utype == 2) {
		normal record;
		lock.l_len = NORMAL_RECORD_SIZE;
		while(bytes_read) {
			lock.l_type = F_RDLCK;
			lock.l_start = recordNumber*NORMAL_RECORD_SIZE;
			fcntl(fd,F_SETLKW,&lock); //Record read lock
			bytes_read = read(fd,&record,NORMAL_RECORD_SIZE);
			lock.l_type = F_UNLCK;
			fcntl(fd,F_SETLK,&lock); //Record read unlock;
			if(!strcmp(record.userId,userId)) {
				found = 1;
				break;
			}
			recordNumber++;
		}
	}
	else if(utype == 3) {
		joint record;
		lock.l_len = JOINT_RECORD_SIZE;
		while(bytes_read) {
			lock.l_type = F_RDLCK;
			lock.l_start = recordNumber*JOINT_RECORD_SIZE;
			fcntl(fd,F_SETLKW,&lock); //Record read lock
			bytes_read = read(fd,&record,JOINT_RECORD_SIZE);
			lock.l_type = F_UNLCK;
			fcntl(fd,F_SETLK,&lock); //Record read unlock;
			if(!strcmp(record.userId1,userId) || !strcmp(record.userId2,userId)) {
				found = 1;
				break;
			}
			recordNumber++;
		}

	}
	else if(utype == 0) { //To search for account number and balance
		fund record;
		lock.l_len = FUND_RECORD_SIZE;
		while(bytes_read) {
			lock.l_type = F_RDLCK;
			lock.l_start = recordNumber*FUND_RECORD_SIZE;
			fcntl(fd,F_SETLKW,&lock); //Record read lock
			bytes_read = read(fd,&record,FUND_RECORD_SIZE);
			lock.l_type = F_UNLCK;
			fcntl(fd,F_SETLK,&lock); //Record read unlock;
			if(!strcmp(record.accountNumber,userId)){
				found = 1;
				break;
			}
			recordNumber++;
		}

	}
	if(found)
		return recordNumber;
	close(fd);
	return -1;
}

int searchAccount(char* accountNumber) {
	int fd = open("./fund.dat", O_RDONLY, 0744), found = 0, recordNumber = 0, bytes_read = 1;
	struct flock lock;
	lock.l_pid = getpid();
	lock.l_whence = SEEK_SET;
	fund record;
	lock.l_len = FUND_RECORD_SIZE;
	while(bytes_read) {
		lock.l_type = F_RDLCK;
		lock.l_start = recordNumber*FUND_RECORD_SIZE;
		fcntl(fd,F_SETLKW,&lock); //Record read lock
		bytes_read = read(fd,&record,FUND_RECORD_SIZE);
		lock.l_type = F_UNLCK;
		fcntl(fd,F_SETLK,&lock); //Record read unlock;
		if(!strcmp(record.accountNumber,accountNumber)){
			found = 1;
			break;
		}
		recordNumber++;
	}
	if(found)
		return recordNumber;
	close(fd);
	return -1;
}

void getfile(char* file, int utype) {
	if(utype == 1) 
		strcpy(file,"admin.dat");
	else if(utype == 2)
		strcpy(file,"normal.dat");
	else if(utype == 3)
		strcpy(file,"joint.dat");
}

int checkLogin(int utype, char* userId, char* password) {
	char file[20];
	getfile(file,utype);
        struct flock lock;
        lock.l_pid = getpid();
        lock.l_whence = SEEK_SET;
	lock.l_type = F_RDLCK;
	int recordNumber = searchRecord(file, utype, userId);
	int fd = open(file, O_RDONLY, 0744);
	if(recordNumber == -1)
		return -1;
	printf("%d\n",recordNumber);
	if(utype == 1) {
		admin record;
		lock.l_len = ADMIN_RECORD_SIZE;
		lock.l_start = ADMIN_RECORD_SIZE*recordNumber;
		lseek(fd,lock.l_start,SEEK_SET);
		fcntl(fd,F_SETLKW,&lock); //Record read lock
		int bytes_read = read(fd,&record,ADMIN_RECORD_SIZE);
		lock.l_type = F_UNLCK;
		fcntl(fd,F_SETLK,&lock); //Record read unlock
		if(!strcmp(password,record.password)) {
			close(fd);
			return recordNumber;
		}
	}
	else if(utype == 2) {
		normal record;
		lock.l_len = NORMAL_RECORD_SIZE;
		lock.l_start = NORMAL_RECORD_SIZE*recordNumber;
		lseek(fd,lock.l_start,SEEK_SET);
		fcntl(fd,F_SETLKW,&lock); //Record read lock
		int bytes_read = read(fd,&record,NORMAL_RECORD_SIZE);
		lock.l_type = F_UNLCK;
		fcntl(fd,F_SETLK,&lock); //Record read unlock
		if(!strcmp(password,record.password)) {
			close(fd);
			return recordNumber;
		}
	}
	else if(utype == 3) {
		joint record;
		lock.l_len = JOINT_RECORD_SIZE;
		lock.l_start = JOINT_RECORD_SIZE*recordNumber;
		lseek(fd,lock.l_start,SEEK_SET);
		fcntl(fd,F_SETLKW,&lock); //Record read lock
		int bytes_read = read(fd,&record,JOINT_RECORD_SIZE);
		lock.l_type = F_UNLCK;
		fcntl(fd,F_SETLK,&lock); //Record read unlock
		if(!strcmp(password,record.password)) {
			close(fd);
			return recordNumber;
		}

	}
	close(fd);
	return -1;
}


void addUser(int ffd) {
	int utype;
	char file[20];
	read(ffd,&utype,sizeof(int));
	getfile(file, utype);
	int fd = open(file,O_WRONLY,0744);
	int bytes_written, flag;
	lseek(fd,0,SEEK_END);
        struct flock lock = {0};
        lock.l_pid = getpid();
        lock.l_whence = SEEK_END;
	lock.l_type = F_WRLCK;
	char accountNum[13];
	if(utype == 2) {
		lock.l_len = NORMAL_RECORD_SIZE;
		normal record;
		read(ffd,&record,NORMAL_RECORD_SIZE);
		fcntl(fd,F_SETLKW,&lock);
		bytes_written = write(fd,&record,NORMAL_RECORD_SIZE);
		lock.l_type = F_UNLCK;
		fcntl(fd,F_SETLK,&lock);
		flag = (bytes_written == NORMAL_RECORD_SIZE);
		strcpy(accountNum,record.accountNumber);
	}
	else if(utype == 3) {
		lock.l_len = JOINT_RECORD_SIZE;
		joint record;
		read(ffd,&record,JOINT_RECORD_SIZE);
		fcntl(fd,F_SETLKW,&lock);
		bytes_written = write(fd,&record,JOINT_RECORD_SIZE);
		lock.l_type = F_UNLCK;
		fcntl(fd,F_SETLK,&lock);
		flag = (bytes_written == JOINT_RECORD_SIZE);
		strcpy(accountNum,record.accountNumber);
	}
	close(fd);
	fund newRec;
	strcpy(newRec.accountNumber,accountNum);
	newRec.balance = 1000; //Initial balance for any new account
	fd = open("./fund.dat",O_WRONLY,0744);
	lseek(fd,0,SEEK_END);
	bytes_written = write(fd, &newRec, FUND_RECORD_SIZE);
	flag = flag & (bytes_written == FUND_RECORD_SIZE);
	close(fd);	
	if(flag) 
		printf("New record added to database\n");
	write(ffd, &flag, sizeof(int));
	return;
}

void deleteUser(int ffd) {
	char file[20], userId[64] = {0}, accountNum[12] = {0};
	int utype,flag = 0;
	read(ffd,&utype,sizeof(int));
	read(ffd,userId,64);
	if(utype == 0) {
		write(ffd,&flag,sizeof(int));
		return;
	}
	getfile(file, utype);
	int recordNum = searchRecord(file, utype, userId);
	if(recordNum == -1) {
		write(ffd,&flag,sizeof(int));
		return;
	}
		
	int fd = open(file,O_RDWR,0744);
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_pid = getpid();
	if(utype == 2) {
		normal record = {0};
		lock.l_start = NORMAL_RECORD_SIZE*recordNum;
		lock.l_len = NORMAL_RECORD_SIZE;
		lseek(fd, lock.l_start, SEEK_SET);
		fcntl(fd,F_SETLKW,&lock);
		read(fd, &record, NORMAL_RECORD_SIZE);
		strcpy(accountNum,record.accountNumber);
		memset(&record,0,NORMAL_RECORD_SIZE);
		lseek(fd, lock.l_start, SEEK_SET);	
		write(fd, &record, NORMAL_RECORD_SIZE);
		lock.l_type = F_UNLCK;
		fcntl(fd,F_SETLK,&lock);
	}
	else if(utype == 3) {
		joint record = {0};
		lock.l_start = JOINT_RECORD_SIZE*recordNum;
		lock.l_len = JOINT_RECORD_SIZE;
		lseek(fd, lock.l_start, SEEK_SET);
		fcntl(fd,F_SETLKW,&lock);
		read(fd, &record, JOINT_RECORD_SIZE);
		strcpy(accountNum,record.accountNumber);
		memset(&record,0,JOINT_RECORD_SIZE);
		lseek(fd, lock.l_start, SEEK_SET);	
		write(fd, &record, JOINT_RECORD_SIZE);
		lock.l_type = F_UNLCK;
		fcntl(fd,F_SETLK,&lock);

	}
        close(fd);
	fd = open("fund.dat",O_WRONLY,0744);
	recordNum = searchAccount(accountNum);
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_pid = getpid();
	lock.l_start = FUND_RECORD_SIZE*recordNum;
	lock.l_len = FUND_RECORD_SIZE;
	fcntl(fd, F_SETLKW, &lock);
	fund delRec = {0};
	lseek(fd,lock.l_start,SEEK_SET);
	write(fd, &delRec, FUND_RECORD_SIZE);
	lock.l_type = F_UNLCK;
	close(fd);
	flag = 1;
	write(ffd,&flag,sizeof(int));
	if(flag)
		printf("Deleting user record succesful\n");
	return;
}

void modifyUser(int ffd) {
	char file[20], userId[64] = {0};
	int utype,flag = 0;
	read(ffd,&utype,sizeof(int));
	read(ffd,userId,64);
	if(utype == 0) {
		write(ffd,&flag,sizeof(int));
		return;
	}
	getfile(file, utype);
	int recordNum = searchRecord(file, utype, userId);
	if(recordNum == -1) {
		write(ffd,&flag,sizeof(int));
		return;
	}
	int fd = open(file,O_RDWR,0744);
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_pid = getpid();
	if(utype == 2) {
		normal record = {0};
		read(ffd,&record,NORMAL_RECORD_SIZE);
		lock.l_start = NORMAL_RECORD_SIZE*recordNum;
		lock.l_len = NORMAL_RECORD_SIZE;
		lseek(fd, lock.l_start, SEEK_SET);
		fcntl(fd,F_SETLKW,&lock);
		write(fd, &record, NORMAL_RECORD_SIZE);
		lock.l_type = F_UNLCK;
		fcntl(fd,F_SETLK,&lock);
	}
	else if(utype == 3) {
		joint record = {0};
		read(ffd,&record,JOINT_RECORD_SIZE);
		lock.l_start = JOINT_RECORD_SIZE*recordNum;
		lock.l_len = JOINT_RECORD_SIZE;
		lseek(fd, lock.l_start, SEEK_SET);
		fcntl(fd,F_SETLKW,&lock);
		write(fd, &record, JOINT_RECORD_SIZE);
		lock.l_type = F_UNLCK;
		fcntl(fd,F_SETLK,&lock);

	}
	flag = 1;
        close(fd);
	write(ffd,&flag,sizeof(int));
	if(flag)
		printf("Modifying user record successful\n");
	return;
}

int searchUser(int ffd) {
	char file[20], userId[64] = {0};
        int utype,flag = 0;
        read(ffd,&utype,sizeof(int));
        read(ffd,userId,64);
        if(utype == 0) {
                write(ffd,&flag,sizeof(int));
                return -1;
        }
        getfile(file, utype);
	int recordNumber = searchRecord(file, utype, userId);
	flag = (recordNumber != -1);
	if(flag)
		printf("User record found\n");
	write(ffd,&flag,sizeof(int));
	return -1;
}

void withdraw(int ffd, int record_num, int utype) {
	char file[20];
	getfile(file,utype);
	int fd = open(file,O_RDONLY,0744);
	char accountNum[12] = {0};
	struct flock lock;	
	lock.l_type = F_RDLCK;
	lock.l_whence = SEEK_SET;
	lock.l_pid = getpid();
	if(utype == 2) {
		normal record;
		lock.l_start = NORMAL_RECORD_SIZE*record_num;
		lock.l_len = NORMAL_RECORD_SIZE;
		lseek(fd,lock.l_start,SEEK_SET);
		fcntl(fd,F_SETLKW,&lock);
		read(fd,&record,NORMAL_RECORD_SIZE);
		lock.l_type = F_UNLCK;
		fcntl(fd,F_SETLK,&lock);
		strcpy(accountNum,record.accountNumber);	
	}
	else if(utype == 3) {
		joint record;
		lock.l_start = JOINT_RECORD_SIZE*record_num;
		lock.l_len = JOINT_RECORD_SIZE;
		lseek(fd,lock.l_start,SEEK_SET);
		fcntl(fd,F_SETLKW,&lock);
		read(fd,&record,JOINT_RECORD_SIZE);
		lock.l_type = F_UNLCK;
		fcntl(fd,F_SETLK,&lock);
		strcpy(accountNum,record.accountNumber);	

	}
	close(fd);
	int recordNumber = searchAccount(accountNum);
	int amount_sub,flag=-1,bytes_written=-1;
	read(ffd,&amount_sub,sizeof(int));
	fd = open("./fund.dat",O_RDWR,0744);
	lock.l_type = F_WRLCK;
	lock.l_start = FUND_RECORD_SIZE*recordNumber;
	lock.l_len =  FUND_RECORD_SIZE;
	lseek(fd,lock.l_start,SEEK_SET);
	fund record;
	fcntl(fd,F_SETLKW,&lock);
	read(fd,&record,FUND_RECORD_SIZE);
	if(record.balance >= amount_sub) {
		record.balance -= amount_sub;
		lseek(fd,lock.l_start,SEEK_SET);
		write(fd,&record,FUND_RECORD_SIZE);
	}
	lock.l_type = F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
	close(fd);
	if(bytes_written) {
		flag = record.balance; //New balance being coveyed.
	}
	write(ffd,&flag,sizeof(int));
	return;
}

void deposit(int ffd, int record_num, int utype) {
	char file[20];
	getfile(file,utype);
	int fd = open(file,O_RDONLY,0744);
	char accountNum[12] = {0};
	struct flock lock;	
	lock.l_type = F_RDLCK;
	lock.l_whence = SEEK_SET;
	lock.l_pid = getpid();
	if(utype == 2) {
		normal record;
		lock.l_start = NORMAL_RECORD_SIZE*record_num;
		lock.l_len = NORMAL_RECORD_SIZE;
		lseek(fd,lock.l_start,SEEK_SET);
		fcntl(fd,F_SETLKW,&lock);
		read(fd,&record,NORMAL_RECORD_SIZE);
		lock.l_type = F_UNLCK;
		fcntl(fd,F_SETLK,&lock);
		strcpy(accountNum,record.accountNumber);	
	}
	else if(utype == 3) {
		joint record;
		lock.l_start = JOINT_RECORD_SIZE*record_num;
		lock.l_len = JOINT_RECORD_SIZE;
		lseek(fd,lock.l_start,SEEK_SET);
		fcntl(fd,F_SETLKW,&lock);
		read(fd,&record,JOINT_RECORD_SIZE);
		lock.l_type = F_UNLCK;
		fcntl(fd,F_SETLK,&lock);
		strcpy(accountNum,record.accountNumber);	

	}
	close(fd);
	printf("%s\n",accountNum);
	int recordNumber = searchAccount(accountNum);
	printf("record %d\n",recordNumber);
	int amount_add,flag=-1;
	read(ffd,&amount_add,sizeof(int));
	fd = open("./fund.dat",O_RDWR,0744);
	lock.l_type = F_WRLCK;
	lock.l_start = FUND_RECORD_SIZE*recordNumber;
	lock.l_len =  FUND_RECORD_SIZE;
	lseek(fd,lock.l_start,SEEK_SET);
	printf("rec - %d\n",recordNumber);
	fund record;
	fcntl(fd,F_SETLKW,&lock);
	read(fd,&record,FUND_RECORD_SIZE);
	record.balance += amount_add;
	lseek(fd,lock.l_start,SEEK_SET);
	int bytes_written = write(fd,&record,FUND_RECORD_SIZE);
	lock.l_type = F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
	close(fd);
	if(bytes_written) {
		flag = record.balance; //New balance being coveyed.
	}
	write(ffd,&flag,sizeof(int));
	return;
}

void balanceEnquiry(int ffd, int record_num, int utype) {
	printf("Hi BE\n");
	char file[20];
	getfile(file,utype);
	int fd = open(file,O_RDONLY,0744);
	char accountNum[12]={0};
	struct flock lock;	
	lock.l_type = F_RDLCK;
	lock.l_whence = SEEK_SET;
	lock.l_pid = getpid();
	if(utype == 2) {
		normal record;
		lock.l_start = NORMAL_RECORD_SIZE*record_num;
		lock.l_len = NORMAL_RECORD_SIZE;
		lseek(fd,lock.l_start,SEEK_SET);
		fcntl(fd,F_SETLKW,&lock);
		read(fd,&record,NORMAL_RECORD_SIZE);
		lock.l_type = F_UNLCK;
		fcntl(fd,F_SETLK,&lock);
		strcpy(accountNum,record.accountNumber);	
	}
	else if(utype == 3) {
		joint record;
		lock.l_start = JOINT_RECORD_SIZE*record_num;
		lock.l_len = JOINT_RECORD_SIZE;
		lseek(fd,lock.l_start,SEEK_SET);
		fcntl(fd,F_SETLKW,&lock);
		read(fd,&record,JOINT_RECORD_SIZE);
		lock.l_type = F_UNLCK;
		fcntl(fd,F_SETLK,&lock);
		strcpy(accountNum,record.accountNumber);	

	}
	close(fd);
	int recordNumber = searchAccount(accountNum);
	int amount_add,flag=-1;
	fd = open("./fund.dat",O_RDWR,0744);
	lock.l_type = F_RDLCK;
	lock.l_start = FUND_RECORD_SIZE*recordNumber;
	lock.l_len =  FUND_RECORD_SIZE;
	lseek(fd,lock.l_start,SEEK_SET);
	fund record;
	fcntl(fd,F_SETLKW,&lock);
	int bytes_read = read(fd,&record,FUND_RECORD_SIZE);
	lseek(fd,lock.l_start,SEEK_SET);
	lock.l_type = F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
	close(fd);
	if(bytes_read)
		flag = record.balance; //Current balance being coveyed.
	write(ffd,&flag,sizeof(int));
	return;
}

void changePassword(int ffd, int record_num, int utype) {
	char file[20], password[64] = {0};
	getfile(file,utype);
	int fd = open(file,O_RDWR,0744), bytes_written = 0;
	struct flock lock;	
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_pid = getpid();
	read(ffd,password,64);
	if(utype == 2) {
		normal record;
		lock.l_start = NORMAL_RECORD_SIZE*record_num;
		lock.l_len = NORMAL_RECORD_SIZE;
		lseek(fd,lock.l_start,SEEK_SET);
		fcntl(fd,F_SETLKW,&lock);
		read(fd,&record,NORMAL_RECORD_SIZE);
		memset(record.password,0,64);
		strcpy(record.password,password);
		lseek(fd,lock.l_start,SEEK_SET);
		bytes_written = write(fd,&record,NORMAL_RECORD_SIZE);
		lock.l_type = F_UNLCK;
		fcntl(fd,F_SETLK,&lock);
	}
	else if(utype == 3) {
		joint record;
		lock.l_start = JOINT_RECORD_SIZE*record_num;
		lock.l_len = JOINT_RECORD_SIZE;
		lseek(fd,lock.l_start,SEEK_SET);
		fcntl(fd,F_SETLKW,&lock);
		read(fd,&record,JOINT_RECORD_SIZE);
		memset(record.password,0,64);
		strcpy(record.password,password);
		lseek(fd,lock.l_start,SEEK_SET);
		bytes_written = write(fd,&record,JOINT_RECORD_SIZE);
		lock.l_type = F_UNLCK;
		fcntl(fd,F_SETLK,&lock);
	}
	close(fd);
	write(ffd,&bytes_written,sizeof(int));
	return;
}	

void viewDetails(int ffd, int record_num, int utype) {
	char file[20];
	getfile(file,utype);
	int fd = open(file,O_RDWR,0744);
	struct flock lock;	
	lock.l_type = F_RDLCK;
	lock.l_whence = SEEK_SET;
	lock.l_pid = getpid();
	if(utype == 2) {
		normal record;
		lock.l_start = NORMAL_RECORD_SIZE*record_num;
		lock.l_len = NORMAL_RECORD_SIZE;
		lseek(fd,lock.l_start,SEEK_SET);
		fcntl(fd,F_SETLKW,&lock);
		read(fd,&record,NORMAL_RECORD_SIZE);
		lock.l_type = F_UNLCK;
		fcntl(fd,F_SETLK,&lock);
		write(ffd,&record,NORMAL_RECORD_SIZE);
	}
	else if(utype == 3) {
		joint record;
		lock.l_start = JOINT_RECORD_SIZE*record_num;
		lock.l_len = JOINT_RECORD_SIZE;
		lseek(fd,lock.l_start,SEEK_SET);
		fcntl(fd,F_SETLKW,&lock);
		read(fd,&record,JOINT_RECORD_SIZE);
		lock.l_type = F_UNLCK;
		fcntl(fd,F_SETLK,&lock);
		write(ffd,&record,JOINT_RECORD_SIZE);
	}
	close(fd);
	return;
}

void* clientHandler(void* ffd_ptr) {
	int ffd = *((int *)ffd_ptr);
	int utype;
	char userId[64] = {0}, password[64] = {0},file[20] = {0};
	read(ffd,&utype,sizeof(int));
	read(ffd,userId,64);
	read(ffd,password,64);
	if(!utype) {
		printf("Invalid user\n");
		return NULL;
	}
	getfile(file,utype);
	int chose;
	int login_record = checkLogin(utype,userId,password);
	int login_successfull = (login_record >= 0) ? 1 : 0;
	write(ffd,&login_successfull,sizeof(int));
	if(!login_successfull)
		return NULL;
	while(1) {
		read(ffd,&chose,sizeof(int));
		printf("choose %d\n",chose);
		if(chose == 0) {
			deposit(ffd,login_record,utype);
		}
		else if(chose == 1) {
			if(utype == 1) 
				addUser(ffd);
			else if(utype == 2 || utype == 3)
				withdraw(ffd,login_record,utype);
		}
		else if(chose == 2) {
			if(utype == 1)
				deleteUser(ffd);	
			else if(utype == 2 || utype == 3)
				balanceEnquiry(ffd,login_record,utype);
		}
		else if(chose == 3) {
			if(utype == 1)
				modifyUser(ffd);		
			else if(utype == 2 || utype == 3)
				changePassword(ffd,login_record,utype);
		}
		else if(chose == 4) {
			if(utype == 1)
				searchUser(ffd);
			else if(utype == 2 || utype == 3)
				viewDetails(ffd,login_record,utype);
		}
		else if(chose == 5) {
			printf("Client session ended\n");
			return NULL;
		}
	}
}

int main() {
	int err;
	//Setting up socket
	struct sockaddr_in server,client;
	int sfd = socket(AF_UNIX, SOCK_STREAM, 0); //Within system socket communication
	server.sin_family = AF_UNIX;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(6767);
	err = bind(sfd,(struct sockaddr *)&server,sizeof(server)); //bind step of server
	if(err == -1) {
		printf("Binding error\n");
		return -1;
	}
	err = listen(sfd,5); //listen step max client backlog of 5 in queue
	if(err == -1) {
		printf("Listening error\n");
		return -1;
	}
	unsigned int sockaddr_len = sizeof(struct sockaddr);
	//Socket set up completed
	printf("Server Active\n");
	while(1){
        	int ffd = accept(sfd, (struct sockaddr *)&client, &sockaddr_len);
	        if(ffd >= 0){
			printf("New client connected\n");
        	    	pthread_t threadId;
            		pthread_create(&threadId, NULL, clientHandler, (void*)&ffd);
        	}
		else 
			printf("Error in accepting new connection. Client queue might be full\n");

    	}
	close(sfd);
	return 0;
}
