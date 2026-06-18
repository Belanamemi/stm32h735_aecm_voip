#include "voip_rtp_handler.h"
#include <string.h>
#include <arpa/inet.h>

static uint16_t rtp_seq = 0;
static uint32_t rtp_ts = 0;

void rtp_init(void) {
    rtp_seq = 0;
    rtp_ts = 0;
}

void rtp_create_header(RtpPacket_t *pkt, uint16_t seq, uint32_t ts) {
    pkt->header.version = 2;
    pkt->header.padding = 0;
    pkt->header.extension = 0;
    pkt->header.csrc_count = 0;
    pkt->header.marker = 0;
    pkt->header.payload_type = 8;  /* G.711 A-law */
    pkt->header.seq_num = htons(seq);
    pkt->header.timestamp = htonl(ts);
    pkt->header.ssrc = htonl(0x12345678);
}

int rtp_recv(RtpPacket_t *pkt) {
    /* Placeholder for RTP reception */
    return -1;
}

int rtp_send(RtpPacket_t *pkt, const char *dest_ip, uint16_t dest_port) {
    /* Placeholder for RTP transmission */
    return 0;
}
