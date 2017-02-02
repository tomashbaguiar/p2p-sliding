#include "tp_socket.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char **argv){
	char ip[1000];
	strcpy(ip,argv[1]);
	int porta = atoi(argv[2]); 
	char nome_arquivo[1000];
	strcpy(nome_arquivo,argv[3]); 
	int tam_buffer = atoi(argv[4]); 
	int tam_janela = atoi (argv[5]); 
	int aux = tp_init();
	//printf("porta eh %d\n tambuffer eh %d\n tam_janela eh %d\n ip eh %s\n nome arquivo eh%s\n",porta,tam_buffer,tam_janela,ip,nome_arquivo);
	int socket=tp_socket(0);
		if(socket == -1){
			printf("erro socket -1\n");
			return -1;
		}
		else if(socket == -2){
			printf("erro socket -2\n"); 
			return -2;
		}
		else if(socket == -3){
		printf("erro socket -3\n"); 
			return -3;
		}
	so_addr addr;
	if(tp_build_addr(&addr, ip ,  porta)==-1){
		return -1;
	} ; 
	char buffer[tam_buffer];
	strcpy(buffer,nome_arquivo);
	printf("O BUFFER EH %s\n",buffer);

	int dados_enviados = tp_sendto(socket, buffer, tam_buffer, &addr); //envia o nome do arquivo
	printf("ENVIOU\n");

	FILE *p_arquivo;
	int dados_recebidos = tp_recvfrom(socket, buffer, tam_buffer, &addr);  
	if(strcmp(buffer,"&&")==0){ //compara o buffer com a flag de arquivo n existente
			printf("O BUFFER EH %s\n",buffer);
			dados_recebidos=0;
			printf("ARQUIVO NAO ENCONTRADO\n");
		}else{
			p_arquivo=fopen(nome_arquivo,"w");
			fprintf(p_arquivo,"%s",buffer);
			int fim_while = 0;
			while(fim_while!=1){
				printf("dados recebidos eh %d\n",dados_recebidos);
				printf(" ENTROU NO WHILE\n");
				dados_recebidos = tp_recvfrom(socket, buffer, tam_buffer, &addr);  
				if(strcmp(buffer,"###")==0){
					fim_while=1;
				}else{
					fprintf(p_arquivo,"%s",buffer);
					printf("recebeu %s\n",buffer);
					}
				}
			fclose(p_arquivo);
			}
	
	printf("depois do fclose\n");
	tp_mtu();
	
}
