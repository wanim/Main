#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "snf_interface.h"


extern SnfStat settings;
extern FILE* outFile;  // output file

const char* protocols[PROTOCOLS_COUNT] =
{
    "UNKNOWN", "ICMP", "IGMP", "GGP", "IPv4", "ST", "TCP",
    "CBT", "EGP", "IGP", "BBN-RCC-MON", "NVP-II", "PUP", "ARGUS",
    "EMCON", "XNET", "CHAOS", "UDP", "MUX", "DCN-MEAS", "HMP"
};

FilT filters_disc[SCAN_MODES_COUNT] =
{
    show_snf_all,
    show_snf_by_source,
    show_snf_by_destination,
    show_snf_by_length,
    show_snf_by_protocol
};

char* err_msg[] =
{
    "\n",
    "Invalid input\n",
    "Can not open file!\n",
    "Connection problems\n"
};

void draw_header()
{
    printf("%-20s %-20s %-12s %-12s\n", CONS_SOURCE, 
                                        CONS_DESTINATION, 
                                        CONS_PROTOCOL, 
                                        CONS_LENGH);
}

void print2console(IPHeader *ip)
{
    char src[16];
    char dst[16];
    const char* protocol;
    uint32_t s1 = ntohl(ip->src.s_addr);
    uint32_t d1 = ntohl(ip->dst.s_addr);

    protocol  = (ip->protocol < PROTOCOLS_COUNT) ? 
                protocols[ip->protocol] : 
                protocols[0];  // transforms id of protocol to its name

    sprintf_s(src, 16, "%d.%d.%d.%d", (uint8_t)(s1 >> 24), (uint8_t)(s1 >> 16), (uint8_t)(s1 >> 8), (uint8_t)s1);
    sprintf_s(dst, 16, "%d.%d.%d.%d", (uint8_t)(d1 >> 24), (uint8_t)(d1 >> 16), (uint8_t)(d1 >> 8), (uint8_t)d1);

    printf("%-20s %-20s %-12s %-8d\n",	src, 
                                        dst, 
                                        protocol, 
                                        ntohs(ip->length));
}

void print2file(IPHeader *ip)
{
    uint16_t bytesCount = ntohs(ip->length);
    fwrite((char*)ip, sizeof(char), bytesCount, outFile);
}

char get_user_input()
{
    char input, chr;
    scanf(" %c", &input);
    while ((chr = (char)getchar()) != '\n' && chr != EOF);  // flush stdin
    return (input - '0');
}

void clear_screen()
{
#ifdef _WIN32
    system("cls");
#else
    // Assume POSIX
    system("clear");
#endif
}

void show_main_menu()
{
    char choice = 1;

    while (choice != 0)
    {
        clear_screen();
        puts("***** IPv4 Packets Sniffer v1.0 by wanim *****\n");
        puts("1\t Sniffer");  // TODO: add some kind of langpack structure
        printf("2\t Output to file on/off (currently %s)\n", settings.o2f_enabled ?
                                                            "ON" :
                                                            "OFF");
        printf("3\t Set output file (currently - '%s')\n", settings.filename);
        puts("0\t Exit\n");
        printf("%s", err_msg[settings.errnum]);  // print error if any
        printf("My choice: ");
        choice = get_user_input();

        switch (choice)
        {
        case 1:
            show_snf_main();
            break;
        case 2:
            show_toggle_o2file();
            break;
        case 3:
            show_set_outfile();
            break;
        case 0:
            puts("\nChau Bambino!");
            return;
        default:
            settings.errnum = ERR_INVALID_INPUT;
        }
    }
}

void show_snf_main()
{
    char choice = 0;
    settings.errnum = ERR_NOERROR;
    clear_screen();
    printf(MSG_SNF_MODES);
    choice = get_user_input();

    if (choice == 0)
    {
        return;
    }
    else if ((uint8_t)choice - 1 < SCAN_MODES_COUNT)
    {
        filters_disc[choice-1]();
        draw_header();
        snf_start_interception(choice-1);
    }
    else
    {
        settings.errnum = ERR_INVALID_INPUT;
    }
}

void show_snf_all()
{
    // intentionally blank
}

void show_snf_by_source()
{
    char src[16];

    printf("Enter source IP: ");
    gets_s(src,16); // TODO: add errcheck
    settings.src = inet_addr(src);
}

void show_snf_by_destination()
{
    char dst[16];

    printf("Enter destination IP: ");
    gets_s(dst, 16);
    settings.dst = inet_addr(dst);
}

void show_snf_by_length()
{
    unsigned length = 0;
    printf("Enter max length of packet(bytes): ");
    scanf("%u", &length);
    settings.length = (uint16_t)length;
}

void show_snf_by_protocol()
{
    unsigned protNum = 1;

    puts("Defined protocols:\n");
    while (protNum < PROTOCOLS_COUNT)
    {
        printf("%d \t%s \n", protNum, protocols[protNum]);
        ++protNum;
    }
    printf("Enter protocol: ");
    scanf(" %u", &protNum);
    settings.protocol = (uint8_t)protNum;
}

void show_toggle_o2file()
{
    char choice;
    clear_screen();
    settings.errnum = ERR_NOERROR;
    puts("1\t Turn ON output to file");
    puts("2\t Turn OFF output to file");
    puts("0\t Return\n");
    printf("My choice: ");
    choice = get_user_input();

    if (choice == 0)
    {
        return;
    }
    else if (choice == 1)
    {
        settings.o2f_enabled = 1;
    }
    else if (choice == 2)
    {
        settings.o2f_enabled = 0;
    }
    else
    {
        settings.errnum = ERR_INVALID_INPUT;
    }
}

void show_set_outfile()
{
    char filename[32];
    clear_screen();
    settings.errnum = ERR_NOERROR;
    printf("Please enter file name: ");
    scanf("%31s", &filename);
    strcpy(settings.filename, filename);
}