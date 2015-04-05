#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include <unistd.h>

char hostName[1024];
int pipeFlag = 0;
char pipeFile[1024];

char * subString(char input[512], int start, int offset)
{
	static char sub[512];
	for(int i = 0;i<sizeof(sub)-1;i++)
	{
		sub[i]='\0';
	}
	strncpy(sub,&input[start],offset);
	return sub;
}

void flagWrite(char fileName[512])
{
	FILE *fileHandle;
	char text[2048];
	for(int i; i<sizeof(text);i++)
	{
		text[i] = '\0';
	}
	fileHandle=fopen(fileName,'w');
	for(int i;i<sizeof(text);i++)
	{
		if(text[i]=='\0')
		{
			return;
		}
		fputc((int)text[i],fileHandle);
	}
	fclose(fileHandle);
	return;
}

void writeFile(char fileName[512])
{
	printf("\nWould you like to append or rewrite? (a/w): \n");
	char choice[2]="\0\0";
	while(choice[0]!='a'&&choice[0]!='w')
	{
		choice[0]=(char)getchar();
	}
	FILE *fileHandle;
	char text[2048];
	for(int i; i<sizeof(text);i++)
	{
		text[i] = '\0';
	}
	getchar();
	printf("Type the line of text:\n");
	fgets(text,2047,stdin);
	if(choice[0]=='a')
	{
		fileHandle=fopen(fileName,"a");
		fprintf(fileHandle,text);
	}
	if(choice[0]=='w')
	{
		fileHandle=fopen(fileName,"w");
		fprintf(fileHandle,text);
	}
	fclose(fileHandle);
	return;
}

void readFile(char fileName[512])
{
	FILE *fileHandle;
	fileHandle=fopen(fileName,"r");
	char character;
	while((character=fgetc(fileHandle))!=EOF)
	{
		printf("%c",character);
	}
	fclose(fileHandle);
	return;
}

int checkSecurity(char dir[512])
{
	if(strcmp(dir,"..")==0||strcmp(dir,".")==0)
	{
		return 6;
	}
	for(int i=0; i<512;i++)
	{
		if(dir[i]=='/')
		{
			return 6;
		}
	}
	return 0;
}

int files()
{
	int ret = 0;
	if(pipeFlag==1)
	{
		ret = 8;
	}
	DIR *d;
	struct dirent *dir;
	d = opendir(".");
	if(d)
	{
		printf("Files:\n");
		printf("======\n");
		while((dir = readdir(d)) != NULL)
		{
			printf("[%s]\n",dir->d_name);
		}
		closedir(d);
	}
	else
	{
		return 3;
	}
	return ret;
}

int findFile(char file[512])
{
	DIR *d;
	struct dirent *dir;
	d = opendir(".");
	if(d)
	{
		while((dir = readdir(d)) != NULL)
		{
			if(strcmp(dir->d_name,file)==0)
			{
				return 0;
			}
		}
		closedir(d);
		return 4;
	}
	else
	{
		return 3;
	}
	return 5;
}

void shell()
{
	printf("          Shell Information\n");
	printf("        =====================\n\n");
	printf("        [LockDown Shell V1.0]\n");
	printf("            [Beta  Build]\n");
	printf("       [Designer - RIVAL |MAX]\n");
	printf("           [SITEOFMAX.COM]\n");
	printf("           [Written in: C]\n\n");
	printf("[  LockDown Shell is a minimal GNU  ]\n");
	printf("[ Linux shell for administrators to ]\n");
	printf("[      allow a user to perform      ]\n");
	printf("[           basic actions           ]\n\n");
}

void usage()
{
	printf("--------=[HELP]=--------\n\n");
	printf("Commands:\n");
	printf("HELP - What you are seeing now | 0 arguments\n");
	printf("SHEL - About the shell | 0 arguments\n");
	printf("FILE - Lists files | 0 arguments\n");
	printf("WRTE - Write text to file | 1 argument - File Name\n");
	printf("READ - Displays contents of text file | 1 argument - File Name\n");
	printf("QUIT - Exits the session | 0 arguments\n");
	printf("TIME - Gets the server's time | 0 arguments\n");
	printf("PIPE - Pipes the output of the following command | 1 argument - File Name\n");
	printf("RSCR - Runs a LockDown Shell Script | 1 argument - File Name\n");
	printf("HOST - Sets the host name for the session | 1 argument - Host Name\n\n");
	return;
}

int command_parser(char input[512])
{
	if(strcmp(input,"HELP")==0)
	{
		if(pipeFlag==1)
		{
			pipeFlag=0;
			int stdout_fd = dup(STDOUT_FILENO);
			freopen(pipeFile, "w", stdout);
			usage();
			fclose(stdout);
			dup2(stdout_fd, STDOUT_FILENO);
			stdout = fdopen(STDOUT_FILENO,"w");
			close(stdout_fd);
			return 8;
		}
		else
		{
			usage();
			return 0;
		}
	}
	if(strcmp(input,"SHEL")==0)
	{
		if(pipeFlag==1)
		{
			pipeFlag=0;
			int stdout_fd = dup(STDOUT_FILENO);
			freopen(pipeFile, "w", stdout);
			shell();
			fclose(stdout);
			dup2(stdout_fd, STDOUT_FILENO);
			stdout = fdopen(STDOUT_FILENO,"w");
			close(stdout_fd);
			return 8;
		}
		else
		{
		shell();
		}
		return 0;
	}
	if(strcmp(input,"FILE")==0)
	{
		if(pipeFlag==1)
		{
			pipeFlag=0;
			int error=8;
			int stdout_fd = dup(STDOUT_FILENO);
			freopen(pipeFile, "w", stdout);
			error = files();
			fclose(stdout);
			dup2(stdout_fd, STDOUT_FILENO);
			stdout = fdopen(STDOUT_FILENO,"w");
			close(stdout_fd);
			return error;
		}
		else
		{
			int error = files();
			return error;
		}
	}
	if(strcmp(subString(input,0,4),"WRTE")==0)
	{
		if(pipeFlag==1)
		{
			pipeFlag=0;
			return 7;
		}
		printf("\nSecurity Check... ");
		int securityCheck = checkSecurity(subString(input,5,strlen(input)));
		if(securityCheck!=0)
		{
			printf("Failed.\n\n");
			return securityCheck;
		}
		printf("Success!\n");
		printf("Finding file... ");
		int error = findFile(subString(input,5,strlen(input)));
		if(error!=0)
		{
			printf("Failed.\n\n");
			return error;
		}
		printf("Success!\n");
		writeFile(subString(input,5,strlen(input)));
		return 0;
	}
	if(strcmp(subString(input,0,4),"READ")==0)
	{
		if(pipeFlag==1)
		{
			pipeFlag=0;
			printf("\nSecurity Check... ");
			int securityCheck = checkSecurity(subString(input,5,strlen(input)));
			if(securityCheck!=0)
			{
				printf("Failed.\n\n");
				return securityCheck;
			}
			printf("Success!\n");
			printf("Finding file... ");
			int error = findFile(subString(input,5,strlen(input)));
			if(error!=0)
			{
				printf("Failed.\n\n");
				return error;
			}
			printf("Success!\n");
			int stdout_fd = dup(STDOUT_FILENO);
			freopen(pipeFile, "w", stdout);
			readFile(subString(input,5,strlen(input)));
			fclose(stdout);
			dup2(stdout_fd, STDOUT_FILENO);
			stdout = fdopen(STDOUT_FILENO,"w");
			close(stdout_fd);
			printf("\n");
			return 8;
		}
		else
		{
			printf("\nSecurity Check... ");
			int securityCheck = checkSecurity(subString(input,5,strlen(input)));
			if(securityCheck!=0)
			{
				printf("Failed.\n\n");
				return securityCheck;
			}
			printf("Success!\n");
			printf("Finding file... ");
			int error = findFile(subString(input,5,strlen(input)));
			if(error!=0)
			{
				printf("Failed.\n\n");
				return error;
			}
			printf("Success!\n");
			readFile(subString(input,5,strlen(input)));
		}
		printf("\n");
		return 0;
	}
	if(strcmp(input,"QUIT")==0)
	{
		exit(0);
	}
	if(strcmp(input,"TIME")==0)
	{
		time_t timer;
		struct tm * timei;
		time(&timer);
		timei = localtime(&timer);
		if(pipeFlag==1)
		{
			pipeFlag=0;
			int stdout_fd = dup(STDOUT_FILENO);
			freopen(pipeFile, "w", stdout);
			printf("The time is %s", asctime(timei));
			fclose(stdout);
			dup2(stdout_fd, STDOUT_FILENO);
			stdout = fdopen(STDOUT_FILENO,"w");
			close(stdout_fd);
			return 8;
		}
		else
		{
			printf("The time is %s", asctime(timei));
			return 0;
		}
	}
	if(strcmp(subString(input,0,4),"PIPE")==0)
	{
		if(pipeFlag==1)
		{
			pipeFlag=0;
			return 7;
		}
		int securityCheck = checkSecurity(subString(input,5,strlen(input)));
		if(securityCheck!=0)
		{
			return securityCheck;
		}
		int error = findFile(subString(input,5,strlen(input)));
		if(error!=0)
		{
			return error;
		}
		strcpy(pipeFile,subString(input,5,strlen(input)));
		pipeFlag=1;
		return 0;
	}
	if(strcmp(input,"RSCR")==0)
	{
		if(pipeFlag==1)
		{
			pipeFlag=0;
			return 7;
		}
		else
		{
			return 0;
		}
	}
	if(strcmp(subString(input,0,4),"HOST")==0)
	{
		if(pipeFlag==1)
		{
			pipeFlag=0;
			return 7;
		}
		for(int i; i<sizeof(hostName);i++)
		{
			hostName[i] = '\0';
		}
		strcpy(hostName,subString(input,5,strlen(input)));
		return 0;
	}
	return 1;
}


void console()
{
	while(1)
	{
		printf("[%s]> ",hostName);
		char input[512];
		for(int i; i<sizeof(input);i++)
		{
			input[i] = '\0';
		}
		fgets(input,512,stdin);
		if(input[strlen(input)-1]!='\0')
		{
			input[strlen(input)-1]='\0';
		}
		int i = command_parser(input);
		if(i==1)
		{
			printf("Command not recognised.\n");
		}
		if(i==2)
		{
			printf("Command cannot be piped.\n");
		}
		if(i==3)
		{
			printf("Could not get files listing.\n");
		}
		if(i==4)
		{
			printf("Could not find the file specified.\n");
		}
		if(i==5)
		{
			printf("Unknown Error.\n");
		}
		if(i==6)
		{
			printf("You don't have permission to access a file by this name.\n");
		}
		if(i==7)
		{
			printf("This command cannot be PIPED.\n");
		}
		if(i==8)
		{
			printf("PIPE successful!\n");
		}
	}
	return;
}

int main(int argc, char * argv[])
{
	for(int i; i<sizeof(hostName);i++)
	{
		hostName[i] = '\0';
	}
	for(int i; i<sizeof(pipeFile);i++)
	{
		pipeFile[i] = '\0';
	}
	strcpy(hostName,"DEFAULT");
	printf("Welcome to LockDown Shell!\n\nType HELP to view the commands\n\n");
	console();
	return 0;
}