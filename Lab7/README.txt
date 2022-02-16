1. Extract the zip folder.


2. Open the folder in a terminal folder. Once opened, compile the client.c file. Execute the command:
	$ gcc client.c -lssl -lcrypto

	
3. The directory contains an openssl certificate. In case required, run the command:
	$ openssl req -x509 -nodes -days 365 -newkey rsa:1024 -keyout mycert.pem -out mycert.pem


4. In the server terminal, execute the following command:
	$./a.out <URL>
	Here the URL is the command line arguement. It must be an HTTP/HTTPS based (valid) URL for the process to execue properly.
	The program expects only one argument


5. If the URL is valid and is a pth to a vlid file, the process establishes a connection with the requested host and the file is downloaded and saved in the currect working directory. The directory can be checked for the results.
	

6. If the file with the required name already exists, then it is overwritten without prompting. The progroam can handle text files, PDF files, image files etc.


7. If the response code of the file is not 200, the program will prompt so and exit.


8. Refer to the pdf for screenshots of relevant events.


9. Sources:
	https://aticleworld.com/ssl-server-client-using-openssl-in-c/
	https://stackoverflow.com/questions/33960385/how-to-download-a-file-from-http-using-c
	https://www.i-programmer.info/programming/cc/9993-c-sockets-no-need-for-a-web-server.html
