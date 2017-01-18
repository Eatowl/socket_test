#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <resolv.h>
#include <string.h>
#include <stdio.h>
#include <sys/un.h>

#define PORT_TIME 10
#define MAX_BUF 256

int main() {
	char buffer[MAX_BUF];
	char message[MAX_BUF];
	int cl_sock;
	struct sockaddr_in saddr;
	/*
		Системный вызов socket предназначен для создания сокета
		PF_INET - определяет протокол сокета,
		в данном случае используется интернет протокол,
		так же существует локальный протокол
		SOCK_STREAM - способ взаимодействия,
		определяет данный сокет как потоковый
		0 - автоматический выбор протокола передачи данных
	*/
	cl_sock = socket(PF_INET, SOCK_STREAM, 0);
	/*
		Обнуляем исходную структуру
	*/
	bzero(&saddr, sizeof(saddr));
	/*
		Задаем исходные параметры подключения
		- Выбираем протокол
		- Назначаем порт
		- Задаем адрес
		Последние два пункта имеют сетевой порядок байтов,
		поэтому требуется преобразование из серверного в сетевой.
		Порт преобразуется функцией htons.
		Адрес преобразуется функцией inet_addr.
	*/
	saddr.sin_family = PF_INET;
	saddr.sin_port = htons(3425);
	saddr.sin_addr.s_addr=inet_addr("127.0.0.2");
	/*
		Функция connect(), устанавливает соединение
		сокета cl_sock с сокетом сервера, адрес которого представлен
		в структуре sockaddr_in
	*/
	if (connect(cl_sock, (struct sockaddr *) &saddr, sizeof(saddr)) == -1) {
		perror("socket connection failed");
		return 1;
	}
	while (1) {
		fflush(stdin);
		if(!strcmp(fgets(buffer, MAX_BUF, stdin), "exit")) {
			close(cl_sock);
			return 0;
		}
		printf("отправка сообщения на сервер...\n");
		/*
			отправка сообщения на сервер
		*/
    	send(cl_sock, buffer, sizeof(buffer), 0);
    	int bytes_read = 0;
    	printf("Ожидание сообщения\n");
    	/*
			прием сообщения от сервера
		*/
    	bytes_read = recv(cl_sock, message, sizeof(buffer), 0);
    	printf("Получено %d bytes\tСообщение: %s\n", bytes_read, message);
    	/*
			Очистка массивов 
    	*/
		bzero(buffer, MAX_BUF);
		bzero(message, MAX_BUF);
	}	
	close(cl_sock); // Закрываем сокет клиента
	return 0;
}
