
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include "tp_socket.h" 
#include "slid_protocol.h"
#include <dirent.h>
#include <pthread.h>
#include <stdint.h>
#include <signal.h>

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
	char nome_arquivo[tam_buffer];

	int dados_recebidos = tp_recvfrom(socket, nome_arquivo, tam_buffer, &addr); //recebe o nome do arquivo



    uint64_t SWS = tam_janela;                                                                  // Tamanho da janela no servidor.
    int64_t LAR = -1;                                                                           // Ultimo ACK recebido.
    int64_t LFS = -1;                                                                           // Ultimo pacote enviado.

    pthread_t *framet = (pthread_t *) malloc(SWS * sizeof(pthread_t));                          // Vetor que guarda pids das threads criadas.
    Thread_arg *attr = (Thread_arg *) malloc(sizeof(Thread_arg));                               // Estrutura para passar argumentos a thread.
    attr->addr = addr;
    attr->socket = socket;

    char *buffer = (char *) malloc(tam_buffer * sizeof(char));                                  // Buffer que recebera as partes do arquivo.

    uint64_t seqNum = 0;                                                                        // Numero de controle.

    Frame pacote;                                                                               // Pacote que recebe as mensagens.

	printf("NOME ARQUIVO %s\n",nome_arquivo); 
	printf("O BUFFER EH %s\n",buffer);
	printf(" TAMANHO DA STRING %d\n",strlen(nome_arquivo));
	int dados_enviados=1;
	
	FILE *p_arquivo; 
	strcat(diretorio,"/");
	strcat(diretorio,nome_arquivo);
	printf("endereco eh: %s\n",diretorio);
	p_arquivo = fopen(diretorio, "r");  
	
	if(p_arquivo==NULL){ //caso o arquivo não seja encontrado, será enviado um flag para o cliente
		dados_enviados = tp_sendto(socket, "&&\0", 3 * sizeof(char), &addr); //flag enviada para o cliente
	}else{ //caso o arquivo seja encontrado
        dados_enviados = tp_sendto(socket, "||\0", 3 * sizeof(char), &addr); //flag enviada para o cliente

        int sair = 0;
        int enviou = 0;
		while(!sair){
                while((SWS > (LFS - LAR)) && !enviou)   {
                
                    //fgets(buffer,tam_buffer,p_arquivo);
                    int i = 0;
                    for(i = 0; (i < tam_buffer) && !feof(p_arquivo); i++)   
                        buffer[i] = fgetc(p_arquivo);                                           // Buffer recebe o arquivo.

                    if(feof(p_arquivo) && (i == 0)) {                                           // Se o arquivo acabou e o buffer esta vazio.
                        attr->seqNum = seqNum;
                        strcpy(attr->message, "###");
                        attr->flag = 2;
		                printf("enviou flag de fim de arquivo\n");
                        enviou = 1;
                    }
                    else    {
                        attr->seqNum = seqNum++;
                        attr->message = buffer;
                        //strcpy(attr->message, buffer);
                        attr->flag = 0;
                    }

                    LFS++;                                                                      // Incrementa o ultimo enviado.
                    pthread_create(&framet[LFS % SWS], NULL, frame_send, (void *) attr);
                    sleep(1);
                    memset(buffer, 0, tam_buffer);                                              // Zera o buffer.
                }
                int seqNumAck = proto_recvfrom(socket, addr, &pacote);                          // Recebe ackNum.
                if(seqNumAck == (LAR + 1))  {
                    LAR++;
                    //pthread_kill(framet[LAR % SWS], SIGINT);
                    pthread_cancel(framet[LAR % SWS]);
                }

                if(LAR == LFS)                                                                          // Sinaliza final de arquivo.
                    sair = 1;

            }
    }
                        
		                fclose(p_arquivo);
                        free(buffer);
                        free(framet);
                        free(attr);
                        tp_mtu();


    return 0;
}
