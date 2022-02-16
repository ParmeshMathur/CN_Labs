Lab 10 Lab compre
Parmesh Mathur
2018A7PS0133G


1. Extract the zip folder.


2. Open three distinct terminal windows, one for the server and two for each client.
	 Run the makefile in either of the terminals. This will compile both the required files. Execute:
	 $ make output


3. In the server terminal, execute the following command:
	$./server.out 8080

	//The server accepts "8080" as the command line input.
	//Here "8080" is the port number (assigned arbitrarily).


4. In one of the client terminals, execute the following command:
	$./client 127.0.0.1 8080 private_key1.pem public_key2.pem

	//Client accpets "127.0.0.1", "8080", "private_key1.pem", "public_key2.pem" (in given order) as command line inputs.
	//Here "127.0.0.1" is the IP address to the server.
	//Here "8080" is the port number (which is required to be same as that for the server program).
	//Here "private_key1.pem" is the private key required to decrypt the message received from the other client.
	//Here "public_key2.pem" is the public key that required to send the message to the other client.


5. In one of the client terminals, execute the following command:
	$./client 127.0.0.1 8080 private_key2.pem public_key1.pem

	//Client accpets "127.0.0.1", "8080", "private_key2.pem", "public_key1.pem" (in given order) as command line inputs.
	//Here "127.0.0.1" is the IP address to the server.
	//Here "8080" is the port number (which is required to be same as that for the server program).
	//Here "private_key2.pem" is the private key required to decrypt the message received from the other client.
	//Here "public_key1.pem" is the public key that required to send the message to the other client.
	

6. The keys "private_key1.pem" and "public_key1.pem" are complementary, so are the keys "private_key2.pem" and "public_key1.pem".


7. In either of the client processes, enter the desired string that is to be sent. This string is encrypted and forwarded through to the other client via the server process.


8. The other client in this case will receive the encrypted string and (ideally) decrypt it, and display the plaintext and ciphertext.
	NOTE: In the process, for undocumented reasons, neither of the clients is able to decrypt the encrypted messages, although the encrypted message is received properly. 
	Consequently, the plaintext displayed in the receiving client's terminal is always empty, regardless of the message.
	The error was taking place in the inbuilt RSA_private_decrypt function, and could not be fixed, although it worked with passign the message via a file instead of a socket.


9. Type "exit" in the client terminal to close the client process, which (ideally) closes the other client as well. In this case the server process keeps running.
	NOTE: since the exit functionality was dependant on the decryption, it only closes the process in which "exit" was typed.


10. The server is built to handle only two client connections at a time. Any more or less than this might cause undefined behaviour.


11. If the server is quit while the clients are running, the clients will have undefined behaviour.


12. In the Lab10 directory if you need to clean the executable files, execute:
	$ make clean
	
	
13. In the Lab10 directory, if the private and public keys need to be remade, execute:
	$ make keys
	
	
14. In the Lab10 directory, if the keys need to be cleared, execute:
	$ make key_clean


15. A sample run looks like the following:

	>make output
	>make keys [optional]
	>./server 8080
	>./client 127.0.0.1 8080 private_key1.pem public_key2.pem
	>./client 127.0.0.1 8080 private_key2.pem public_key1.pem
	
	>exit (in client terminal) to exit from either client.
	>Ctrl + C to exit from the server
	
