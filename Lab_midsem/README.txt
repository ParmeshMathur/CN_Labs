1. Extract the zip folder.


2. Open two distinct terminal windows, one each for the server and the client directories.
	Open a third terminal in the Lab_midsem directory, and run the makefile. This will compile both the required files in the corresponding directories.


3. In the terminal opened in the server directory, execute the following command:
	$./server.out 8080

	//The server accepts "8080" as the command line input.
	//Here "8080" is the port number (assigned arbitrarily).


4. In the terminal opened in the server directory, execute the following command:
	$./client.out 127.0.0.1 8080

	//Client accpets "127.0.0.1" and "8080" (in given order) as command line inputs.
	//Here "127.0.0.1" is the IP address to the server.
	//Here "8080" is the port number (which is required to be same as that for the server program).


5. In the client process, enter the desired string to be reversed. This string is reversed and shown in the other terminal, and vice versa.


6. Type "exit" in the client terminal to close the client process, in this case the server process keeps running. Type "exit" in the server terminal to close both the running processes.


7. In the Lab_midsem directory if you need to clean the (.out) files, execute:
	$make clean

8. The [.pcap] file provided can be viwed by opening it in Wireshark.


9. Refer to the pdf for screenshot of relevant events.
