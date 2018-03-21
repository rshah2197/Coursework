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
#include "Sorter.h"


void process_csv(DIR * input_Directory, char * directory_input, DIR * outputDir, char * directory_output,  int colNum, char* title_sort){
  struct dirent* file_open;


  while((file_open = readdir(input_Directory)) != NULL){
    traverse.sorted = strstr(file_open->d_name, "-sorted-");
    if((strcmp(file_open->d_name, ".") == 0 || strcmp(file_open->d_name, "..") == 0) && file_open->d_type == 4){
      continue; //skipping appropriate items
    }
    if(traverse.sorted){ //contains -sorted- or is . or is ..
    	continue; //move on to the next file
    }
    traverse.pidOfChild = fork();
    if(traverse.pidOfChild == 0){
    	continue; //the parent moves on to the next file while the children stay on the current one
    }
    *number_of_processes = (*number_of_processes)+1;
    traverse.title = file_open->d_name;
    traverse.past_processes = 0;
    traverse.loc = strlen(traverse.title);

    traverse.pid = getpid();
    
    printf("%d, ", traverse.pid);
    fflush(stdout);
    

    //The following handles files
    if(file_open->d_type == 8 && traverse.title[traverse.loc-4] == '.' && traverse.title[traverse.loc-3] == 'c' && traverse.title[traverse.loc-2] == 's' && traverse.title[traverse.loc-1] == 'v'){
      char path[strlen(directory_input) + 1 + strlen(traverse.title)];
      strcpy(path, directory_input);
      strcat(path, "/");
      strcat(path, traverse.title);
      FILE * readFile = fopen(path, "r");
      sort_csv(colNum, directory_output, readFile, traverse.title, title_sort);
      fclose(readFile);
    }
    //The following handles directories
    if(file_open->d_type == 4){
      char newDir[1 + strlen(directory_input) + strlen(traverse.title)];
      strcpy(newDir, directory_input);
      strcat(newDir, "/");
      strcat(newDir, traverse.title);
      DIR * open = opendir(newDir);
      //fork a child process to sort
      traverse.Pid_new = fork();
      if(traverse.Pid_new == 0){
        process_csv(open, newDir, outputDir, directory_output, colNum, title_sort);
      }
      //child goes through entire directory while parent waits then continues when child is done
      if(traverse.Pid_new != 0){
        
        waitpid(traverse.Pid_new, &traverse.State, 0);

      }
    }

    waitpid(traverse.pidOfChild, &traverse.State, 0);
    exit(0);
    }

  exit(0);
}
