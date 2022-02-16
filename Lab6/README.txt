1. Extract the zip folder.


2. Open distinct terminal windows, one for the server and one for each client desired.
	 Run the makefile in either of the terminals. This will compile both the required files.


3. In the server terminal, execute the following command:
	$./server.out 8080

	//The server accepts "8080" as the command line input.
	//Here "8080" is the port number (assigned arbitrarily).


4. In each of the client terminals, execute the following command:
	$./client.out 127.0.0.1 8080

	//Client accpets "127.0.0.1" and "8080" (in given order) as command line inputs.
	//Here "127.0.0.1" is the IP address to the server.
	//Here "8080" is the port number (which is required to be same as that for the server program).


5. In the client process, enter the desired string to be reversed. This string is reversed and shown in the server terminal, and vice versa.


6. Type "exit" in the client terminal to close the client process, in this case the server process keeps running. Type "exit" in the server terminal to close the corresponding client process.


7. The server is built to handle upto four client connections at a time. Any more than this will be rejected by the server.


8. In the Lab6 directory if you need to clean the (.out) files, execute:
	$make clean


8. Refer to the pdf for screenshot of relevant events.
