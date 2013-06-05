// 802.11 Programming example
// Author: Michel Barbeau, January 2004
// Update: Rafael Guerra, April 2013
// File: wlan.hpp
//
#ifndef RECEIVER
#define RECEIVER
typedef int Outcome;
#define OK 0
#define NOK -1

#define MAX_NAME_LEN 128

// Declaration of struct WLANAddr.
#define WLAN_ADDR_LEN 6

class WLANAddr
{
public:
   // address
   unsigned char data[WLAN_ADDR_LEN];
   // return the address in a human readable form
   char * wlan2asc();
   // defined the address from a human readable form
   Outcome str2wlan(char s[]);


};

struct SignalStrength
{
   char address[20]; //MAC addres of a Access Point
   int strength; //signal strength in dB
   bool update; // if the signal strength is update
};


// format of an WLAN header
struct WLANHeader
{
   WLANAddr destAddr;
   WLANAddr srcAddr;
   unsigned short type;
   // make IP different from the real IP number (x0800)
   // types x3*** are not reserved
   #define IP_TYPE 0x3901


};
#define WLAN_HEADER_LEN 14 // bytes

// wireless interface configuration
struct Ifconfig
{
   int sockid; // socket descriptor
   int ifindex; // index of the interface
   WLANAddr hwaddr; // MAC address
   int mtu; // maximum transmission unit
};

struct Neighbor
{
    //Neighbor address
    char address[32];
    //message arrival
    bool arrival;
    //last time recorded
    int lastRecorded;
};




static int hexdigit(char a);
static int sscanf6(char *str, int *a1, int *a2, int *a3, int *a4, int *a5, int *a6);

// Global variables
class Receiver{
public:
    // label of device, e.g. "eth0"
    char* device;
    // interface configuration
    Ifconfig ifconfig;
    // prototype header
    WLANHeader hdr;
    //number of Access Points
    int numAP;


    Outcome init(int n);
    // receive data over a socket
    SignalStrength* Receive();
    void shutdown();
};

#endif
