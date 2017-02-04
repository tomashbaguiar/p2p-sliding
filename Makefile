all: slid_protocol tp_socket cliente servidor clean

slid_protocol: slid_protocol.h slid_protocol.c
	gcc -Wall -c slid_protocol.c

tp_socket: tp_socket.c tp_socket.h
	gcc -Wall -c tp_socket.c

cliente: tp_socket.o slid_protocol.o
	gcc -Wall tp_socket.o slid_protocol.o cliente.c -o clienteUDP

servidor: tp_socket.o slid_protocol.o
	gcc -Wall tp_socket.o slid_protocol.o servidor.c -o servidorUDP -pthread

clean: slid_protocol.o tp_socket.o
	rm *.o
	ls
