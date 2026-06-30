#include "voip_rtp.h"
#include "itm_telemetry.h"
#include <string.h>

/* Byte-order conversion macros (for embedded systems without arpa/inet.h) */
#define HTONS(x) ((((x) & 0xff00) >> 8) | (((x) & 0x00ff) << 8))
#define HTONL(x) ((((x) & 0xff000000) >> 24) | (((x) & 0x00ff0000) >> 8) | \
                  (((x) & 0x0000ff00) << 8) | (((x) & 0x000000ff) << 24))
#define NTOHS(x) HTONS(x)
#define NTOHL(x) HTONL(x)

/* NetX DUO UDP socket stub (to be implemented with actual NetX) */
static uint16_t rtp_sequence = 0;
static uint32_t rtp_timestamp = 0;

void rtp_init(void) {
    rtp_sequence = 0;
    rtp_timestamp = 0;
    itm_printf(ITM_CHANNEL_RTP, "✅ RTP initialized\n");
}

void rtp_create_header(RtpPacket_t *pkt, uint16_t seq, uint32_t ts) {
    memset(&pkt->header, 0, sizeof(RtpHeader_t));
    
    pkt->header.vpxcc = 0x80;              /* V=2, P=0, X=0, CC=0 */
    pkt->header.mpt = RTP_PAYLOAD_TYPE_G711A;  /* M=0, PT=8 (G.711 a-law) */
    pkt->header.seq_num = HTONS(seq);
    pkt->header.timestamp = HTONL(ts);
    pkt->header.ssrc = HTONL(RTP_SSRC);
}

int rtp_send(const RtpPacket_t *pkt, const char *dest_ip, uint16_t dest_port) {
    /* TODO: Implement NetX UDP socket send */
    itm_printf(ITM_CHANNEL_RTP, "📤 RTP TX: seq=%d, ts=%lu, len=%d\n",
               NTOHS(pkt->header.seq_num), NTOHL(pkt->header.timestamp), pkt->payload_len);
    return 0;
}

int rtp_recv(RtpPacket_t *pkt) {
    /* TODO: Implement NetX UDP socket receive */
    return -1;  /* No packet received */
}

uint32_t rtp_get_timestamp(void) {
    return rtp_timestamp;
}
