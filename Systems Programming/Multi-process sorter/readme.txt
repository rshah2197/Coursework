NAME
	Sorter - sorts all csv files in a directory lexicographically and outputs
	
SYNOPSIS
	Sorter -c  [ARGUMENT] -d [DIRIN] -o [DIROUT]
	Sorter -c  [ARGUMENT] -d [DIRIN]
	Sorter -c  [ARGUMENT] -o [DIROUT]
	Sorter -c  [ARGUMENT]
	
DESCRIPTION
	Sorts all text files in an optional [DIRIN] with 28 specific header columns 
	lexicographically by passing the name of the column as an argument, the sorted file
	are outputted as [FILE]-sorted-[ARGUMENT] in an optional [DIROUT] directory. Default [DIRIN]
	is the current directory, default [DIROUT] is the source file. Prints initial PID, all child
	PIDS, and total number of PIDS to stdout.  Used mmaps to accomplish this.
	
OPTIONS
	-c color
		Sorts by color of movie.
	-c director_name
		Sorts by director name alphabetically.
	-c num_critic_for_reviews
		Sorts by the amount of critics who reviewed the movie.
	-c duration
		Sorts by runtime.
	-c director_facebook_likes
		Sorts by amount of faceboook likes for the movie.
	-c actor_3_facebook_likes
		Sorts by amount of facebook likes for main actor 3.
	-c actor_2_name
		Sorts by main actor 2 name alphabetically
	-c actor_1_facebook_likes
		Sorts by amount of facebook likes for main actor 1.
	-c gross
		Sorts by gross income.
	-c genres
		Sorts alphabetically by genre.
	-c actor_1_name
		Sorts by main actor name alphabetically.
	-c movie_title
		Sorts alphabetically by movie title.
	-c num_voted_users
		Sorts by number of user votes.
	-c cast_total_facebook_likes
		Sorts by total cast facebook likes.
	-c actor_3_name
		Sorts by main actor 3 name alphabetically.
	-c facenumber_in_poster
		Sorts by number of faces in poster.
	-c plot_keywords
		Sorts alphabetically by plot keywords.
	-c movie_imdb_link
		Sorts alphabetically by URL.
	-c num_user_for_reviews
		Sorts by number of user views.
	-c language
		Sorts alphabetically by language.
	-c country
		Sorts alphabetically by country.
	-c content_rating
		Sorts alphabetically by MPAA rating.
	-c budget
		Sorts by budget.
	-c title_year
		Sorts by release year.
	-c actor_2_facebook_likes
		Sorts by amount of facebook likes of main actor 2.
	-c imdb_score
		Sorts by IMDB score of the movie.
	-c aspect_ratio
		Sorts by aspect ratio of the movie.
	-c movie_facebook_likes
		Sorts by likes on facebook page of movie.

____________________________________
Header file:
the header file keeps tract of of the members of each function. 
It was easy to organize every variable based on the function it was being used in.
It makes the variables more readable and less arbitray because you know the significance of
them.  It also initialized the arrays for the headers.
the header had the static variable to keep track of the process count

____________________________________
Challenges:
At first, it was difficult trying to picture forking inside our heads as it seems complex but it's really not.  
Forking essentially creates another instance of the program.  Once we had that down, we tried to understand how
to manipulate PID's in order to control the parent and children processes.
Recursivley sorting through directories is pretty straight forward in that regard.

The big challenge was figuring out how to keep track of the processes and keep count of them.  
After reading the textbook, and looking at piazza, we stumbled upon memory maps.
Since it maps process' address space and a file, shared memory objects, it was a great tool to overcome
this obstacle.
