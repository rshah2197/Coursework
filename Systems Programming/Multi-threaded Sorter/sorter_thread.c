#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <pthread.h>
#include "sorter_thread.h"
#include "mergesort.c"
#include "traverse.c"
#include "printdata.c"


static int csv_switch = 0;

int main(int argc, char ** argv){
  clock_t bg = clock();
  char *c  = NULL;



    if(argc < 2){
        fprintf(stderr, "Check usage please");
        return 0;
    }
    
    if(getcwd(file.directory_input, sizeof(file.directory_input)) == NULL){
        fprintf(stderr, "Current directory could not be read");
        return 0;
    }   

    strcpy(file.directory_output, file.directory_input);


    if(strcmp(argv[1],"-c") == 0){
        c = argv[2];
    }


    if(strcmp(argv[1],"-d") == 0){
        strcpy(file.directory_input, argv[2]);
    }


    if(strcmp(argv[1],"-o") == 0){
        strcpy(file.directory_output, argv[2]);
    } 
    
    if(argc > 3){
      if(strcmp(argv[3],"-c") == 0){
          c = argv[4];
      }


      if(strcmp(argv[3],"-d") == 0){
          strcpy(file.directory_input, argv[4]);
      }


      if(strcmp(argv[3],"-o") == 0){
          strcpy(file.directory_output, argv[4]);
      }
    }        
    
    if(argc > 5){    
      if(strcmp(argv[5],"-c") == 0){
          c = argv[6];
      }


      if(strcmp(argv[5],"-d") == 0){
          strcpy(file.directory_input, argv[6]);
      }


      if(strcmp(argv[5],"-o") == 0){
          strcpy(file.directory_output, argv[6]);
      }
    } 

    if(c == NULL){
        fprintf(stderr, "NEED -o flag. Usage is './sorter -c [header] -d [in directory] -o [out directory]'");
        return 0;
    } 

    //create a tag for the header
    file.colNum = c;
    file.tag = 0;

    //Changed strcmp to memcmp as memcmp should give a little performance edge over strcmp
    if(memcmp(file.colNum,  "color", 5)==0)file.tag=0;
    else if(memcmp(file.colNum, "director_name", 10)==0)file.tag=1;
    else if(memcmp(file.colNum, "num_critic_for_reviews", 10)==0)file.tag=2;
    else if(memcmp(file.colNum, "duration", 8)==0)file.tag=3;
    else if(memcmp(file.colNum, "director_facebook_likes", 10)==0)file.tag=4;
    else if(memcmp(file.colNum, "actor_3_facebook_likes", 10)==0)file.tag=5;
    else if(memcmp(file.colNum, "actor_2_name", 10)==0)file.tag=6;
    else if(memcmp(file.colNum, "actor_1_facebook_likes", 10)==0)file.tag=7;
    else if(memcmp(file.colNum, "gross", 5)==0)file.tag=8;
    else if(memcmp(file.colNum, "genres", 6)==0)file.tag=9;
    else if(memcmp(file.colNum, "actor_1_name", 10)==0)file.tag=10;
    else if(memcmp(file.colNum, "movie_title", 10)==0)file.tag=11;
    else if(memcmp(file.colNum, "num_voted_users", 10)==0)file.tag=12;
    else if(memcmp(file.colNum, "cast_total_facebook_likes", 10)==0)file.tag=13;
    else if(memcmp(file.colNum, "actor_3_name", 10)==0)file.tag=14;
    else if(memcmp(file.colNum, "facenumber_in_poster", 10)==0)file.tag=15;
    else if(memcmp(file.colNum, "plot_keywords", 10)==0)file.tag=16;
    else if(memcmp(file.colNum, "movie_imdb_link", 10)==0)file.tag=17;
    else if(memcmp(file.colNum, "num_user_for_reviews", 10)==0)file.tag=18;
    else if(memcmp(file.colNum, "language", 8)==0)file.tag=19;
    else if(memcmp(file.colNum, "country", 7)==0)file.tag=20;
    else if(memcmp(file.colNum, "content_rating", 10)==0)file.tag=21;
    else if(memcmp(file.colNum, "budget", 6)==0)file.tag=22;
    else if(memcmp(file.colNum, "title_year", 10)==0)file.tag=23;
    else if(memcmp(file.colNum, "actor_2_facebook_likes", 10)==0)file.tag=24;
    else if(memcmp(file.colNum, "imdb_score", 10)==0)file.tag=25;
    else if(memcmp(file.colNum, "aspect_ratio", 10)==0)file.tag=26;
    else if(memcmp(file.colNum, "movie_facebook_likes", 10)==0)file.tag=27;
    else{
        fprintf(stderr, "You did not enter a valid column name\n");
        return 0;
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
    
    fflush(stdout);
    //in order to keep track of the count, we implemented a mmap
    number_of_processes = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);


    thread_args *args = malloc(sizeof *args);
    args->input_Directory = inputDir;
    strcpy(args->directory_input, file.directory_input);
    args->outputDir = outputDir;
    strcpy(args->directory_output, file.directory_output);
    args->colNum = file.tag;
    args->title_sort = file.colNum;

    printf("Initial PID: %u\nTIDs of all child processes: \n", pthread_self());
    char buf[256];
    snprintf(buf, sizeof buf, "%s/AllFiles.csv", file.directory_output, file.colNum);

    FILE * writeFile = fopen(buf, "w");
//    basket = mmap(NULL, sizeof(*basket), PROT_READ|PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);    
    pthread_t tid;
    pthread_create(&tid, NULL, process_csv, args);
//    printf("\t%u\n", tid);
    pthread_join(tid, NULL);
    fclose(writeFile);    

    char bf[256];
    snprintf(bf, sizeof bf, "%s/AllFiles.csv", outputDir);

    char str[] = "AllFiles.csv";

//    printf("%s\n", basename(bf));

    sort_args *s_args = malloc(sizeof(sort_args));
    s_args->colNum = file.tag;
    strcpy(s_args->target_file, bf);
    strcpy(s_args->nameOfFile,str);
    strcpy(s_args->stringDir, outputDir);
    strcpy(s_args->title_sort, args->title_sort);  

    csv_switch = 1;
    sort_csv(s_args);

//    free(p2);
    clock_t en = clock();
    double time_taken = (double)(en - bg)/CLOCKS_PER_SEC;

    printf("\nTotal number of processes: %d\n", *number_of_processes + 1);
//    printf("\nTime taken: %f\n", time_taken);
//    printf("Dirs : %s", file.directory_output);

    closedir(inputDir);
    closedir(outputDir);
    if(getpid() == file.ppid){
        puts("");
    }

    return 0;
}



void sort_csv(void *args){
    pthread_mutex_lock(&lock);

    printf("%u,\n", pthread_self());

    sort_args *s_args = (sort_args *)args;
    int colNum = s_args->colNum;
    char *nameOfFile = s_args->nameOfFile;
    char * stringDir =  s_args->stringDir;
    char * title_sort = s_args->title_sort;

    //printf("csv sort started %s\n", s_args->target_file);
    FILE * target_file = fopen(s_args->target_file, "r");
    if(target_file == 0) {
        pthread_mutex_unlock(&lock);
        return;
    }
    csv.line = NULL;
    csv.cap = 0 * sizeof(char);
    IMDB * pHead = NULL;
    IMDB * node = NULL;
    getline(&csv.line, &csv.cap, target_file); //ignore the headers

//    printf("First line %s\n", csv.line);

//    char    str[]= &csv.line;
    char ** res  = NULL;
    char *  p    = strtok (csv.line, ",");
    int n_spaces = 0, i;


    /* split string and append tokens to 'res' */

    while (p) {
      res = realloc (res, sizeof (char*) * ++n_spaces);

      if (res == NULL)
        exit (-1); /* memory allocation failed */

      res[n_spaces-1] = p;

      p = strtok (NULL, ",");
    }

    /* realloc one extra element for the last NULL */

    res = realloc (res, sizeof (char*) * (n_spaces+1));
    res[n_spaces] = 0;

    /* print the result */

    if(memcmp(res[0], "color", 5) != 0 || memcmp(res[1], "director_name", 10) != 0 || memcmp(res[2], "num_critic", 10) != 0){
      printf("Non IMDB format csv file : %s\n", s_args->nameOfFile);
      free (res);      
      pthread_mutex_unlock(&lock);      
      return;
    }
/*
    for (i = 0; i < (n_spaces+1); ++i)
      printf ("res[%d] = %s\n", i, res[i]);  */

    /* free the memory allocated */

    free (res);


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

              //Changed if else block to switch case as switch cases are faster with single valued variables  
              switch(csv.header){
                  case 0:
                      node->color = csv.token;
                      break;
                  case 1:
                      node->director_name = csv.token;
                      break;  
                  case 2:
                      node->num_critic_for_reviews = csv.token[0] == '\0' ? -1 : atoi(csv.token);
                      break;
                  case 3:
                      node->duration = csv.token[0] == '\0' ? -1 : atoi(csv.token);
                      break;
                  case 4:
                      node->director_facebook_likes = csv.token[0] == '\0' ? -1 : atoi(csv.token);
                      break;
                  case 5:
                      node->actor_3_facebook_likes = csv.token[0] == '\0' ? -1 : atoi(csv.token);
                      break;
                  case 6:
                      node->actor_2_name = csv.token;
                      break;
                  case 7:
                      node->actor_1_facebook_likes = csv.token[0] == '\0' ? -1 : atoi(csv.token);
                      break;
                  case 8:
                      node->gross = csv.token[0] == '\0' ? -1 : atoi(csv.token);
                      break;
                  case 9:
                      node->genres = csv.token;
                      break;
                  case 10:
                      node->actor_1_name = csv.token;
                      break;
                  case 11:
                      node->movie_title = csv.token;
                      break;
                  case 12:
                      node->num_voted_users = csv.token[0] == '\0' ? -1 : atoi(csv.token);
                      break;
                  case 13:
                      node->cast_total_facebook_likes = csv.token[0] == '\0' ? -1 : atoi(csv.token);
                      break;
                  case 14:
                      node->actor_3_name = csv.token;
                      break;
                  case 15:
                      node->facenumber_in_poster = csv.token[0] == '\0' ? -1 : atoi(csv.token);
                      break;
                  case 16:
                      node->plot_keywords = csv.token;
                      break;
                  case 17:
                      node->movie_imdb_link = csv.token;
                      break;
                  case 18:
                      node->num_user_for_reviews = csv.token[0] == '\0' ? -1 : atoi(csv.token);
                      break;
                  case 19:
                      node->language = csv.token;
                      break;
                  case 20:
                      node->country = csv.token;
                      break;
                  case 21:
                      node->content_rating = csv.token;
                      break;
                  case 22:
                      node->budget = csv.token[0] == '\0' ? -1 :atol(csv.token);
                      break;
                  case 23:
                      node->title_year = csv.token[0] == '\0' ? -1 : atoi(csv.token);
                      break;
                  case 24:
                      node->actor_2_facebook_likes = csv.token[0] == '\0' ? -1 : atoi(csv.token);
                      break;
                  case 25:
                      node->imdb_score = csv.token[0] == '\0' ? -1 : atof(csv.token);
                      break;
                  case 26:
                      node->aspect_ratio = csv.token[0] == '\0' ? -1 : atof(csv.token);
                      break;
                  case 27:
                      node->movie_facebook_likes = csv.token[0] == '\0' ? -1 : atoi(csv.token);
                      break;

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
    fclose(target_file);

    //using the header that was inputted, we sort (mergesort)
    IMDB ** sortCol = mergesort(&node, colNum);
    IMDB * colSorted = *sortCol;
    csv.loc = strlen(nameOfFile);
    csv.new_CSV[(csv.loc-4) + 13 + strlen(stringDir) + strlen(title_sort)];//13 is for -sorted- + .csv + new slash
    strcpy(csv.new_CSV, stringDir);
    if(strcmp(stringDir, "")){
        strcat(csv.new_CSV, "/");
    }
    strncat(csv.new_CSV,nameOfFile,csv.loc - 4);//trim the .csv extension
    strcat(csv.new_CSV,"-sorted-");
    strcat(csv.new_CSV,title_sort);
    strcat(csv.new_CSV,".csv");
    //printf("csv sort ended\n");

//    printdata(colSorted, csv.new_CSV);

    char buf[256];

 
        snprintf(buf, sizeof buf, "%s/AllFiles-sorted-<%s>.csv", stringDir, title_sort);    
    

    printdata(colSorted, buf);

    pthread_mutex_unlock(&lock);
    return;
}
