#ifndef SORTER_H
#define SORTER_H
struct content{//keep track of csv information
	const char *headerNum;
	char * line;
	size_t cap;
	int first;
	int last;
	char track;
	int header;
	short record;
	char * token;
	int templast;
};
struct content csv;

//initialized arrays
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
struct Rec{//csv field structure
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


  struct Rec * next;//used this as next
};



struct Rec;


//prototypes
struct Rec ** mergesort(struct Rec **, int);
int count(struct Rec **);
struct Rec * split(struct Rec **, int);
struct Rec ** merge(struct Rec **, struct Rec **, int);



#endif
