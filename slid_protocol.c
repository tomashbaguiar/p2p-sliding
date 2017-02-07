#include "slid_protocol.h"
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

void *
frame_send(void *arg)
{
    Frame pacote;                                                                           // Pacote a ser enviado.
    Thread_arg *args = (Thread_arg *) arg;                                                  // Recebe os argumentos.

    /*  Coloca as informaçoes no pacote */
    pacote.header.seqNum = args->seqNum;
    pacote.header.ackNum = 0;
    pacote.header.flag = args->flag;                                                        // Define o tipo de pacote.
    for(int i = 0; i < strlen(args->message); i++)
        pacote.Msg[i] = args->message[i];                                                   // Copia a mensagem no pacote.

    /*  Loop ate a thread ser terminada no programa principal   */
    while(1)    {
        printf("Enviando pacote de seqNum [%d].\n", pacote.header.seqNum);
        sleep(TIMER);                                                                       // Tempo de espera para reenvio.
        proto_sendto(args->socket, &pacote, args->addr);
    }
}

int
proto_recvfrom(const int socket, const struct sockaddr_in6 addr, Frame *pacote)
{
    socklen_t slen = sizeof(struct sockaddr_in6);                                           
    int ret = recvfrom(socket, pacote, sizeof(Frame), 0, &addr, &slen);                     // Recebe pacote.
    if(ret <= 0)    {
        perror("proto-recvfrom");
        return -1;
    }

    if(pacote->header.flag == 1)    {
        printf("Recebido pacote tipo [%d] de [%d].\n", pacote->header.flag, 
                                                            pacote->header.ackNum);
        return pacote->header.ackNum;                                                       // Retorna o seqNum a ser ACKed.
    }
    else    {
        printf("Recebido pacote tipo [%d] de [%d].\n", pacote->header.flag,
                                                            pacote->header.seqNum);
        return pacote->header.seqNum;                                                       // Retorna o seqNum que enviou a mensagem.
    }
}

int
proto_sendto(const int socket, const Frame *pacote, const struct sockaddr_in6 addr)
{
    socklen_t slen = sizeof(struct sockaddr_in6);

    int ret = sendto(socket, pacote, sizeof(Frame), 0, (struct sockaddr *) &addr, slen);    // Envia pacote.
    if(ret < 0)    {
        perror("proto-sendto");
        return -1;
    }

    return ret;                                                                             // Retorna a quantidade de bytes enviados.
}

void
ack_frame(const int seqNum, const int socket, const struct sockaddr_in6 addr)
{
    Frame pacote;
    pacote.header.seqNum = 0;
    pacote.header.ackNum = seqNum;                                                          // Define o seqNum a ser ACKed.
    pacote.header.flag = 1;                                                                 // Define como ackNum o pacote.
    strcpy(pacote.Msg, "\0");

    printf("Enviando ACK do seqNum [%d].\n", seqNum);
    proto_sendto(socket, &pacote, addr);                                                    // Envia pacote de ACK.
    return;
}

