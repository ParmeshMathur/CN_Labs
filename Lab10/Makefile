output:
	gcc -o server server.c -lpthread
	gcc -o client client.c -lpthread -lcrypto

keys:
	openssl genrsa -out private_key1.pem 10240
	openssl rsa -in private_key1.pem -pubout > public_key1.pem
	openssl genrsa -out private_key2.pem 10240
	openssl rsa -in private_key2.pem -pubout > public_key2.pem

clean:
	rm -rf server client

key_clean:
	rm -rf *.pem