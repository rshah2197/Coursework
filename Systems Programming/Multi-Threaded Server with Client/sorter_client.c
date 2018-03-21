#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include <ftw.h>
#include <pthread.h>
#include <errno.h>
#include "sorter_server.h"

#define SIZEOF_STRING 10000

int sockfd;
struct sockaddr_in servaddr;
int number_of_files=0;

int thread_index=0;
pthread_mutex_t m;
pthread_mutex_t socklock;

struct arg
{
	char* filePath;
	char* sortType;
};
struct s{
 int j;
 int i;
  int ii;
char* filename1;
char* type_global;
char c;

char* temp;
char* junk;
int a;
char *str;
int size;
int fileSize;
char* fname;
char* whole;
char* file;
int index1;
int index2;
pthread_t threads[10000];

};
struct s client;

void talk(char* fpath,char* type)
{

	pthread_mutex_lock(&socklock);
	//printf("talk\n");
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	fflush(stdout);
	char * sendline = malloc(SIZEOF_STRING);
 
    	if(connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) == -1)
	{
		printf("ERROR : Failed to connect to server [%s]\n",strerror(errno));
		pthread_mutex_unlock(&socklock);
		exit(EXIT_FAILURE);
	}

	client.filename1 = malloc(100);
	
	strncpy(client.filename1, fpath, strlen(fpath));
	client.filename1[strlen(fpath) + 1] = '\0';
//FileName	//printf("%s\n", client.filename1);
	FILE * fp = fopen(client.filename1, "r");
	int row_position = 0;
	client.j = 0;
	int file_count = 0;
	int i = 0;
	client.c = getc(fp);
	while (client.c != EOF)
	{
		if(client.c == '\n')
		{
			file_count++;
		}
		i++;
		client.c = getc(fp);
    	}


	fclose(fp);
	char * str_file = malloc(sizeof(char) * (i + 1));
	FILE * fp1 = fopen(client.filename1, "r");
	if(fp1 == NULL)
	{
		pthread_mutex_unlock(&socklock);
		exit(1);
	}
	client.c = getc(fp1);
	i=0;
	while (client.c != EOF)
	{
		str_file[i] = client.c;
		i++;
		client.c = getc(fp1);
   	}
	if(fp1 != NULL)
	{
		fclose(fp1);	
	}
	if(client.filename1 != NULL)
	{
		free(client.filename1);
	}	
	
	str_file[i] = '\0';	
	
	int sentn = htonl(file_count);
	char * recvline = malloc(100);
	//read(sockfd,recvline,100);
	//printf("first write -> %d\n",ntohl(sentn));
	write(sockfd,&sentn,sizeof(sentn));   	
	read(sockfd,recvline,100);
	//printf("second write -> %d\n",i);
	write(sockfd,&i,sizeof(i));
	read(sockfd,recvline,100);
	client.index1 = 0;
	client.index2 = 0;

	for(client.j = 0; client.j < file_count; client.j++)
    	{
		//sleep(1);
		bzero(sendline, 10000);
		while(str_file[client.index2] != '\n')
		{
			client.index2++;
		}
		client.index2++;
		strncpy(sendline,str_file+client.index1, client.index2 - client.index1);
		write(sockfd,sendline,strlen(sendline)+1);
		client.index1 = client.index2; 
		read(sockfd,recvline,100);
	}
	//read(sockfd,recvline,100);    
	//printf("\nFile Sent\n");
	pthread_mutex_unlock(&socklock);
	return;
}

int isCSV(const char* name)
{
	client.temp=strdup(name);
	char* ext=strrchr(client.temp,'.');
	if(ext!=NULL&&strcmp(ext,".csv")==0)
	{
		if(client.temp!=NULL)
		{
			free(client.temp);
		}
		return 1;
	}
	if(client.temp!=NULL)
	{
		free(client.temp);
	}
	return 0;
}

void* threadify(void* arguments)
{
	struct arg* args=(struct arg*)arguments;
	if(isCSV((*args).filePath))
	{
		talk((*args).filePath,(*args).sortType);
	}
	return "";
}

int search(const char* fpath,const struct stat* sb,int tflag)
{
	struct arg* args=malloc(sizeof(struct arg));
	(*args).filePath=strdup(fpath);
	(*args).sortType=strdup(client.type_global);
	pthread_mutex_lock(&m);
	pthread_create(&client.threads[thread_index++],NULL,&threadify,(void*)args);
	pthread_mutex_unlock(&m);
	return 0;
}

int count_files(const char* fpath,const struct stat* sb,int tflag)
{
	number_of_files++;
	return 0;
}

int main(int argc,char **argv)
{
    char* in_dir=malloc(1000);
	strcpy(in_dir,"./\0");
	client.type_global=malloc(100); 
	strcpy(client.type_global,"none");
	char* out_dir=malloc(1000); 
   	strcpy(out_dir,"./\0"); 
	int port = -1; 
	char* host=malloc(1000);
	strcpy(host,"bad\0");
	if(argc<7)
	{
		printf("./sorter_client -c <column> -h <host name> -p <port>\nOptional: -o <output dir> -d <input dir>");
		return 0;
	}


	if(argc==2&&strcmp(argv[2],"-help")==0)
	{
		printf("./sorter_client -c <column> -h <host name> -p <port>\nOptional: -o <output dir> -d <input dir>");
		return 0;
	}
	if(argc!=7&&argc!=9&&argc!=11)
	{
		printf("ERROR: Incorrect number of arguments\n");
		printf("./sorter_client -c <column> -h <host name> -p <port>\nOptional: -o <output dir> -d <input dir>");
		return 0;
	}
	int ii;
	for(client.ii=0;client.ii<argc;client.ii++)
	{
		if(strcmp(argv[client.ii],"-c") == 0)
		{
			if((client.ii % 2) == 0)
			{
				fprintf(stderr, "<ERROR> : Incorrect format, for more information , please use  $ ./sorter -help \n");
				return 0;
			}
			strcpy(client.type_global, argv[client.ii+1]);
		}
		if(strcmp(argv[client.ii], "-d") == 0)
		{
			if((client.ii % 2) == 0)
			{
				fprintf(stderr, "<ERROR> : Incorrect format, for more information , please use  $ ./sorter -help \n");
				return 0;
			}
		strcpy(in_dir, argv[client.ii+1]);
		}
		if(strcmp(argv[client.ii], "-o") == 0)
		{
			if((client.ii % 2) == 0)
			{
				fprintf(stderr, "<ERROR> : Incorrect format, for more information , please use  $ ./sorter -help \n");
				return 0;
			}
			strcpy(out_dir, argv[client.ii+1]);
		}
		if(strcmp(argv[client.ii], "-p") == 0)
		{
			if((client.ii % 2) == 0)
			{
				fprintf(stderr, "<ERROR> : Incorrect format, for more information , please use  $ ./sorter -help \n");
				return 0;
			}
			port=strtol(argv[client.ii+1],NULL,10);
		}
		if(strcmp(argv[client.ii], "-h") == 0)
		{
			if((client.ii % 2) == 0)
			{
				fprintf(stderr, "<ERROR> : Incorrect format, for more information , please use  $ ./sorter -help \n");
				return 0;
			}
			strcpy(host, argv[client.ii+1]);
		}
	


	}
	if(strcmp(client.type_global,"none")==0||port==-1||strcmp(host,"bad\0") == 0)
	{
		printf("ERROR: Must specify a cloumn, a port, and a host\n");
		return 0;
	}

	client.junk=malloc(2);

	bzero(&servaddr,sizeof servaddr);
 
	servaddr.sin_family=AF_INET;

    	servaddr.sin_port=htons(port);
    	inet_pton(AF_INET,host,&(servaddr.sin_addr));

	int x=ftw(in_dir,count_files,0);
	number_of_files++;
	

	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) == -1)
	{
		printf("ERROR : Failed to connect to server [%s]\n",strerror(errno));
		pthread_mutex_unlock(&socklock);
		exit(EXIT_FAILURE);
	}
	//printf("bepfre write\n");
	//printf("%s\n", client.type_global);
	write(sockfd,client.type_global,strlen(client.type_global));
	read(sockfd,client.junk,2);

	char * junk2 = malloc(2);
	write(sockfd,out_dir,strlen(out_dir));
	read(sockfd,junk2,2);


	if(ftw(in_dir,search,0)==-1)
	{
		fprintf(stderr,"ERROR : Problem in file tree walk\n");
		free(in_dir);
		exit(1);
	}


	for(client.a=0;client.a<thread_index;client.a++)
	{
		pthread_join(client.threads[client.a],NULL);
	}

	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) == -1)
	{
		printf("ERROR : Failed to connect to server [%s]\n",strerror(errno));
		pthread_mutex_unlock(&socklock);
		exit(EXIT_FAILURE);
	}
	char* recv=malloc(100);
	write(sockfd,"-1",2);
	read(sockfd,recv,2);


	//printf("rec\n");
	char * str = malloc(10000);
   	client.size = 0;
	int fileSize=0;

	strcpy(recv,"hello");
	
	if (read(sockfd, &client.size, sizeof(client.size)) == 0) //get client.size lines of file
	{
		printf("1[-] Disconnected from client %d\n", sort.listen_fd); //to be changed to ip?
		free(str);
		exit(EXIT_FAILURE);
	}
	//printf("first read -> %d\n",size);
	int q=write(sockfd,"q",2);
	//printf("[+] Connect to client %d\n", listen_fd); //to be changed to ip?
	if (read(sockfd, &fileSize, sizeof(fileSize)) == 0) //get size lines of file
	{
		//printf("2[-] Disconnected from client %d\n", listen_fd); //to be changed to ip?
		free(str);
		exit(EXIT_FAILURE);
	}
	//printf("second read -> %d\n",fileSize);
	write(sockfd,"q",2);
	client.file=malloc(ntohl(client.size));
	client.whole=malloc(ntohl(fileSize));	
	strcpy(recv,"hello");
	client.j = 0;
	int len=0;
	//printf("start getting file\n");
	for (client.j = 0; client.j < ntohl(client.size); client.j++)
    	{
 		
		bzero(str, 10000); 
       		if(read(sockfd,str,10000) == 0) //go through each line of csv, and get the line
		{
			//printf("3[-] Disconnected from client %d\n", listen_fd); //to be changed to ip?
 			//sockfd = accept(listen_fd, (struct sockaddr*) NULL, NULL);
			continue;
		}
		else
		{
			len+=sprintf(client.whole+len,"%s",str);
			write(sockfd, recv, strlen(recv)+1); //send back a signal to show that it's done recieving data(lets it be in order)	
		}
		 
    	}
	free(str);
	free(client.file);

	char* fname=malloc(1000);
	if(strcmp(out_dir, "./\0") == 0){
		//printf("a");
		sprintf(fname,"%sAllFiles-sorted-%s.csv",out_dir, client.type_global);
	}
	else
		//printf("b");
		sprintf(fname,"%s/AllFiles-sorted-%s.csv",out_dir, client.type_global);

	FILE* done=fopen(fname,"w");
	fprintf(done,"%s",client.whole);
	free(client.whole);
	fclose(done);
	return 0; 
}
