#ifndef SNF_INTERFACE_H_INCLUDED
#define SNF_INTERFACE_H_INCLUDED

#include "snf.h"

#define PROTOCOLS_COUNT 21
#define SCAN_MODES_COUNT 5

// error codes
#define ERR_NOERROR 0
#define ERR_INVALID_INPUT 1
#define ERR_CANT_OPEN_FILE 2

// TODO: move all messages in one place
#define MSG_SNF_MODES "Sniffer modes: \n1 \tIntercept All \n2 \tFilter by source IP \n3 \tFilter by destination IP \n4 \tFilter by length of packet \n5 \tFilter by protocol \n0 \tReturn \n\nMy choice: "
#define CONS_SOURCE "Source:"
#define CONS_DESTINATION "Destination:"
#define CONS_PROTOCOL "Protocol:"
#define CONS_LENGH "Lengh:"

// typedef for filter mode specific function
typedef void(__cdecl *FilT)();  

// Draws columns description line
void draw_header();

// Prints to console
void print2console(IPHeader *ip);

// Prints to file
void print2file(IPHeader *ip);

// Gets exactly one char from user. Flushes everything else
char get_user_input();

// Show main menu of program
void show_main_menu();

// Shows available sniffing modes
void show_snf_main();

// Shows info specific to no filter mode
void show_snf_all();

// Shows filter by source IP specific info
void show_snf_by_source();

// Shows filter by destination IP specific info
void show_snf_by_destination();

// Shows filter by packet length specific info 
void show_snf_by_length();

// Shows filter by protocol specific info
void show_snf_by_protocol();

// Shows menu enabling output to file
void show_toggle_o2file();

// Shows menu that enables setting output file name
void show_set_outfile();

#endif // SNF_INTERFACE_H_INCLUDED