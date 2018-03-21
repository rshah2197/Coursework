#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>
#include "sorter.h"
#include "mergesort.c"
#include "printdata.c"


int main(int argc, char *argv[]){
  csv.line = NULL;
  csv.cap = 0 * sizeof(char);
  int tag = 0;
  if(argc != 3){
    printf("Invalid Number of Arguments'\n");
    return 0;
  }

  if(strcmp(argv[1], "-c") != 0){
    printf("Format: 'cat input.file | ./sorter -c [column]'\n");
  }
  csv.headerNum = argv[2];
  
  struct Rec * pHead = NULL;
  struct Rec * node = NULL;
  //assigns a tag based on the input that corresponds with the column
  if(strcmp(csv.headerNum,  "color")==0)tag=0;
  else if(strcmp(csv.headerNum, "director_name")==0)tag=1;
  else if(strcmp(csv.headerNum, "num_critic_for_reviews")==0)tag=2;
  else if(strcmp(csv.headerNum, "duration")==0)tag=3;
  else if(strcmp(csv.headerNum, "director_facebook_likes")==0)tag=4;
  else if(strcmp(csv.headerNum, "actor_3_facebook_likes")==0)tag=5;
  else if(strcmp(csv.headerNum, "actor_2_name")==0)tag=6;
  else if(strcmp(csv.headerNum, "actor_1_facebook_likes")==0)tag=7;
  else if(strcmp(csv.headerNum, "gross")==0)tag=8;
  else if(strcmp(csv.headerNum, "genres")==0)tag=9;
  else if(strcmp(csv.headerNum, "actor_1_name")==0)tag=10;
  else if(strcmp(csv.headerNum, "movie_title")==0)tag=11;
  else if(strcmp(csv.headerNum, "num_voted_users")==0)tag=12;
  else if(strcmp(csv.headerNum, "cast_total_facebook_likes")==0)tag=13;
  else if(strcmp(csv.headerNum, "actor_3_name")==0)tag=14;
  else if(strcmp(csv.headerNum, "facenumber_in_poster")==0)tag=15;
  else if(strcmp(csv.headerNum, "plot_keywords")==0)tag=16;
  else if(strcmp(csv.headerNum, "movie_imdb_link")==0)tag=17;
  else if(strcmp(csv.headerNum, "num_user_for_reviews")==0)tag=18;
  else if(strcmp(csv.headerNum, "language")==0)tag=19;
  else if(strcmp(csv.headerNum, "country")==0)tag=20;
  else if(strcmp(csv.headerNum, "content_rating")==0)tag=21;
  else if(strcmp(csv.headerNum, "budget")==0)tag=22;
  else if(strcmp(csv.headerNum, "title_year")==0)tag=23;
  else if(strcmp(csv.headerNum, "actor_2_facebook_likes")==0)tag=24;
  else if(strcmp(csv.headerNum, "imdb_score")==0)tag=25;
  else if(strcmp(csv.headerNum, "aspect_ratio")==0)tag=27;
  else if(strcmp(csv.headerNum, "movie_facebook_likes")==0)tag=27;
  else{
    printf("Please use a valid column name!\n");
    return 0;
  }


  //goes through every line
  while (getline(&csv.line, &csv.cap, stdin) != -1) {
    node = (struct Rec *)malloc(sizeof(struct Rec));
    csv.first = 0;
    csv.last = 0;
    csv.track = csv.line[csv.last];
    csv.header = 0;
    csv.record = 0;
    while((csv.track = csv.line[csv.last]) != '\n'){
        if(csv.track == '"'){
        csv.record = csv.record == 0 ? 1 : 0;
      }//char
      else{ // find token
        if(csv.track == ',' && csv.record == 0){ 
          csv.token = (char *)malloc(sizeof(char));
          csv.token[0] = '\0';
          if(csv.last != csv.first){//make sure it's full 
            csv.templast = csv.last - 1;
            if(csv.line[csv.first] == '"' && csv.line[csv.last-1] == '"'){ //this trims the quotes
              csv.templast--;
              csv.first++;
            }
            csv.templast++;
            
            while(isspace(csv.line[csv.templast-1])){//this trims whitespace
              csv.templast--;
            }
            while(isspace(csv.line[csv.first])){
              csv.first++;
            }
            if(csv.line[csv.templast - 1] == ' '){
              csv.line[csv.templast - 1] = '\0';
            }
            else{
              csv.line[csv.templast] = '\0';
            }
            csv.token = (char *)realloc(csv.token, sizeof(char) * (csv.templast-csv.first+1));
            memcpy(csv.token, csv.line + csv.first, csv.templast - csv.first+1);
          }
          //assigns tokens
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
              node->budget = csv.token[0] == '\0' ? -1 : atoi(csv.token);
              
            }else if (csv.header == 23){
              node->title_year = csv.token[0] == '\0' ? -1 : atoi(csv.token);
              
            }else if (csv.header == 24){
              node->actor_2_facebook_likes = csv.token[0] == '\0' ? -1 : atoi(csv.token);
              
            }else if (csv.header == 25){
              node->imdb_score = csv.token[0] == '\0' ? -1 : atof(csv.token);
              
            }else if (csv.header == 26){
              node->aspect_ratio = csv.token[0] == '\0' ? -1 : atof(csv.token);
              
            }else if(csv.header == 27){
              node->movie_facebook_likes = csv.token[0] == '\0' ? -1 : atoi(csv.token);
              
       
              
          }
          csv.header++;
          csv.first = ++csv.last;
          continue;
        }
      }
      csv.last++;
    }

    csv.token = (char *)malloc(sizeof(char) * (csv.last-csv.first));
    csv.templast = csv.last;
    while(isspace(csv.line[csv.templast])){
      csv.templast--;
    }
    while(isspace(csv.line[csv.first])){
      csv.first++;
    }
    memcpy(csv.token, csv.line + csv.first, csv.templast - csv.first + 1);
    node->movie_facebook_likes = csv.token[0] == '\0' ? -1 : atoi(csv.token);


    node->next = pHead;
    pHead = node;
  }


	//calls mergesort (based on the tag) and print function
	int loc = count(&node);
	struct Rec ** sortCol = mergesort(&node, tag);
	struct Rec * colSorted = *sortCol;
	int items = count(sortCol);
	printdata(colSorted);
	return 0;
}
