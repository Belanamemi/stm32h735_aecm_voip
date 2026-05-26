#ifndef __VOIP_RTP_H__
#define __VOIP_RTP_H__

#include <stdint.h>
#include "aecm_config.h"

/* ====== RTP HEADER (RFC 3550) ====== */
typedef struct __attribute__((packed)) {
    uint8_t vpxcc;              /* V(2), P(1), X(1), CC(4) */
    uint8_t mpt;                /* M(1), PT(7) */
    uint16_t seq_num;
    uint32_t timestamp;
    uint32_t ssrc;
} RtpHeader_t;

/* ====== RTP PACKET ====== */
typedef struct {
    RtpHeader_t header;
    uint8_t payload[FRAME_SIZE_BYTES];
    uint16_t payload_len;
    uint32_t rx_timestamp;
} RtpPacket_t;

/**
 * @brief Initialize RTP session
 */
void rtp_init(void);

/**
 * @brief Create RTP header
 */
void rtp_create_header(RtpPacket_t *pkt, uint16_t seq, uint32_t ts);

/**
 * @brief Send RTP packet over UDP
 */
int rtp_send(const RtpPacket_t *pkt, const char *dest_ip, uint16_t dest_port);

/**
 * @brief Receive RTP packet
 */
int rtp_recv(RtpPacket_t *pkt);

/**
 * @brief Get current RTP timestamp
 */
uint32_t rtp_get_timestamp(void);

#endif /* __VOIP_RTP_H__ */
