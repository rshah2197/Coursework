#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include "sorter_thread.h"

void process_csv( void * args){
    //printf("process csv call  %x\n", args);
    thread_args *args_data = (thread_args *)args;
    DIR * input_Directory = args_data->input_Directory;
    char *directory_input = args_data->directory_input;
    DIR * outputDir = args_data->outputDir;
    char * directory_output = args_data->directory_output;
    int colNum = args_data->colNum;
    char* title_sort = args_data->title_sort;

    //printf("dir input %s\n"
    //       "dir output %s\n" 
    //       "colNum %d \n"
    //       "titleSort %s\n", directory_input, directory_output, colNum, title_sort);


    struct dirent* file_open;

    while((file_open = readdir(input_Directory)) != NULL){
        traverse.sorted = strstr(file_open->d_name, "-sorted-");

        if((strcmp(file_open->d_name, ".") == 0 || strcmp(file_open->d_name, "..") == 0) && file_open->d_type == 4){
            continue; //skipping appropriate items
        }
        if(traverse.sorted){ //contains -sorted- or is . or is ..
            continue; //move on to the next file
        }

        traverse.title = file_open->d_name;
        traverse.past_processes = 0;
        traverse.loc = strlen(traverse.title);

        //The following handles files
        if(file_open->d_type == 8 && traverse.title[traverse.loc-4] == '.' && traverse.title[traverse.loc-3] == 'c' && traverse.title[traverse.loc-2] == 's' && traverse.title[traverse.loc-1] == 'v'){
            char path[strlen(directory_input) + 1 + strlen(traverse.title)];
            strcpy(path, directory_input);
            strcat(path, "/");
            strcat(path, traverse.title);

            sort_args *s_args = malloc(sizeof(sort_args));
            s_args->colNum = colNum;
            strcpy(s_args->target_file, path);
            strcpy(s_args->nameOfFile, traverse.title);
            strcpy(s_args->stringDir, directory_output);
            strcpy(s_args->title_sort, title_sort);

            *number_of_processes = (*number_of_processes)+1;
            //printf("write file %s\n", traverse.title);
            pthread_t tid;

            pthread_create(&tid, NULL, sort_csv, s_args);
//            printf("\t%u\n", tid);
//            printf("%u, ", pthread_self());
//            printf("\tchild :%ld\t%u\n", pthread_self(), tid);
            pthread_join(tid, NULL);
            //printf("writing file finished %s \n", traverse.title);
        }else if(file_open->d_type == 4){   //The following handles directories
            char newDir[1 + strlen(directory_input) + strlen(traverse.title)];
            strcpy(newDir, directory_input);
            strcat(newDir, "/");
            strcat(newDir, traverse.title);
            //printf("dir name %s\n", newDir);

            DIR * open = opendir(newDir);

            thread_args *argsl = malloc(sizeof(thread_args));
            argsl->input_Directory = open;
            strcpy(argsl->directory_input, newDir);
            argsl->outputDir = outputDir;
            strcpy(argsl->directory_output, directory_output);
            argsl->colNum = colNum;
            argsl->title_sort = title_sort;

            *number_of_processes = (*number_of_processes)+1;
            pthread_t tid;
            pthread_create(&tid, NULL, process_csv, argsl);
            //printf("next folder %s %x\n", newDir, argsl);
            //printf("Join thread %d, \n", tid);
//            printf("\t%u\n", tid);//fflush(stdout);
            printf("%u,\n", pthread_self());
//            printf("\tDirectory thread :%u\t%u\n", pthread_self(), tid);//fflush(stdout);
            pthread_join(tid, NULL);
            //printf("Join finsh thread %d, \n", tid);

        }else{
            printf("Not a csv file. : %s\n", file_open->d_name);            
        }

    }
    pthread_exit(0);
}
