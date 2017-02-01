#ifndef SLID_PROTOCOL_H
    #define SLID_PROTOCOL_H

    #include <stdint.h>

    struct window   {
        uint64_t seqNum;                                                                // Recebe o numero de sequencia do pacote.
        uint64_t ackNum;                                                                // Recebe o numero do pacote acknowledged.
        uint8_t flag;                                                                   // Flag que define tipo da mensagem.
    };

#endif /* SLID_PROTOCOL_H */
