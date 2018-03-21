#include "sorter_thread.h"
#include <string.h>


int count(IMDB ** node)//keeps count
{
	int count=0;
	while(*node){
		if(count < 100){
			
		}
		node = &(**node).next;
		++count;
	}

	return count;
}

IMDB   * split(IMDB  ** node, int count)//splits the linked list
{
	if( count % 2 == 0){
	--count;
	}
	if(count == 3){
	IMDB   ** temp = &((*node)->next);
	IMDB   * headB = (*temp)->next;
	(*temp)->next = NULL;
	return headB;
	}
	int x; IMDB   ** temp = node;
	for(x = count/2; x > 0; x --){
	temp = &((*temp)->next);
	}
	IMDB   * headB = (*temp)->next;
	(**temp).next = NULL;
	return headB;
}

IMDB   **mergeHelper(IMDB   **node,IMDB   ** headB,int sublistB,int headerNum)
{//helper function that does the second sublist
    if(sublistB == 0){
        (*headB)->next = *merge(node,&((*headB)->next),headerNum);
        return headB;
    }

    (*headB)->next = *node;
    return headB;
}


IMDB   ** merge(IMDB   ** node, IMDB   ** headB, int headerNum)
{//merges the linkedlists
	
      if(!(*node)){
      
          return headB;
        }
      if(!(*headB)){
        
        return node;
      }
      int sublistA = 0; 
      if(!((*node)->next)){
      sublistA = 1;
      }int sublistB = 0;
      if(!((*headB)->next)){
      sublistB = 1;
      }
      
	  
//Changed if else block to switch case as switch cases are faster with single valued variables  
switch(headerNum){ 
    case 0: //start at color
        if(strcasecmp(((*node)->color),((*headB)-> color))<=0){
           if(sublistA == 0){
            (*node)->next = *merge(&(*node)->next,headB,headerNum);
            return node;
            }
            (*node)->next = *headB;
            return node;
        }
          
	    return mergeHelper(node,headB,sublistB,headerNum);
	    break;
  	case 1: 
        if(strcasecmp(((*node)->director_name),((*headB)-> director_name))<=0){
               if(sublistA == 0){
            (*node)->next = *merge(&(*node)->next,headB,headerNum);
            return node;
            }
            (*node)->next = *headB;
            return node;
        }
        return mergeHelper(node,headB,sublistB,headerNum);
        break;
    case 2: 
        if(((*node)->num_critic_for_reviews) <= (((*headB)-> num_critic_for_reviews))){
              if(sublistA == 0){
            (*node)->next = *merge(&(*node)->next,headB,headerNum);
            return node;
            }
            (*node)->next = *headB;
            return node;
        }
        return mergeHelper(node,headB,sublistB,headerNum);
        break;
    case 3: 
        if(((*node)->duration) <= (((*headB)-> duration))){
              if(sublistA == 0){
            (*node)->next = *merge(&(*node)->next,headB,headerNum);
            return node;
            }
            (*node)->next = *headB;
            return node;
        }
        return mergeHelper(node,headB,sublistB,headerNum);
        break;
  	case 4: 
        if(((*node)->director_facebook_likes) <= (((*headB)->director_facebook_likes))){
              if(sublistA == 0){
            (*node)->next = *merge(&(*node)->next,headB,headerNum);
            return node;
            }
            (*node)->next = *headB;
            return node;
        }
          return mergeHelper(node,headB,sublistB,headerNum);
        break;
  	case 5: 
        if(((*node)->actor_3_facebook_likes) <= (((*headB)-> actor_3_facebook_likes))){
            if(sublistA == 0){
            (*node)->next = *merge(&(*node)->next,headB,headerNum);
            return node;
            }
            (*node)->next = *headB;
            return node;
        }
          return mergeHelper(node,headB,sublistB,headerNum);
          return headB;
          break;
  	case 6: 
        if(strcasecmp(((*node)->actor_2_name),((*headB)-> actor_2_name))<=0){
              if(sublistA == 0){
            (*node)->next = *merge(&(*node)->next,headB,headerNum);
            return node;
            }
            (*node)->next = *headB;
            return node;
        }
        return mergeHelper(node,headB,sublistB,headerNum);
        break;        
  	case 7: 
        if(((*node)->actor_1_facebook_likes) <= ((*headB)-> actor_1_facebook_likes)){
            if(sublistA == 0){
            (*node)->next = *merge(&(*node)->next,headB,headerNum);
            return node;
            }
            (*node)->next = *headB;
            return node;
        }
        return mergeHelper(node,headB,sublistB,headerNum);
        break;        
  	case 8:
        if(((*node)->gross) <= (((*headB)-> gross))){
              if(sublistA == 0){
            (*node)->next = *merge(&(*node)->next,headB,headerNum);
            return node;
            }
            (*node)->next = *headB;
            return node;
        }
        return mergeHelper(node,headB,sublistB,headerNum);
        break;        
  	case 9: 
        if(strcasecmp(((*node)->genres),((*headB)-> genres))<0){
                if(sublistA == 0){
            (*node)->next = *merge(&(*node)->next,headB,headerNum);
            return node;
            }
            (*node)->next = *headB;
            return node;
        }
        return mergeHelper(node,headB,sublistB,headerNum);
        break;        
  	case 10: 
        if(strcasecmp(((*node)->actor_1_name),((*headB)-> actor_1_name))<=0){
                if(sublistA == 0){
            (*node)->next = *merge(&(*node)->next,headB,headerNum);
            return node;
            }
            (*node)->next = *headB;
            return node;
        }
        return mergeHelper(node,headB,sublistB,headerNum);
        break;        
  	case 11: 
        if(strcasecmp(((*node)->movie_title),((*headB)-> movie_title))<=0){
            if(sublistA == 0){
            (*node)->next = *merge(&(*node)->next,headB,headerNum);
            return node;
            }
            (*node)->next = *headB;
            return node;
        }
        return mergeHelper(node,headB,sublistB,headerNum);
        break;        
  	case 12: 
        if(((*node)->num_voted_users) <= (((*headB)-> num_voted_users))){
              if(sublistA == 0){
            (*node)->next = *merge(&(*node)->next,headB,headerNum);
            return node;
            }
            (*node)->next = *headB;
            return node;
        }
        return mergeHelper(node,headB,sublistB,headerNum);
        break;        
  	case 13: 
        if(((*node)->cast_total_facebook_likes) <= (((*headB)->cast_total_facebook_likes))){
             if(sublistA == 0){
            (*node)->next = *merge(&(*node)->next,headB,headerNum);
            return node;
            }
            (*node)->next = *headB;
            return node;
        }
        return mergeHelper(node,headB,sublistB,headerNum);
        break;        
  	case 14: 
        if(strcasecmp(((*node)->actor_3_name),((*headB)-> actor_3_name))<=0){
              if(sublistA == 0){
            (*node)->next = *merge(&(*node)->next,headB,headerNum);
            return node;
            }
            (*node)->next = *headB;
            return node;
        }
        return mergeHelper(node,headB,sublistB,headerNum);
        break;        
  	case 15: 
        if(((*node)->facenumber_in_poster) <= (((*headB)-> facenumber_in_poster))){
             if(sublistA == 0){
            (*node)->next = *merge(&(*node)->next,headB,headerNum);
            return node;
            }
            (*node)->next = *headB;
            return node;
        }
        return mergeHelper(node,headB,sublistB,headerNum);
        break;        
  	case 16:
        if(strcasecmp(((*node)->plot_keywords),((*headB)->plot_keywords))<=0){
               if(sublistA == 0){
            (*node)->next = *merge(&(*node)->next,headB,headerNum);
            return node;
            }
            (*node)->next = *headB;
            return node;
        }
        return mergeHelper(node,headB,sublistB,headerNum);
        break;        
  	case 17:
        if(strcasecmp(((*node)->movie_imdb_link),((*headB)->movie_imdb_link))<=0){
               if(sublistA == 0){
            (*node)->next = *merge(&(*node)->next,headB,headerNum);
            return node;
            }
            (*node)->next = *headB;
            return node;
        }
        return mergeHelper(node,headB,sublistB,headerNum);
        break;        
  	case 18: 
        if(((*node)->num_user_for_reviews) <= (((*headB)-> num_user_for_reviews))){
              if(sublistA == 0){
            (*node)->next = *merge(&(*node)->next,headB,headerNum);
            return node;
            }
            (*node)->next = *headB;
            return node;
        }
        return mergeHelper(node,headB,sublistB,headerNum);
        break;        
  	case 19:
        if(strcasecmp(((*node)->language),((*headB)->language))<=0){
                if(sublistA == 0){
            (*node)->next = *merge(&(*node)->next,headB,headerNum);
            return node;
            }
            (*node)->next = *headB;
            return node;
        }
        return mergeHelper(node,headB,sublistB,headerNum);
        break;        
  	case 20: 
        if(strcasecmp(((*node)->country),((*headB)->country))<=0){
               if(sublistA == 0){
            (*node)->next = *merge(&(*node)->next,headB,headerNum);
            return node;
            }
            (*node)->next = *headB;
            return node;
        }
          (*headB)->next = *merge(node,&((*headB)->next),headerNum);
        return headB;
        break;        
  	case 21: 
        if(strcasecmp(((*node)->content_rating),((*node)->next)->content_rating)<=0){
              if(sublistA == 0){
            (*node)->next = *merge(&(*node)->next,headB,headerNum);
            return node;
            }
            (*node)->next = *headB;
            return node;
        }
        return mergeHelper(node,headB,sublistB,headerNum);
        break;        
  	case 22: 
        if(((*node)->budget) <= (((*headB)-> budget))){
             if(sublistA == 0){
            (*node)->next = *merge(&(*node)->next,headB,headerNum);
            return node;
            }
            (*node)->next = *headB;
            return node;
        }
        return mergeHelper(node,headB,sublistB,headerNum);
        break;        
  	case 23: 
        if(((*node)->title_year) <= (((*headB)->title_year))){
             if(sublistA == 0){
            (*node)->next = *merge(&(*node)->next,headB,headerNum);
            return node;
            }
            (*node)->next = *headB;
            return node;
        }
        return mergeHelper(node,headB,sublistB,headerNum);
        break;        
  	case 24: 
        if(((*node)->actor_2_facebook_likes) <= (((*headB)-> actor_2_facebook_likes))){
              if(sublistA == 0){
            (*node)->next = *merge(&(*node)->next,headB,headerNum);
            return node;
            }
            (*node)->next = *headB;
            return node;
        }
        return mergeHelper(node,headB,sublistB,headerNum);
        break;        
  	case 25: 
        if(((*node)->imdb_score) <= (((*headB)-> imdb_score))){
             if(sublistA == 0){
            (*node)->next = *merge(&(*node)->next,headB,headerNum);
            return node;
            }
            (*node)->next = *headB;
            return node;
        }
        return mergeHelper(node,headB,sublistB,headerNum);
        break;        
  	case 26: 
        if(((*node)->aspect_ratio) <= (((*headB)-> aspect_ratio))){
             if(sublistA == 0){
            (*node)->next = *merge(&(*node)->next,headB,headerNum);
            return node;
            }
            (*node)->next = *headB;
            return node;
        }
        return mergeHelper(node,headB,sublistB,headerNum);
        break;        
  	case 27: 
        if(((*node)->movie_facebook_likes) <= (((*headB)-> movie_facebook_likes))){
            if(sublistA == 0){
            (*node)->next = *merge(&(*node)->next,headB,headerNum);
            return node;
            }
            (*node)->next = *headB;
            return node;
        }
	      return mergeHelper(node,headB,sublistB,headerNum);
        break;        
    default:
      return node;
      break;        
}	




}
IMDB   **mergesortHelper(IMDB   *temp,IMDB   ** node)
{//mergesort helper function
	temp = (*node)->next;
        temp->next = *node;
        (*node)->next = NULL;
        *node = temp;
	return node;
}

IMDB   **mergesort(IMDB   ** node, int headerNum)
 {//Does the mergesort!
	 int size = count(node);
	 if(size > 2){
		IMDB   * headB; IMDB   * fhead;
		headB = split(node,size);
		IMDB   ** secondHalf = &headB;
		headB = *mergesort(secondHalf,headerNum);
		fhead = *mergesort(node,headerNum);
		*node = fhead;
		*secondHalf  = headB;
		return merge(node,secondHalf,headerNum);

	}
	if(size == 2){ 
		IMDB   * temp;

    //Changed if else block to switch case as switch cases are faster with single valued variables  
    switch(headerNum){
      case 0: //start at color
          if(strcasecmp((*node)->color,((*node)->next)-> color)<=0){
              return node;
          }
              
          return mergesortHelper(temp,node);
          break;
      case 1: 
          if(strcasecmp((*node)->director_name,((*node)->next)-> director_name)<=0){
              return node;
          }
          return mergesortHelper(temp,node);
          break;    
      case 2: 
          if(((*node)->num_critic_for_reviews) <= (((*node)->next)-> num_critic_for_reviews)){
              return node;
          }
          return mergesortHelper(temp,node);
          break;         
      case 3: 
          if(((*node)->duration) <= (((*node)->next)-> duration)){
              return node;
          }
          return mergesortHelper(temp,node);
          break;    
      case 4:
          if(((*node)->director_facebook_likes) <= (((*node)->next)->director_facebook_likes)){
              return node;
          }
          return mergesortHelper(temp,node);
      case 5: 
          if(((*node)->actor_3_facebook_likes) <= (((*node)->next)-> actor_3_facebook_likes)){
              return node;
          }
          return mergesortHelper(temp,node);
          break;    
      case 6: 
          if(strcasecmp((*node)->actor_2_name,((*node)->next)-> actor_2_name)<=0){
              return node;
          }
          return mergesortHelper(temp,node);
          break;    
      case 7: 
          if(((*node)->actor_1_facebook_likes) <= (((*node)->next)->actor_1_facebook_likes)){
              return node;
          }
          return mergesortHelper(temp,node);
          break;    
      case 8: 
          if(((*node)->gross) <= (((*node)->next)-> gross)){
              return node;
          }
          return mergesortHelper(temp,node);
          break;    
      case 9: 
          if(strcasecmp((*node)->genres,((*node)->next)->genres)<=0){
              return node;
          }
          return mergesortHelper(temp,node);
          break;    
      case 10: 
          if(strcasecmp(((*node)->actor_1_name),((*node)->next)-> actor_1_name)<=0){
              return node;
          }
          return mergesortHelper(temp,node);
          break;    
      case 11: 
          if(strcasecmp((*node)->movie_title,((*node)->next)->movie_title)<=0){
              return node;
          }
          return mergesortHelper(temp,node);
          break;    
      case 12: 
          if(((*node)->num_voted_users) <= (((*node)->next)-> num_voted_users)){
              return node;
          }
          return mergesortHelper(temp,node);
          break;    
      case 13: 
          if(((*node)->cast_total_facebook_likes) <= (((*node)->next)-> cast_total_facebook_likes)){
              return node;
          }
          return mergesortHelper(temp,node);
          break;    
      case 14: 
          if(strcasecmp((*node)->actor_3_name,((*node)->next)-> actor_3_name)<=0){
              return node;
          }
          return mergesortHelper(temp,node);
          break;    
      case 15: 
          if(((*node)->facenumber_in_poster) <= (((*node)->next)-> facenumber_in_poster)){
              return node;
          }
          return mergesortHelper(temp,node);
          break;    
      case 16: 
          if(strcasecmp((*node)->plot_keywords,((*node)->next)-> plot_keywords)<=0){
              return node;
          }
          return mergesortHelper(temp,node);
          break;    
      case 17: 
          if(strcasecmp((*node)->movie_imdb_link,((*node)->next)-> movie_imdb_link)<=0){
              return node;
          }
          return mergesortHelper(temp,node);
          break;    
      case 18: 
          if(((*node)->num_user_for_reviews) <= (((*node)->next)-> num_user_for_reviews)){
              return node;
          }
          return mergesortHelper(temp,node);
          break;    
      case 19:
          if(strcasecmp((*node)->language,((*node)->next)-> language)<=0){
              return node;
          }
          return mergesortHelper(temp,node);
          break;    
      case 20: 
          if(strcasecmp((*node)->country,((*node)->next)-> country)<=0){
              return node;
          }
          return mergesortHelper(temp,node);
          break;    
      case 21: 
          if(strcasecmp((*node)->content_rating,((*node)->next)-> content_rating)<=0){
              return node;
          }
          return mergesortHelper(temp,node);
          break;    
      case 22: 
          if(((*node)->budget) <= (((*node)->next)-> budget)){
              return node;
          }
          return mergesortHelper(temp,node);
          break;    
      case 23:
          if(((*node)->title_year) <= (((*node)->next)-> title_year)){
              return node;
          }
          return mergesortHelper(temp,node);
          break;    
      case 24: 
          if(((*node)->actor_2_facebook_likes) <= (((*node)->next)-> actor_2_facebook_likes)){
              return node;
          }
          return mergesortHelper(temp,node);
          break;    
      case 25: 
          if(((*node)->imdb_score) <= (((*node)->next)-> imdb_score)){
              return node;
          }
          return mergesortHelper(temp,node);
          break;    
      case 26: 
          if(((*node)->aspect_ratio) <= (((*node)->next)-> aspect_ratio)){
              return node;
          }
          return mergesortHelper(temp,node);
          break;    
      case 27: 
          if(((*node)->movie_facebook_likes) <= (((*node)->next)-> movie_facebook_likes)){
              return node;
          }
          return mergesortHelper(temp,node);
          break;    
    }
    

  
  }
    return node;
}