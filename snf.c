#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "snf.h"
#include "snf_interface.h"
#include <conio.h> // for _kbhit(), will be changed (probably)


char Buffer[MAX_PACKET_SIZE];   // buffer for packet
char        nameBuff[128];      // host name
WSADATA     wsadata;            // WinSock init
SOCKET      snf_socket;         // listening socket
HOSTENT*    hostent;            // host info
SOCKADDR_IN host_addr;          // host addr
FILE*       outFile;            // output file
SnfStat     settings;           // current settings of sniffer
IPHeader*   iphdr;
unsigned long flag = 1;         // promiscuous mode on\off


SnfT snf_filter[] =
{
    snf_sniff_all,
    snf_sniff_by_src,
    snf_sniff_by_dest,
    snf_sniff_by_packet_length,
    snf_sniff_by_protocol
};

void snf_init()
{
    WSAStartup(MAKEWORD(2, 2), &wsadata);
    snf_socket = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
    gethostname(nameBuff, sizeof(nameBuff));
    hostent = gethostbyname(nameBuff);
    memset(&host_addr, 0, sizeof(host_addr));
    host_addr.sin_family = AF_INET;
    host_addr.sin_addr.s_addr = ((struct in_addr *)hostent->h_addr_list[0])->s_addr;

    bind(snf_socket, (SOCKADDR *)&host_addr, sizeof(SOCKADDR));
    ioctlsocket(snf_socket, SIO_RCVALL, &flag);  // Turn on promiscuous mode.

    strcpy(settings.filename, "sniffer_output.raw");  // default output file
}

void snf_shdwn()
{
    closesocket(snf_socket);
    WSACleanup();
}

IPHeader* snf_grabPacket()
{
    int bnum;
    bnum = recv(snf_socket, Buffer, sizeof(Buffer), 0);  // grab IP packet

    if (bnum >= (int)(sizeof(IPHeader)))
    {
        IPHeader* hdr = (IPHeader*)Buffer;
        return hdr;
    }
    return NULL;
}

void snf_start_interception(char mode)
{
    if (settings.o2f_enabled)
    {
        outFile = fopen(settings.filename, "w");
        if (!outFile)
        {
            settings.errnum = ERR_CANT_OPEN_FILE;
            return;
        }
    }
    while (!_kbhit()) 
    {
        iphdr = snf_grabPacket();

        if (iphdr)
        {
            if (snf_filter[mode]())  // evaluate depending on filther mode
            {
                print2console(iphdr);

                if (settings.o2f_enabled)
                {
                    print2file(iphdr);
                }
            }		
        }
    }
    if (settings.o2f_enabled)
    {
        fclose(outFile);
    }
}

uint8_t snf_sniff_all()
{
    return 1;
}

uint8_t snf_sniff_by_src()
{
    return (iphdr->src.s_addr == settings.src) ? 1 : 0;
}

uint8_t snf_sniff_by_dest()
{
    return (iphdr->dst.s_addr == settings.dst) ? 1 : 0;
}

uint8_t snf_sniff_by_packet_length()
{
    return (htons(iphdr->length) <= settings.length) ? 1 : 0;
}

uint8_t snf_sniff_by_protocol()
{
    return (iphdr->protocol == settings.protocol) ? 1 : 0;
}
