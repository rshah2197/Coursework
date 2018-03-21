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
#include "mergesort.c"
#include "traverse.c"
#include "printdata.c"


int main(int argc, char ** argv){
  if(strcmp(argv[1],"-c") != 0){
    fprintf(stderr, "check usage please");
    return 0;
  }
 if(argc < 2){
    fprintf(stderr, "Check usage please");
    return 0;
  }
  //create a tag for the header
  file.colNum = argv[2];
  file.tag = 0;
  if(strcmp(file.colNum,  "color")==0)file.tag=0;
  else if(strcmp(file.colNum, "director_name")==0)file.tag=1;
  else if(strcmp(file.colNum, "num_critic_for_reviews")==0)file.tag=2;
  else if(strcmp(file.colNum, "duration")==0)file.tag=3;
  else if(strcmp(file.colNum, "director_facebook_likes")==0)file.tag=4;
  else if(strcmp(file.colNum, "actor_3_facebook_likes")==0)file.tag=5;
  else if(strcmp(file.colNum, "actor_2_name")==0)file.tag=6;
  else if(strcmp(file.colNum, "actor_1_facebook_likes")==0)file.tag=7;
  else if(strcmp(file.colNum, "gross")==0)file.tag=8;
  else if(strcmp(file.colNum, "genres")==0)file.tag=9;
  else if(strcmp(file.colNum, "actor_1_name")==0)file.tag=10;
  else if(strcmp(file.colNum, "movie_title")==0)file.tag=11;
  else if(strcmp(file.colNum, "num_voted_users")==0)file.tag=12;
  else if(strcmp(file.colNum, "cast_total_facebook_likes")==0)file.tag=13;
  else if(strcmp(file.colNum, "actor_3_name")==0)file.tag=14;
  else if(strcmp(file.colNum, "facenumber_in_poster")==0)file.tag=15;
  else if(strcmp(file.colNum, "plot_keywords")==0)file.tag=16;
  else if(strcmp(file.colNum, "movie_imdb_link")==0)file.tag=17;
  else if(strcmp(file.colNum, "num_user_for_reviews")==0)file.tag=18;
  else if(strcmp(file.colNum, "language")==0)file.tag=19;
  else if(strcmp(file.colNum, "country")==0)file.tag=20;
  else if(strcmp(file.colNum, "content_rating")==0)file.tag=21;
  else if(strcmp(file.colNum, "budget")==0)file.tag=22;
  else if(strcmp(file.colNum, "title_year")==0)file.tag=23;
  else if(strcmp(file.colNum, "actor_2_facebook_likes")==0)file.tag=24;
  else if(strcmp(file.colNum, "imdb_score")==0)file.tag=25;
  else if(strcmp(file.colNum, "aspect_ratio")==0)file.tag=26;
  else if(strcmp(file.colNum, "movie_facebook_likes")==0)file.tag=27;
  else{
    fprintf(stderr, "You did not enter a valid column name\n");
    return 0;
  }

  //load input and output directory to current working directory
  
 
  if (getcwd(file.directory_input, sizeof(file.directory_input)) == NULL)
  {
    fprintf(stderr, "Current directory could not be read");
    return 0;
  }
  strcpy(file.directory_output, file.directory_input);
  //-o or -d flags
  if(argc >= 4){
    // argument of type: ./sorter -c [col] -d [dir]
    if(strcmp(argv[3],"-d") == 0){
      strcpy(file.directory_input, argv[4]);
      strcpy(file.directory_output, file.directory_input);
      // argument of type: ./sorter -c [col] -d [dir] -o [dir]
      if(argc == 7){
        if(strcmp(argv[5],"-o") == 0){
          strcpy(file.directory_output, argv[6]);
        }
        else{
          fprintf(stderr, "NEED -o flag. Usage is './sorter -c [header] -d [in directory] -o [out directory]'");
          return 0;
        }
      }
    }
    // argument of type: ./sorter -c [col] -o [dir]
    else if(strcmp(argv[3],"-o") == 0)
    {
      strcpy(file.directory_output, argv[4]);
    }
    else{
      fprintf(stderr, "NEED -d or -o flag. Usage is './sorter -c [header] -d [input directory] -o [output directory]'");
      return 0;
    }
  }
  DIR * inputDir = opendir (file.directory_input);
  DIR * outputDir = opendir (file.directory_output);
  if (inputDir == NULL) {
      fprintf(stderr,"Invalid Input Directory %s. Check spelling, then try again.\n",
       file.directory_input);
      return 0;
  }
  if (outputDir == NULL) {
      fprintf(stderr,"Invalid output directory %s. Check spelling, then try again.\n",
       file.directory_output);
      return 0;
  }
  file.ppid = getpid();
  printf("Initial PID: %d\n", file.ppid);
  printf("PIDS of all child processes: ");
  fflush(stdout); 
  //in order to keep track of the count, we implemented a mmap
  number_of_processes = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

  file.firstProcess = fork();
  if(file.firstProcess == 0){ 
  	process_csv(inputDir, file.directory_input, outputDir, file.directory_output, file.tag,file.colNum);
  } 

  else{
   int status;
    waitpid(file.firstProcess, &status, 0);
    printf("\nTotal number of processes: %d\n", *number_of_processes + 1);
  }
  closedir(inputDir);
  closedir(outputDir);
  if(getpid() == file.ppid){
    puts("");
  }
  return 0;
}








void sort_csv(int colNum, char * stringDir, FILE * target_file, char * nameOfFile, char * title_sort){
  csv.line = NULL;
  csv.cap = 0 * sizeof(char);
  IMDB * pHead = NULL;
  IMDB * node = NULL;
  getline(&csv.line, &csv.cap, target_file); //ignore the headers

  //go line by line
  while (getline(&csv.line, &csv.cap, target_file) != -1) {
    node = (IMDB *)malloc(sizeof(IMDB));
    csv.first = 0;
    csv.last = 0;
    csv.track = csv.line[csv.last];
    csv.header = 0;
    csv.record = 0;
    while((csv.track = csv.line[csv.last]) != '\n'){
        if(csv.track == '"'){
        csv.record = csv.record == 0 ? 1 : 0; //note if we are in quotes
      }
      else{ 
        if(csv.track == ',' && csv.record == 0){ //there is a token
          csv.token = (char *)malloc(sizeof(char));
          csv.token[0] = '\0';
          if(csv.last != csv.first){ //make sure it's not empty
            csv.tempLast = csv.last - 1;
            if(csv.line[csv.first] == '"' && csv.line[csv.last-1] == '"'){ //this trims quotes
              csv.tempLast--;
              csv.first++;
            }
            csv.tempLast++;//move past last valid character
            //trim whitespace
            while(isspace(csv.line[csv.tempLast-1])){
              csv.tempLast--;
            }
            while(isspace(csv.line[csv.first])){
              csv.first++;
            }
            if(csv.line[csv.tempLast - 1] == ' '){
              csv.line[csv.tempLast - 1] = '\0';
            }
            else{
              csv.line[csv.tempLast] = '\0';
            }
            csv.token = (char *)realloc(csv.token, sizeof(char) * (csv.tempLast-csv.first+1));
            memcpy(csv.token, csv.line + csv.first, csv.tempLast - csv.first+1);
          }

          
             if (csv.header == 0){
              node->color = csv.token;
              
            }else if (csv.header == 1){
              node->director_name = csv.token;
              
            }else if (csv.header == 2){
              node->num_critic_for_reviews = csv.token[0] == '\0' ? -1 : atoi(csv.token);
              
            }else if (csv.header == 3){
              node->duration = csv.token[0] == '\0' ? -1 : atoi(csv.token);
              
            }else if (csv.header == 4){
              node->director_facebook_likes = csv.token[0] == '\0' ? -1 : atoi(csv.token);
              
            }else if (csv.header == 5){
              node->actor_3_facebook_likes = csv.token[0] == '\0' ? -1 : atoi(csv.token);
              
            }else if (csv.header == 6){
              node->actor_2_name = csv.token;
              
            }else if (csv.header == 7){
              node->actor_1_facebook_likes = csv.token[0] == '\0' ? -1 : atoi(csv.token);
              
            }else if (csv.header == 8){
              node->gross = csv.token[0] == '\0' ? -1 : atoi(csv.token);
              
            }else if (csv.header == 9){
              node->genres = csv.token;
              
            }else if (csv.header == 10){
              node->actor_1_name = csv.token;
              
            }else if (csv.header == 11){
              node->movie_title = csv.token;
              
            }else if (csv.header == 12){
              node->num_voted_users = csv.token[0] == '\0' ? -1 : atoi(csv.token);
              
            }else if (csv.header == 13){
              node->cast_total_facebook_likes = csv.token[0] == '\0' ? -1 : atoi(csv.token);
              
            }else if (csv.header == 14){
              node->actor_3_name = csv.token;
              
            }else if (csv.header == 15){
              node->facenumber_in_poster = csv.token[0] == '\0' ? -1 : atoi(csv.token);
              
            }else if (csv.header == 16){
              node->plot_keywords = csv.token;
              
            }else if (csv.header == 17){
              node->movie_imdb_link = csv.token;
              
            }else if (csv.header == 18){
              node->num_user_for_reviews = csv.token[0] == '\0' ? -1 : atoi(csv.token);
              
            }else if (csv.header == 19){
              node->language = csv.token;
              
            }else if (csv.header == 20){
              node->country = csv.token;
              
            }else if (csv.header == 21){
              node->content_rating = csv.token;
              
            }else if (csv.header == 22){
              node->budget = csv.token[0] == '\0' ? -1 :atol(csv.token);
              
            }else if (csv.header == 23){
              node->title_year = csv.token[0] == '\0' ? -1 : atoi(csv.token);
              
            }else if (csv.header == 24){
              node->actor_2_facebook_likes = csv.token[0] == '\0' ? -1 : atoi(csv.token);
              
            }else if (csv.header == 25){
              node->imdb_score = csv.token[0] == '\0' ? -1 : atof(csv.token);
              
            }else if (csv.header == 26){
              node->aspect_ratio = csv.token[0] == '\0' ? -1 : atof(csv.token);
              
            }else if (csv.header == 27){
              node->movie_facebook_likes = csv.token[0] == '\0' ? -1 : atoi(csv.token);
              
            
          }
          csv.header++;
          csv.first = ++csv.last;
          continue;
        }
      }
      csv.last++;
    }
    //add final column
    csv.token = (char *)malloc(sizeof(char) * (csv.last-csv.first));
    csv.tempLast = csv.last;
    while(isspace(csv.line[csv.tempLast])){
      csv.tempLast--;
    }
    while(isspace(csv.line[csv.first])){
      csv.first++;
    }
    memcpy(csv.token, csv.line + csv.first, csv.tempLast - csv.first + 1);
    node->movie_facebook_likes = csv.token[0] == '\0' ? -1 : atoi(csv.token);

    //we need to create a new struct here
    node->next = pHead;
    pHead = node;
  }

  //using the header that was inputted, we sort (mergesort)
  IMDB ** sortCol = mergesort(&node, colNum);
  IMDB * colSorted = *sortCol;
  csv.loc = strlen(nameOfFile);
  csv.new_CSV[(csv.loc-4) + 13 + strlen(stringDir) + strlen(title_sort)];//13 is for -sorted- + .csv + new slash
  strcpy(csv.new_CSV, stringDir);
  strcat(csv.new_CSV, "/");
  strncat(csv.new_CSV,nameOfFile,csv.loc - 4);//trim the .csv extension
  strcat(csv.new_CSV,"-sorted-");
  strcat(csv.new_CSV,title_sort);
  strcat(csv.new_CSV,".csv");

  printdata(colSorted, csv.new_CSV);
  return;
}
