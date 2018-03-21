#ifndef SORTER_H
#define SORTER_H

static int *number_of_processes;
//helps organizes the dirctory structure
struct process{
  int ppid;
  char * colNum;
  int tag;
  char directory_input[1024];
  char directory_output[1024];
  int firstProcess;
  
};

struct process file;
//helps organizes the processes
struct sort{
  int pid;
  char * title;
  int past_processes;
  int loc;
  pid_t pidOfChild;
  char * sorted;
  int Pid_new;
  int State;
};

struct sort traverse;
//helps organize the csv file
struct content{
   char * line;
   size_t cap;
   int first;
   int last;
   char track;
   int header;
   short record;
   char * token;
   int tempLast;
   int loc;
   char new_CSV[];
   
};

struct content csv;




struct Record{
  char* color;
  char* director_name;
  int num_critic_for_reviews;
  int duration;
  int director_facebook_likes;
  int actor_3_facebook_likes;
  char * actor_2_name;
  int actor_1_facebook_likes;
  int gross;
  char * genres;
  char * actor_1_name;
  char * movie_title;
  int num_voted_users;
  int cast_total_facebook_likes;
  char * actor_3_name;
  int facenumber_in_poster;
  char * plot_keywords;
  char * movie_imdb_link;
  int num_user_for_reviews;
  char * language;
  char * country;
  char * content_rating;
  long budget;
  int title_year;
  int actor_2_facebook_likes;
  float imdb_score;
  float aspect_ratio;
  int movie_facebook_likes;

  struct Record * next;
};
typedef struct Record IMDB;
//arrays for headers
    char numCritic[50] = "";
    char duration[50] = "";
    char directLikes[50] = "";
    char actor3Likes[50] = "";
    char actor1Likes[50] = "";
    char gross[50] = "";
    char numVoted[50] = "";
    char castLikes[50] = "";
    char faceNumber[50] = "";
    char numReviews[50] = "";
    char budget[50] = "";
    char actor2Likes[50] = "";
    char titleYear[50] = "";
    char imdbScore[50] = "";
    char aspectRatio[50] = "";
    char movieLikes[50] = "";

IMDB ** mergesort(IMDB **, int);
int count(IMDB **);
IMDB * split(IMDB **, int);
IMDB ** merge(IMDB **, IMDB **, int);
void sort_csv(int sortByCol, char * stringDir, FILE * target_file, char * nameOfFile, char* title_sort);
void   process_csv(DIR * input_Directory, char * inDir, DIR * outputDir, char * outDir, int sortByCol, char* title_sort);


#endif
