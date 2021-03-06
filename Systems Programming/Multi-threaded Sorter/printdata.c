#include <stdio.h>
#include "sorter_thread.h"


static int headed = 0;

void printdata(IMDB* colSorted, char* newFile) {
    //printf("writing started %s \n", newFile);
    FILE * writeFile = fopen(newFile, "a");

    //headers are printed out
    if(headed == 0){
        headed = 1;
        fprintf(writeFile,"color,director_name,num_critic_for_reviews,duration,director_facebook_likes,"
            "actor_3_facebook_likes,actor_2_name,actor_1_facebook_likes,gross,genres,actor_1_name,"
            "movie_title,num_voted_users,cast_total_facebook_likes,actor_3_name,facenumber_in_poster,"
            "plot_keywords,movie_imdb_link,num_user_for_reviews,language,country,content_rating,"
            "budget,title_year,actor_2_facebook_likes,imdb_score,aspect_ratio,movie_facebook_likes\n"); 
    }


/*    strcat(basket,"color,director_name,num_critic_for_reviews,duration,director_facebook_likes,"
        "actor_3_facebook_likes,actor_2_name,actor_1_facebook_likes,gross,genres,actor_1_name,"
        "movie_title,num_voted_users,cast_total_facebook_likes,actor_3_name,facenumber_in_poster,"
        "plot_keywords,movie_imdb_link,num_user_for_reviews,language,country,content_rating,"
        "budget,title_year,actor_2_facebook_likes,imdb_score,aspect_ratio,movie_facebook_likes\n"); */

    while(colSorted != NULL){
        IMDB * printData = colSorted;

        if(printData->num_critic_for_reviews != -1){
            snprintf(numCritic, 5000, "%d",printData->num_critic_for_reviews);
        }
        if(printData->duration != -1){
            snprintf(duration, 5000, "%d",printData->duration);
        }
        if(printData->director_facebook_likes != -1){
            snprintf(directLikes, 5000, "%d",printData->director_facebook_likes);
        }
        if(printData->actor_3_facebook_likes != -1){
            snprintf(actor3Likes, 5000, "%d",printData->actor_3_facebook_likes);
        }
        if(printData->actor_1_facebook_likes != -1){
            snprintf(actor1Likes, 5000, "%d",printData->actor_1_facebook_likes);
        }
        if(printData->gross != -1){
            snprintf(gross, 5000, "%d",printData->gross);
        }
        if(printData->num_voted_users != -1){
            snprintf(numVoted, 5000, "%d",printData->num_voted_users);
        }
        if(printData->cast_total_facebook_likes != -1){
            snprintf(castLikes, 5000, "%d",printData->cast_total_facebook_likes);
        }
        if(printData->facenumber_in_poster != -1){
            snprintf(faceNumber, 5000, "%d",printData->facenumber_in_poster);
        }
        if(printData->num_critic_for_reviews != -1){
            snprintf(numReviews, 5000, "%d",printData->num_critic_for_reviews);
        }
        if(printData->budget != -1){
            snprintf(budget, 5000, "%li",printData->budget);
        }
        if(printData->actor_2_facebook_likes != -1){
            snprintf(actor2Likes, 5000, "%d",printData->actor_2_facebook_likes);
        }
        if(printData->title_year != -1){
            snprintf(titleYear, 5000, "%d",printData->title_year);
        }
        if(printData->imdb_score != -1){
            snprintf(imdbScore, 5000, "%f",printData->imdb_score);
        }
        if(printData->aspect_ratio != -1){
            snprintf(aspectRatio, 5000, "%f",printData->aspect_ratio);
        }
        if(printData->movie_facebook_likes != -1){
            snprintf(movieLikes, 5000, "%d",printData->movie_facebook_likes);
        }

        if(strchr(printData->movie_title, ',') == NULL){ //for no commas in movie title
            fprintf(writeFile,"%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n",
            printData->color, printData->director_name, numCritic, duration,
            directLikes, actor3Likes, printData->actor_2_name,
            actor1Likes, gross, printData->genres, printData->actor_1_name,
            printData->movie_title, numVoted, castLikes,
            printData->actor_3_name, faceNumber, printData->plot_keywords, printData->movie_imdb_link,
            numReviews,printData->language, printData->country, printData->content_rating,
            budget, titleYear, actor2Likes, imdbScore,
            aspectRatio, movieLikes);

 /*           char str[1000];
            snprintf(str, sizeof(str), "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n", printData->color, printData->director_name, numCritic, duration,
            directLikes, actor3Likes, printData->actor_2_name,
            actor1Likes, gross, printData->genres, printData->actor_1_name,
            printData->movie_title, numVoted, castLikes,
            printData->actor_3_name, faceNumber, printData->plot_keywords, printData->movie_imdb_link,
            numReviews,printData->language, printData->country, printData->content_rating,
            budget, titleYear, actor2Likes, imdbScore,
            aspectRatio, movieLikes);

            printf("%s", *basket); */

//            strcat(*basket, str); 
        }
        else{ //have to put quotes around movie title
            fprintf(writeFile,"%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,\"%s\",%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n",
            printData->color, printData->director_name, numCritic, duration,
            directLikes, actor3Likes, printData->actor_2_name,
            actor1Likes, gross, printData->genres, printData->actor_1_name,
            printData->movie_title, numVoted, castLikes,
            printData->actor_3_name, faceNumber, printData->plot_keywords, printData->movie_imdb_link,
            numReviews,printData->language, printData->country, printData->content_rating,
            budget, titleYear, actor2Likes, imdbScore,
            aspectRatio, movieLikes);


/*            char str[100000];
            snprintf(str, sizeof(str),  "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,\"%s\",%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n",printData->color, printData->director_name, numCritic, duration,
            directLikes, actor3Likes, printData->actor_2_name,
            actor1Likes, gross, printData->genres, printData->actor_1_name,
            printData->movie_title, numVoted, castLikes,
            printData->actor_3_name, faceNumber, printData->plot_keywords, printData->movie_imdb_link,
            numReviews,printData->language, printData->country, printData->content_rating,
            budget, titleYear, actor2Likes, imdbScore,
            aspectRatio, movieLikes);     */  

//            strcat(basket, str);             
        }
        IMDB * temp = colSorted;
        colSorted = colSorted->next;
        free(temp);
    }
//    fclose(writeFile);
    //printf("writing ended %s\n",newFile);
}
