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

#define QUEUE_LENGTH 1
#define MAX_BUF 256

int main() {
	/*
		Дескрипторы сокета
	*/
	int server_socket, client_socket;
	/*
		Структура задает адресное пространство и сам адрес
		sockaddr_in - структура для интернет сокетов
	*/
	struct sockaddr_in saddr;
	char *buf;
	/*
		Системный вызов socket предназначен для создания сокета
		PF_INET - определяет протокол сокета,
		в данном случае используется интернет протокол,
		так же существует локальный протокол
		SOCK_STREAM - способ взаимодействия,
		определяет данный сокет как потоковый
		0 - автоматический выбор протокола передачи данных
	*/
	server_socket = socket(PF_INET, SOCK_STREAM, 0);
	buf = (char *) malloc(MAX_BUF);
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
		Созданному сокету server_socket, необходимо назначить адрес,
		который был указан выше в струтуре sockaddr_in,
		за это отвечает функция bind()
	*/
	if (bind(server_socket, (struct sockaddr *) &saddr, sizeof(saddr)) == -1) {
		perror("bind() error");
		return 1;
	}
	/*
		Сервер находится в режиме прослушивания, ожидая подключения клиента.
		За это отвечает функция bind(), в ней указывается сокет и максимальный
		размер очереди запросов на подключение
	*/
	if (listen(server_socket, QUEUE_LENGTH) == -1) {
		perror("listen() error");
		return 0;
	}
	int count;
	while(1) {
		/*
			Как только к серверу подключается клиент, то начинает работу
			системный вызов accept(), он возращает дескриптор нового сокета,
			который будет обслуживать это соединение.
		*/
		client_socket = accept(server_socket, NULL, NULL);
		while(1) {
			printf("Ожидаем сообщение...\n");
			/*
				Принимаем сообщение от клиента и помещаем его в buf.
			*/
			count = recv(client_socket, buf, MAX_BUF, 0);
			if(!strcmp(buf, "exit")) break;
			printf("Получено %d bytes\tСообщение: %s\n", count, buf);
			printf("Отправляю принятое сообщение клиенту\n");
			/*
				Отправляем принятое сообщение обратно
			*/
			send(client_socket, buf, count, 0);
        }
    
        close(client_socket); // закрываем сокет клиента
        _exit(0);
	}
	free(buf);
	close(server_socket); // закрываем сокет сервера
	return 0;
}