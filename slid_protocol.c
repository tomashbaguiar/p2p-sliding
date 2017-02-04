#include "slid_protocol.h"
#include <errno.h>
#include <unistd.h>
#include <stdio.h>

void *
frame_send(void *arg)
{
    Frame pacote;                                                                           // Pacote a ser enviado.
    Thread_arg *args = (Thread_arg *) arg;                                                // Recebe os argumentos.

    /*  Coloca as informaçoes no pacote */
    pacote.header.seqNum = args->seqNum;
    pacote.header.ackNum = 0;
    pacote.header.flag = 0;                                                                 // Define como seqNum + Msg o pacote.
    pacote.Msg = args->message;

    /*  Loop ate a thread ser terminada no programa principal   */
    while(1)    {
        printf("Enviando pacote de seqNum [%d].\n", pacote.header.seqNum);
        proto_sendto(args->socket, &pacote, args->addr);
        sleep(TIMER);
    }
}

int
proto_recvfrom(const int socket, const struct sockaddr_in6 addr, Frame *pacote)
{
    socklen_t slen = sizeof(struct sockaddr_in6);
    int ret = recvfrom(socket, pacote, sizeof(Frame), 0, &addr, &slen);
    if(ret <= 0)    {
        perror("proto-recvfrom");
        return -1;
    }

    printf("Recebido pacote tipo [%d]\n", pacote->header.flag);

    if(pacote->header.flag == 1)
        return pacote->header.ackNum;
    else if(pacote->header.flag == 0)
        return pacote->header.seqNum;
}

int
proto_sendto(const int socket, const Frame *pacote, const struct sockaddr_in6 addr)
{
    socklen_t slen = sizeof(struct sockaddr_in6);

printf("Enviando [%s].\n", pacote->Msg);

    int ret = sendto(socket, pacote, sizeof(Frame), 0, (struct sockaddr *) &addr, slen);
    if(ret < 0)    {
        perror("proto-sendto");
        return -1;
    }

    return ret;
}

void
ack_frame(const int seqNum, const int socket, const struct sockaddr_in6 addr)
{
    Frame pacote;
    pacote.header.seqNum = 0;
    pacote.header.ackNum = seqNum;
    pacote.header.flag = 1;                                                                 // Define como ackNum o pacote.
    pacote.Msg = NULL;

    printf("Enviando ACK do seqNum [%d].\n", seqNum);
    proto_sendto(socket, &pacote, addr);
    return;
}

