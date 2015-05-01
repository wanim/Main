#ifndef SNF_H_INCLUDED
#define SNF_H_INCLUDED

#include <stdint.h>
#include <winsock2.h>

#pragma comment (lib, "ws2_32.lib")

#define MAX_PACKET_SIZE 0x10000         // 64 Kb
#define SIO_RCVALL      0x98000001      // control code that enables receiving of all IPv4 or IPv6 packets

// Interface to sniffer settings
typedef struct _SnfStat
{
    char sniff_mode;
    uint32_t src;
    uint32_t dst;
    uint16_t length;
    uint8_t  protocol;
    char o2f_enabled;
    uint8_t errnum;
    char filename[32];
}SnfStat;

// Interface to IPv4 packet header
typedef struct _IPHeader
{
    uint8_t  verlen;    // version and headef lengh
    uint8_t  tos;       // type of service
    uint16_t length;    // total lengh of packet
    uint16_t id;
    uint16_t offset;    // flags and offset
    uint8_t  ttl;       // time to live
    uint8_t  protocol;
    uint16_t xsum;      // checksum
    struct in_addr  src;        // source IP
    struct in_addr  dst;        // destination IP
} IPHeader;

// typedef for filter functions
typedef uint8_t(__cdecl *SnfT)();  

// Initialize raw socket
void snf_init();

// Clean-ups socket
void snf_shdwn();

// Intercepts IP packets
IPHeader* snf_grabPacket();

// Starts sniffing and outputting dependind on filther mode
void snf_start_interception(char mode);

// Intercept everything
uint8_t snf_sniff_all();

// Filter for interception by source IP
uint8_t snf_sniff_by_src();

// Filter for interception by destination IP
uint8_t snf_sniff_by_dest();

// Filter for interception by packet length
uint8_t snf_sniff_by_packet_length();

// Filter for interception by protocol
uint8_t snf_sniff_by_protocol();

#endif // SNF_H_INCLUDED