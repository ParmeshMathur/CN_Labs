1. Extract the zip folder.


2. Open two distinct terminal windows, one each for the server and the client directories.
	Open a third terminal in the Lab5 directory, and run the makefile. This will compile both the required files in the corresponding directories.


3. In the terminal opened in the server directory, run the following command:
	$./server.out 8080

	Here "8080" is the port number (assigned arbitrarily).


4. In the terminal opened in the client directory, run the following command:
	$./client.out 127.0.0.1 8080

	Here "127.0.0.1" is the IP address to the server.
	"8080" is the port number (same as that for the server program).


5. In the client process, enter the filenames that you require from the server directory. They are parsed and processed one by one in a first come first serve manner.


6. Type "exit" in the client terminal to close the process.


7. In the Lab5 directory if you need to clean the files created, run:
	$make clean


8. Refer to the pdf for screenshot of relevant events.
