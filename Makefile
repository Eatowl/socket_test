all:
	gcc -Wall -o server server.c
	gcc -Wall -o client client.c
	gcc -Wall -o serverUDP udpServer.c
	gcc -Wall -o clientUDP udpClient.c
	gcc -Wall -o mcastS mcastServ.c
	gcc -Wall -o mcastC mcastClient.c