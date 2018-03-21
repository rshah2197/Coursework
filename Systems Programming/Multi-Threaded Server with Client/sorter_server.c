#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <ctype.h>
#include <errno.h>
#include <arpa/inet.h>
#include "sorter_server.h"
#include "mergesort.c"
pthread_mutex_t m;
pthread_mutex_t socklock;
pthread_t* threadIDs;
int threadIndex=0;
struct sockaddr_in servaddr; //init stuff

char* token;
char header='0';
char * out_dir_global;



void doTheSort()
{
	FILE* fp=fopen("files_sorted.txt","r");
	sort.filecount = 0;
	sort.c = 0;
	sort.i = 0;
	sort.str_file = malloc(10);
	sort.row_position = 0;
	
	//fprintf(stdout, "%s\n", token);		
	
	sort.c = fgetc(fp);
	while (sort.c != EOF) 
	{
		//printf("%sort.c\n",sort.c);
		sort.str_file = realloc(sort.str_file, (sort.i+1) * sizeof(char));	
		sort.str_file[sort.i] = sort.c;
		if(sort.c == '\n')
		{
			sort.filecount++;
		}
		sort.i++;
		sort.c = fgetc(fp);
    	}
        
	fclose(fp);
	
	sort.str_file[sort.i] = '\0';
	
	
	CSVRow *movies = malloc(sort.filecount * sizeof(CSVRow));
	//token = strtok(str_file, "\n");
	
	for(sort.j = 0; sort.j < sort.filecount; sort.j++)
	{
		movies[sort.j].data = malloc(10000);
		movies[sort.j].point = sort.j;
		movies[sort.j].string_row = malloc(10000);
	}

	CSVRow* help=malloc(sizeof(CSVRow)*sort.filecount*2);    //array used for merging
    	for(sort.j =0;sort.j<sort.filecount;sort.j++)
    	{
		help[sort.j].data=malloc(10000);
		help[sort.j].point=sort.j;
		help[sort.j].string_row=malloc(10000);
    	}

	


	int temp = 0;
	sort.count = 0;
	sort.index = 0;
	int comma_position_max = 0;
	sort.p1 = 0;
	int p2 = 0;
	int char_found = 0;
	sort.comma_number = 0;
	char * check_token = malloc(1000);
	sort.c = 0;
	for(sort.j = 0; sort.j < sort.i; sort.j++)
	{
		if(sort.str_file[sort.j] == '\n')
		{
			//printf("a\n");
			strncpy(movies[sort.count].string_row, sort.str_file+temp,sort.j-temp+1);
			movies[sort.count].string_row[sort.j-temp+1] = '\0';
			if (sort.count == 0)
			{
				sort.c = movies[sort.count].string_row[sort.index];
				for(sort.index = 0; sort.index<strlen(movies[sort.count].string_row) ; sort.index++)
				{
					//fprintf(stdout, "%sort.c\n", sort.c);
					sort.c = movies[sort.count].string_row[sort.index];
					if(sort.c == ',' || movies[sort.count].string_row[sort.index+1] == '\n')
					{
						if( movies[sort.count].string_row[sort.index+1] == '\n')
						{
							sort.index++;
						}
						comma_position_max++;
						if(sort.index == sort.p1 || sort.index == sort.p1+1)
						{
							check_token = "\0";
						}
						else
						{
							strncpy(check_token, movies[sort.count].string_row+sort.p1,sort.index-sort.p1);
							check_token[sort.index-sort.p1] =  '\0';
							//fprintf(stdout, "[%s] , [%s]\n", check_token, token);
						}
						if(strcmp(check_token,token) == 0)
						{
							char_found = 1;
							//fprintf(stdout, "[%s] , [%s]\n", check_token, token);
							break;
						}
						sort.p1 = sort.index+1;
						if( movies[sort.count].string_row[sort.index+1] == '\n')
						{
							sort.index--;
						}

					}
					//printf("%d\n %sort.c",char_found, sort.c);
				}
				if(char_found == 0)
				{
					//fprintf(stderr, "ERROR: <Selected item was not found in parameters>\n");
					free(check_token);
					free(movies);
					free(token);
					free(sort.str_file);

					return;
				}
				
				strcpy(movies[sort.count].data, token);
				movies[sort.count].data[strlen(token)+1] = '\0';
			}
			else
			{
				//fprintf(stdout, "%d \n ", sort.count);
				sort.comma_number = 0;
				sort.index = 0;
				sort.p1 = 0;
				sort.c = movies[sort.count].string_row[sort.index];
				for(sort.index = 0; sort.index<strlen(movies[sort.count].string_row); sort.index++)
				{
					//fprintf(stdout, "%sort.c\n", sort.c);
					sort.c = movies[sort.count].string_row[sort.index];
					if(sort.c == ',' && sort.index+1 != strlen(movies[sort.count].string_row) && movies[sort.count].string_row[sort.index+1] == '"')
					{
							
						sort.comma_number++;
						if((sort.index == sort.p1) && (sort.comma_number == comma_position_max))
						{
							//movies[sort.count].data = "0\0";
							break;
						}
						else if(sort.comma_number == comma_position_max)
						{
							strncpy(movies[sort.count].data, movies[sort.count].string_row+sort.p1,sort.index-sort.p1);
							//fprintf(stdout, "[%s] , [%s]\n", check_token, token);
							movies[sort.count].data[sort.index-sort.p1] = '\0';
							//fprintf(stdout, "%d: [%s]\n",sort.count, movies[sort.count].data);	
							trim(movies[sort.count].data);
							break;
						}
						sort.p1 = sort.index+1;
						
						sort.index = sort.index+2;
						int x;
						for(x = 0; sort.c != '"'; sort.index++)
						{
							sort.c = movies[sort.count].string_row[sort.index];
						}	
						
						sort.c = movies[sort.count].string_row[sort.index];
						//fprintf(stdout,"%sort.c\n" , sort.c);
						sort.comma_number++;
						if((sort.index == sort.p1) && (sort.comma_number == comma_position_max))
						{
							//movies[sort.count].data = "0\0";
							break;
						}
						else if(sort.comma_number == comma_position_max)
						{
							strncpy(movies[sort.count].data, movies[sort.count].string_row+sort.p1+1,sort.index-sort.p1-2);
							//fprintf(stdout, "[%s] , [%s]\n", check_token, token);
							movies[sort.count].data[sort.index-sort.p1-2] = '\0';
							//fprintf(stdout, "%d: %s\n",sort.count, movies[sort.count].data);	
							trim(movies[sort.count].data);
							break;
						}
						sort.p1 = sort.index+1;
						sort.index++;
						sort.c = movies[sort.count].string_row[sort.index];
	
					}
					if(sort.c == ',' || movies[sort.count].string_row[sort.index+1] == '\n')
					{

						if( movies[sort.count].string_row[sort.index+1] == '\n')
						{
							sort.index++;
						}

						sort.comma_number++;
						if((sort.index == sort.p1) && (sort.comma_number == comma_position_max))
						{
							//movies[sort.count].data = "NULL";
							break;
						}
						else if(sort.comma_number == comma_position_max)
						{
							strncpy(movies[sort.count].data, movies[sort.count].string_row+sort.p1,sort.index-sort.p1);
							//fprintf(stdout, "[%s] , [%s]\n", check_token, token);
							movies[sort.count].data[sort.index-sort.p1] = '\0';
							//fprintf(stdout, "%d: [%s]\n",sort.count, movies[sort.count].data);	
							trim(movies[sort.count].data);
							break;
						}
						sort.p1 = sort.index+1;
						if( movies[sort.count].string_row[sort.index+1] == '\n')
						{
							sort.index--;
						}

					}
				}
			}
			
			temp = sort.j+1;
			sort.count++;
		}
	}
	
	char type = 'i';
	int k;
	for(sort.j = 1; sort.j < sort.filecount; sort.j++)
	{
		//printf("\n");
		for(k = 0; movies[sort.j].data[k] != '\0'; k++)
		{	//printf("%sort.c", movies[sort.j].data[k]);
					
			if(!(isdigit(movies[sort.j].data[k])))
			{
				if(isspace(movies[sort.j].data[k])){
					continue;
				}
				if(movies[sort.j].data[k] != '.' || movies[sort.j].data[k] != '-')
				{
					type = 's';	
					break;
					//printf("%d [%s], 
				}
			}
		}
		if(type == 's'){
			break;
		}
	}
	//printf("%d \n", type);
	//mergesort(movies,1,sort.filecount-1,sort.filecount);
	callMe(sort.filecount,type,movies,help);
	//printf("heyo\n");
	//printf("\n");
	int big=0;
	for(sort.j=0;sort.j<sort.filecount;sort.j++)
	{
		big+=strlen(movies[sort.j].string_row);
	}
	int len=0;
	char* sorted_output=malloc(big);
	for(sort.j = 0; sort.j < sort.filecount; sort.j++)
	{
		//printf("sort.j:%d\n",sort.j);
		len+=sprintf(sorted_output+len,"%s",movies[sort.j].string_row);
		//printf("[%s]\n", movies[sort.j].data);
	}
	
	char * sendline = malloc(10000);
 
	
	sort.row_position = 0;
	sort.j = 0;

	free(sort.str_file);
	
	int sentn = htonl(sort.filecount);
	char * recvline = malloc(100);
	char* resp=malloc(2);
	
	write(sort.comm_fd,&sentn,sizeof(sentn));   	
	read(sort.comm_fd,resp,2);
	//printf("second write -> %d\n",i);
	write(sort.comm_fd,&big,sizeof(big));
	read(sort.comm_fd,resp,2);
	sort.index1 = 0;
	int index2 = 0;
	//printf("start sending file\n");

	for(sort.j = 0; sort.j < sort.filecount; sort.j++)
    	{
		bzero(sendline, 10000);
		while(sorted_output[index2] != '\n')
		{
			index2++;
		}
		index2++;
		strncpy(sendline,sorted_output+sort.index1, index2 - sort.index1);
		write(sort.comm_fd,sendline,strlen(sendline)+1);
		sort.index1 = index2; 
		read(sort.comm_fd,recvline,100);
	}
	//read(sockfd,recvline,100);    
	//printf("\nFile Sent\n");
	return;






	for(sort.j = 0; sort.j < sort.filecount; sort.j++)
	{
		free(movies[sort.j].data);
		free(help[sort.j].data);
		//movies[j].point = j;
		free(movies[sort.j].string_row);
		free(help[sort.j].string_row);
	}


	free(check_token);
	free(movies);
	free(help);
	free(token);
	free(sort.str_file);
	free(sorted_output);
	return;

}

void write_test(FILE * fp, char * str) //test function to be replaced with merge/etc
{
	//printf("%s", str);
	fprintf(fp, "%s", str);
	fflush(fp);
}

void* rec(void* args)
{	
	//printf("rec\n");
	pthread_mutex_lock(&socklock);
	char * str = malloc(10000);
   	int size = 0;
	sort.fileSize=0;
	char* recv=malloc(100);
	strcpy(recv,"hello");
	if (read(sort.comm_fd, &size, sizeof(size)) == 0) //get size lines of file
	{
		//printf("1[-] Disconnected from client %d\n", listen_fd); //to be changed to ip?
		free(str);
		pthread_mutex_unlock(&socklock);
		return "no";
	}
	//printf("-- %d\n",ntohl(size));
	write(sort.comm_fd,"q",2);
	//printf("[+] Connect to client %d\n", listen_fd); //to be changed to ip?
	if(ntohl(size)==758185984)
	{
		//this is not a file
		//printf("dump\n");
		//do the dump thing and send over sorted of all files
		pthread_mutex_unlock(&socklock);
		doTheSort();
		//printf("dd\n");
		header='0';
		return "yo";
	}
	if (read(sort.comm_fd, &sort.fileSize, sizeof(sort.fileSize)) == 0) //get size lines of file
	{
		//printf("2[-] Disconnected from client %d\n", listen_fd); //to be changed to ip?
		free(str);
		pthread_mutex_unlock(&socklock);
		return "no";
	}
	write(sort.comm_fd,"q",2);
	FILE* out_file = fopen("files_sorted.txt", "a");
	sort.file=malloc(ntohl(size));
	sort.whole=malloc(ntohl(sort.fileSize));	
	strcpy(recv,"hello");
	sort.j = 0;
	int len=0;
	for (sort.j = 0; sort.j < ntohl(size); sort.j++)
    	{
 		
		bzero(str, 10000); 
       		if(read(sort.comm_fd,str,10000) == 0) //go through each line of csv, and get the line
		{
			//printf("3[-] Disconnected from client %d\n", listen_fd); //to be changed to ip?
 			sort.comm_fd = accept(sort.listen_fd, (struct sockaddr*) NULL, NULL);
			continue;
		}
		else
		{
			if(sort.j!=0||header=='0')
			{
				header='1';
				len+=sprintf(sort.whole+len,"%s",str);
			}
			write(sort.comm_fd, recv, strlen(recv)+1); //send back a signal to show that it's done recieving data(lets it be in order)	
		}
		 
    	}
	fprintf(out_file,"%s",sort.whole);
 	fclose(out_file);
	free(str);
	free(sort.file);
	free(sort.whole);
	pthread_mutex_unlock(&socklock);
	return "no";
}

int main(int argc, char** argv)
{
	out_dir_global = malloc(1000);
	if(argc==2&&strcmp(argv[1],"-h")==0)
	{
		printf("check args\n");
		return 0;
	}
	if(argc!=3)
	{
		printf("check args\n");
		printf("check args\n");
		return 0;
	}
	if(strcmp(argv[1],"-p")!=0)
	{
		
		printf("check args\n");
		return 0;
	}

	threadIDs=malloc(sizeof(pthread_t)*5); //change 5 to how many files  
 	char state='0';
	bzero( &servaddr, sizeof(servaddr)); //zero at addresse
	 
	//int port = atoi(argv[1]); 
	
		
	if((sort.listen_fd=socket(AF_INET,SOCK_STREAM,0))<=0) //check if sockets being used
	{
		// print error message with perror()
		perror("socket");
		// exit your server
		exit(EXIT_FAILURE);
	}

	printf("Received connections from: ");
	fflush(stdout);
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(strtol(argv[2],NULL,10)); //set to a random port which isn't being used
	while(1)
	{
		bind(sort.listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr)); //bind program to that port
		
		socklen_t len = sizeof(servaddr);
		
		listen(sort.listen_fd, 500); //allow for max 10 connections (not sure how it changes threads)
		
		FILE* fp=fopen("files_sorted.txt","w");
		
		fflush(fp);
		fclose(fp);
		token=malloc(100);

		struct sockaddr* client_addr;
		struct sockaddr* addr;
		int sizeaddr=sizeof(addr);
		sort.comm_fd = accept(sort.listen_fd, (struct sockaddr*)&addr, &sizeaddr);
		struct sockaddr_in* sai=(struct sockaddr_in*)&addr;
		
		char *ip = inet_ntoa((*sai).sin_addr);
//		printf("fsf\n");
//		fflush(stdout);
		printf("%s,",ip);
		fflush(stdout);

		if (read(sort.comm_fd, token, 100) == 0) //get token
		{
			//printf("[-] Disconnected from client %d\n", sort.listen_fd); //to be changed to ip?
			free(token);
			pthread_mutex_unlock(&socklock);
			exit(EXIT_FAILURE);
		}
		//printf("%s\n",token);
		write(sort.comm_fd,"q",2);

		
		if (read(sort.comm_fd, out_dir_global, 999) == 0) //get out directory
		{
			//printf("[-] Disconnected from client %d\n", sort.listen_fd); //to be changed to ip?
			free(out_dir_global);
			pthread_mutex_unlock(&socklock);
			exit(EXIT_FAILURE);
		}
		//printf("%s\n",token);
		write(sort.comm_fd,"q",2);




		while(sort.comm_fd = accept(sort.listen_fd, (struct sockaddr*) NULL, NULL)) //start listening on connection 
		{
			//printf("got one\n");
			void* ret;
			pthread_mutex_lock(&m);
			pthread_t index=threadIDs[threadIndex++];
			pthread_mutex_unlock(&m);
			pthread_create(&index,NULL,&rec,NULL);
			pthread_join(index,&ret);
			char* val=(char*)ret;
			if(val[0]=='y')
			{
	/////			state='1';
				break;
				/*
				if (read(sort.comm_fd, token, sizeof(token)) == 0) //get size lines of file
				{
					printf("[-] Disconnected from client %d\n", sort.listen_fd); //to be changed to ip?
					free(token);
					pthread_mutex_unlock(&socklock);
					exit(EXIT_FAILURE);
				}
				write(sort.comm_fd,"q",2);
				*/
			}
		}

	}	
	printf("\n");
	
	close(sort.listen_fd);
	
	exit(EXIT_SUCCESS);
}
