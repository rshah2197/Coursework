a. Parameters
Your code will read in a set of parameters via the command line. Records will be stored in CSV files in the provided directory. Directories may have multiple levels and you must find all CSV files. Your code should ignore non-CSV files and CSV files that do not have the correct format of the movie_metadata CSV (e.g. CSV files that have other random data in them).
 
Remember, the first record (line) is the column headings and should not be sorted as data. Your code must take in a command-line parameter to determine which value type (column) to sort on. If that parameter is not present (?-> throw an error, or default behavior). The first parameter your program will be '-c' to indicate sorting by column, the second will ‘-h‘ to indicate the hostname of the server, and the third will be ‘-p’ to indicate the port number the server is listening on. ALL of these parameters are required for the program to run:
 
./sorter_client -c food -h grep.cs.rutgers.edu -p 12345
 
              Be sure to check the arguments are there and that they correspond to a listed value type (column heading) in the CSV.
 
./sorter_client -c food -h grep.cs.rutgers.edu -p 12345
    -d thisdir/thatdir -o anotherdir
 
The fourth parameter to your program will be ‘-d’ indicating the directory the program should search for .csv files. This parameter is optional. The default behavior will search the current directory.
 
The fifth parameter to your program will be ‘-o’ indicating the output where the sorted CSV should be written to. This parameter is optional. The default behavior will be to output in the same directory as the source directory.
 
The server has only one parameter: ‘-p’, and is required for operation:
 
./sorter_server -p 12345
 
 
b. Operation
 
Client
Your client should traverse all subdirectories under the directory it is given. For each file it finds, it should spawn a thread. Each thread it spawns should read in the file, construct a search request, connect to your server, send the request to your server and then wait for and read back the server’s response to be sure the file was sorted. Once the client has sent all files under the search directory to the server, it should send the server another request to give it the sorted version of all the files it was sent. When the server responds and sends back the sorted version of all the files, the client should output a SINGLE  CSV file named:
                AllFiles-sorted-<fieldname>.csv.
 
The client need not output anything to STDOUT.
 
 
Server
The server will open a port and wait for connection requests. On connection, it will spawn a service thread to handle that connection and go back to waiting for requests. Each service thread should read in a client request, if it is a sort request, it should perform the sort and store the results at the server. If it is a dump request, it should merge the current collection of sorted results into one sorted list and send the result back to the client. You may want/need to make use of synchronization constructs like mutex_locks, semaphores, and/or condition variables in your implementation to prevent memory corruption.
 
The server will run until stopped by a SIGKILL (i.e. kill <pid of server>).
 
To STDOUT, output a list of the ip addresses of all the clients that have connected: (?-> when?)
Received connections from: <ipaddress>,<ipaddress>,<ipaddress>,…
 
