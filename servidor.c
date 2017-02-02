
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include "tp_socket.h" 
#include <dirent.h>

int main(int argc, char **argv){
	int porta = atoi(argv[1]);
	int tam_buffer = atoi(argv[2]); 
	int tam_janela = atoi (argv[3]); 
	char diretorio[1000] ; 
	char ip[1000];
	strcpy(diretorio, argv[4]); 
	int aux = tp_init();
	int socket=tp_socket(porta);
	
		if(socket == -1){
			printf("erro socket -1\n");
		}
		else if(socket == -2){
			printf("erro socket -2\n");
		}
		else if(socket == -3){
			printf("erro socket -3\n");
		}

	so_addr addr;
	char buffer[tam_buffer];
	char nome_arquivo[tam_buffer];


	int dados_recebidos = tp_recvfrom(socket, nome_arquivo, tam_buffer, &addr); //recebe o nome do arquivo
	
	printf("NOME ARQUIVO %s\n",nome_arquivo); 
	printf("O BUFFER EH %s\n",buffer);
	printf(" TAMANHO DA STRING %d\n",strlen(nome_arquivo));
	int dados_enviados=1;
	
	//DIR *dp;   
	//dp = opendir(diretorio);
	FILE *p_arquivo; 
	strcat(diretorio,"/");
		strcat(diretorio,nome_arquivo);
		printf("endereco eh: %s\n",diretorio);
		p_arquivo = fopen(diretorio, "r");  
	//p_arquivo=fopen(nome_arquivo,"r"); //abre o arquivo
	
		if(p_arquivo==NULL){ //caso o arquivo não seja encontrado, será enviado um flag para o cliente
			dados_enviados = tp_sendto(socket, "&&", tam_buffer, &addr); //flag enviada para o cliente
		}else{ //caso o arquivo seja encont
			while(!feof(p_arquivo)){
				//fscanf(p_arquivo,"%[^\n]s",buffer); //grava uma parte do arquivo no buffer
				fgets(buffer,tam_buffer,p_arquivo);
				if(!feof(p_arquivo)){
					dados_enviados = tp_sendto(socket, buffer, tam_buffer, &addr); //envia os dados
					printf("enviou %s\n",buffer);
				}
			}
			dados_enviados = tp_sendto(socket, "###", tam_buffer, &addr);// flag indicando o fim da transmição 
			printf("enviou flag de fim de arquivo");
				fclose(p_arquivo);
		}
		//closedir( dp );
	tp_mtu();

}