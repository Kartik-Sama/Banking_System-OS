#ifndef RECORDS_HEADER
#define RECORDS_HEADER
typedef struct admin {
	char userId[64];
	char password[64];
} admin;
typedef struct normal {
	char name[64];
	char userId[64];
	char password[64];
	char accountNumber[12];
} normal;
typedef struct joint {
	char name1[64];
	char name2[64];
	char userId1[64];
	char userId2[64];
	char password[64];
	char accountNumber[12];
} joint;
typedef struct fund {
	char accountNumber[12];
	unsigned int balance;
} fund;
#define ADMIN_RECORD_SIZE 128
#define NORMAL_RECORD_SIZE 204
#define JOINT_RECORD_SIZE 332
#define FUND_RECORD_SIZE 16
#endif
