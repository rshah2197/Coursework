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

void mergeStr(CSVRow* arr,CSVRow* help, int lptr,int rptr,int llimit,int rlimit,int num)
{
    int i=lptr,j=llimit,k=0;
    while(i<=rptr && j<=rlimit) 
    {
	if(strcmp(arr[i].data,arr[j].data)==0)
	{
		if(arr[i].point<arr[j].point)
		{
				bzero(help[k].data, strlen(help[k].data));	
        	    strcpy(help[k].data,arr[i].data);
				help[k].point=arr[i].point;
        	    bzero(help[k].string_row, strlen(help[k].string_row));
				strcpy(help[k].string_row,arr[i].string_row);
        	    k++;
        	    i++;
		}
        	else
		{
        	    bzero(help[k].data, strlen(help[k].data));
				strcpy(help[k].data,arr[j].data);
        	    help[k].point=arr[j].point;
        	    bzero(help[k].string_row, strlen(help[k].string_row));
				strcpy(help[k].string_row,arr[j].string_row);
        	    k++;
        	    j++;
		}
	}
        else if(strcmp(arr[i].data,arr[j].data)<0)
	{
            bzero(help[k].data, strlen(help[k].data));
			strcpy(help[k].data,arr[i].data);
            help[k].point=arr[i].point;
            bzero(help[k].string_row, strlen(help[k].string_row));
			strcpy(help[k].string_row,arr[i].string_row);
            k++;
            i++;
	}
        else
	{
            bzero(help[k].data, strlen(help[k].data));
			strcpy(help[k].data,arr[j].data);
            help[k].point=arr[j].point;
            strcpy(help[k].string_row,arr[j].string_row);
            k++;
            j++;
	}
    }

    while(i<=rptr)
    {
            bzero(help[k].data, strlen(help[k].data));
			strcpy(help[k].data,arr[i].data);
            help[k].point=arr[i].point;
            strcpy(help[k].string_row,arr[i].string_row);
            k++;
            i++;
    }
    while(j<=rlimit) 
    {
            bzero(help[k].data, strlen(help[k].data));	
			strcpy(help[k].data,arr[j].data);
            help[k].point=arr[j].point;
            strcpy(help[k].string_row,arr[j].string_row);
            k++;
            j++;
    }
    for(i=lptr,j=0;i<=rlimit;i++,j++)
    {
	    bzero(arr[i].data, strlen(arr[i].data));	
		strcpy(arr[i].data,help[j].data);
		arr[i].point=help[j].point;
		strcpy(arr[i].string_row,help[j].string_row);
    }
}

void sortStr(CSVRow* a,CSVRow *b, int i,int j,int num)
{
    int mid;
    if(i<j)
    {
        mid=(i+j)/2;
        sortStr(a,b,i,mid,num); 
        sortStr(a,b,mid+1,j,num);
        mergeStr(a,b, i,mid,mid+1,j,num); 
    }
}

void mergeInt(CSVRow* arr,CSVRow* help, int lptr,int rptr,int llimit,int rlimit,int num)
{
    int i=lptr,j=llimit,k=0;
    while(i<=rptr && j<=rlimit) 
    {
	if(strtof(arr[i].data,NULL)==strtof(arr[j].data,NULL))
	{
		if(arr[i].point<arr[j].point)
		{
        	    bzero(help[k].data, strlen(help[k].data));
				strcpy(help[k].data,arr[i].data);
        	    help[k].point=arr[i].point;
        	    strcpy(help[k].string_row,arr[i].string_row);
        	    k++;
        	    i++;
		}
        	else
		{
        	    bzero(help[k].data, strlen(help[k].data));
				strcpy(help[k].data,arr[j].data);
        	    help[k].point=arr[j].point;
        	    strcpy(help[k].string_row,arr[j].string_row);
        	    k++;
        	    j++;
		}
	}
        else if(strtof(arr[i].data,NULL)<strtof(arr[j].data,NULL))
	{
			bzero(help[k].data, strlen(help[k].data));
			strcpy(help[k].data,arr[i].data);
            help[k].point=arr[i].point;
            strcpy(help[k].string_row,arr[i].string_row);
            k++;
            i++;
	}
        else
	{
            bzero(help[k].data, strlen(help[k].data));
			strcpy(help[k].data,arr[j].data);
            help[k].point=arr[j].point;
            strcpy(help[k].string_row,arr[j].string_row);
            k++;
            j++;
	}
    }
    while(i<=rptr) 
    {
            bzero(help[k].data, strlen(help[k].data));
			strcpy(help[k].data,arr[i].data);
            help[k].point=arr[i].point;
            strcpy(help[k].string_row,arr[i].string_row);
            k++;
            i++;
    }
    while(j<=rlimit) 
    {
            bzero(help[k].data, strlen(help[k].data));
			strcpy(help[k].data,arr[j].data);
            help[k].point=arr[j].point;
            strcpy(help[k].string_row,arr[j].string_row);
            k++;
            j++;
    }
    for(i=lptr,j=0;i<=rlimit;i++,j++)
    {
        bzero(arr[i].data, strlen(arr[i].data));	
		strcpy(arr[i].data,help[j].data);
		arr[i].point=help[j].point;
		strcpy(arr[i].string_row,help[j].string_row);
    }
}

void sortInt(CSVRow* a,CSVRow* b, int i,int j,int num)
{
    int mid;
    if(i<j)
    {
        mid=(i+j)/2;
        sortInt(a,b, i,mid,num); 
        sortInt(a,b, mid+1,j,num); 
        mergeInt(a,b, i,mid,mid+1,j,num); 
    }
}

void callMe(int size,char type,CSVRow* arr, CSVRow* b)
{
	if(type=='i')
	{
		sortInt(arr,b, 1,size-1,size);
	}
	else
	{
		sortStr(arr,b, 1,size-1,size);
	}
	return;
}

void trim(char* str)
{
	char * t = malloc(strlen(str));
	int i =0;
	int j=0;
	for(i=0;i<strlen(str);i++)
	{
		if(isspace(str[i])==0)
		{
			t[j]=str[i];
			j++;
		}
	}
	t[j]='\0';
	strcpy(str,t);
	free(t);
}

