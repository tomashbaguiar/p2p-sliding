#ifndef SLID_PROTOCOL_H
    #define SLID_PROTOCOL_H

    #include <stdint.h>
    #include <net/socket.h>
    #include <netinet/in.h>

    /*  Estrutura que define o cabeçalho do protocolo   */
    typedef struct window   {
        uint64_t seqNum;                                                                // Recebe o numero de sequencia do pacote.
        uint64_t ackNum;                                                                // Recebe o numero do pacote acknowledged.
        uint8_t flag;                                                                   // Flag que define tipo da mensagem.
    }
    SWheader;

    /*  Estrutura que guarda argumentos a serem passados a thread   */
    typedef struct thread_arg   {
        int socket;                                                                     // Recebe o descritor do socket de comunicacao.
        struct sockaddr_in6 addr;                                                       // Recebe as informaçoes do peer.
        uint64_t seqNum;                                                                // Recebe o numero de sequencia do pacote.
        char *message;                                                                  // Ponteiro para o buffer contendo a mensagem.
    }
    Thread_arg;

    /*  Estrutura que guarda o pacote a ser enviado */
    typedef struct frame    {
        SWheader header;                                                                // Recebe o cabeçalho do pacote a ser enviado.
        char *Msg;                                                                      // Recebe a mensagem a ser enviada.
    }
    Frame;

    void *frame_send(void *arg);                                                        // Procedimento thread que realiza a contagem do tempo e reenvio.

#endif /* SLID_PROTOCOL_H */
