#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include<errno.h>
#include<sys/types.h>


int tokenise(char command[], char *tokens[], const char* delim){
	char **new_tokens = tokens;
	int i = 0;
	char *token = strtok(command,delim);
	new_tokens[0] = token;
	i = 1;
	while(token!=NULL)
	{
		token = strtok(NULL,delim);
		new_tokens[i] = token;
		i++;
	}
	new_tokens[i] = NULL;
	return i;
}

int main()
{
	while(1)
	{
		pid_t pid;
		//pid = fork();
		// Variables
		char *uname = getlogin();
		char sys[1024];
		char buff[130];
		char *curr_dir = getcwd(buff,129);
		int i=0;
		
		// Built In Functions
		gethostname(sys,1024);
		//printf("%s\n",curr_dir);
		char *com =  (char*)malloc(100);
		printf("<%s@%s:%s>",uname,sys,curr_dir);
		fgets(com,130,stdin);
		com[strlen(com) - 1] = '\0';
		//printf("%c %c\n",com[strlen(com) - 1],com[strlen(com) - 2 ]);
		char* word;
		char* command[5];
		int a=0;
		int count = tokenise(com,command,";");
		
		for(a=0;a<count-1;a++)
		{
			char *final[10];
			//printf("AT 54: %s\n",command[a] );
			int nos = tokenise(command[a],final," ");//TOKENISE ??????
			//printf("AT 56: %s\n",command[a] );
			int i;
			pid_t pid;

			pid = fork();
			if(pid<0)
			{
				perror("Child not Created");
				_exit(-1);
			}
			else if(pid>0)
			{
				if(strcmp(final[0],"cd")==0)
				{
					//printf("dir %s\n",final[1]);					
					chdir(final[1]);						

				}
			}
			// else
			// 	_exit(0);
			if(pid == 0)
			{				
				//printf("%s\n",final[0] );
				if(strcmp(final[0],"pwd")==0)
				{
					curr_dir = getcwd(buff,129);
					printf("%s\n",curr_dir );
				}
				else if(strcmp(final[0],"echo")==0)
				{
					int x;
					if(nos>2)
					{
						printf("%s",final[1]);
					}
					for(x=2;x<nos-1;x++)
					{
						printf(" %s",final[x]);
					}
					printf("\n");					
					
				}
				else if(strcmp(final[0],"pinfo")==0)
				{	
					char arg[10];
					FILE  *file;
					char str[600];			
					if(final[1]==NULL)
					{
						strcpy(arg,"self");	
						//printf("Inside IF\n");					
					}
					else
					{
						strcpy(arg,final[1]);
					}					
					char path[200]="/proc/";
					strcat(path,arg);					
					strcat(path,"/status");
					//printf("%s\n", path);
					//file = fopen(path, "r");
					if (file == NULL)
					{
					    perror("fopen() failed");
					        
					}
					else
					{
						int newlines = 0;
						while (fgets(str,600,file)) 
						{
						printf("%s\n",str );
						newlines++;    					
        				if (newlines == 1)
            					printf("%s\n",str );
    					}
						
					}
					fclose(file);

				}
				else
				{
					int ret=0;
					ret=execvp(final[0],final);
					if(ret<0)
					{
						perror("Could not execute command");
						_exit(-1);
					}
				}
				_exit(0);
			}
			
			if(strcmp(final[nos-2],"&")!=0)
			{
				wait();
			}
			
		}
		//free(curr_dir);
	}
}