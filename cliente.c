#include "tp_socket.h"
#include "slid_protocol.h"
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
    char *buffer = (char *) malloc(tam_buffer * sizeof(char));                                  // Buffer que recebe o arquivo.
	strcpy(buffer,nome_arquivo);
	printf("O BUFFER EH %s\n",buffer);

	int dados_enviados = tp_sendto(socket, buffer, tam_buffer, &addr); //envia o nome do arquivo
	printf("ENVIOU\n");

    uint64_t RWS = tam_janela;                                                                  // Recebe o tamanho da janela.
    int64_t LFR = -1;                                                                           // Ultimo pacote recebido e acknowledged.
    uint64_t LAF = RWS + LFR;                                                                   // Maior seqNum aceitável.
    uint64_t seqNumToAck = 0;                                                                   // Proximo pacote recebido que sera acknowledged.

    Frame *recebido = (Frame *) malloc(tam_janela * sizeof(Frame));                             // Vetor que guarda os pacotes recebidos.

    Frame pacote;                                                                               // Pacote que recebe os pacotes.

	FILE *p_arquivo;
	int dados_recebidos = tp_recvfrom(socket, buffer, 3 * sizeof(char), &addr);  
	if(strcmp(buffer,"&&\0")==0){ //compara o buffer com a flag de arquivo n existente
        printf("O BUFFER EH %s\n",buffer);
		dados_recebidos=0;
		printf("ARQUIVO NAO ENCONTRADO\n");
	}else if(strcmp(buffer, "||\0")==0){
		p_arquivo=fopen("output.txt","w");

        int seqNum = 0;
		while(seqNum != -1) {
            seqNum = proto_recvfrom(socket, addr, &pacote);
            if((seqNum <= LFR) || (seqNum > LAF))   
                printf("Descartando o pacote [%d].\n", seqNum);                                 // Descarta o pacote.
            else if((seqNum > LFR) && (seqNum <= LAF))  {
                recebido[seqNum % RWS] = pacote;                                                // Recebe o pacote na memoria.
                printf("Recebido pacote de [%d].\n", seqNum);
            }

            if((seqNumToAck == (LFR + 1)) && 
                (seqNumToAck == recebido[seqNumToAck % RWS].header.seqNum) &&
                    (seqNumToAck >= 0))  {

                if(recebido[seqNumToAck % RWS].header.flag == 2)    {                           // Verifica flag de fim de arquivo.
                    printf("Final do arquivo.\n");
                    seqNum = -1;
                }
                else 
                    fprintf(p_arquivo,"%s",recebido[seqNumToAck % RWS].Msg);                    // Coloca o buffer no arquivo.


                ack_frame(seqNumToAck, socket, addr);                                           // ACK seqNum.
                LFR = seqNumToAck++;                                                            // Incrementa ultimo pacote recebido.
                LAF = LFR + RWS;                                                                // Incrementa maior seqNum aceitavel.

			}
        }

    }

	fclose(p_arquivo);
    free(buffer);
    free(recebido);
	
	printf("depois do fclose\n");
	tp_mtu();

    return 0;
	
}
