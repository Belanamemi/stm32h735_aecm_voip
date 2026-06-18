#ifndef VOIP_RTP_HANDLER_H
#define VOIP_RTP_HANDLER_H

#include <stdint.h>

#define RTP_UDP_PORT 5004
#define RTP_PAYLOAD_SIZE 160

typedef struct {
    uint8_t version:2;
    uint8_t padding:1;
    uint8_t extension:1;
    uint8_t csrc_count:4;
    uint8_t marker:1;
    uint8_t payload_type:7;
    uint16_t seq_num;
    uint32_t timestamp;
    uint32_t ssrc;
} RtpHeader_t;

typedef struct {
    RtpHeader_t header;
    uint8_t payload[RTP_PAYLOAD_SIZE];
    uint16_t payload_len;
} RtpPacket_t;

void rtp_init(void);
void rtp_create_header(RtpPacket_t *pkt, uint16_t seq, uint32_t ts);
int rtp_recv(RtpPacket_t *pkt);
int rtp_send(RtpPacket_t *pkt, const char *dest_ip, uint16_t dest_port);

#endif
