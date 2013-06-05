// Author:
//
//
// File: NNSSStation.cpp
//

#include <iostream>

using namespace std;

#include "wlan.h"

//
// WLANAddr
//


// for socket()
#include <sys/socket.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>

// for htons()
#include <arpa/inet.h>

// for ifreq
#include <net/if.h>

#include <errno.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "wlan.h"

//
// WLANAddr
//

char * WLANAddr::wlan2asc()
{
   static char str[32];

   sprintf(str, "%x:%x:%x:%x:%x:%x",
      data[0],data[1],data[2],data[3],data[4],data[5]);

   return str;
}

static int hexdigit(char a)
{
    if (a >= '0' && a <= '9') return(a-'0');
    if (a >= 'a' && a <= 'f') return(a-'a'+10);
    if (a >= 'A' && a <= 'F') return(a-'A'+10);
    return -1;
}

static int sscanf6(char *str, int *a1, int *a2, int *a3, int *a4, int *a5, int *a6)
{
    int n;

    *a1 = *a2 = *a3 = *a4 = *a5 = *a6 = 0;
    while ((n=hexdigit(*str))>=0)
        (*a1 = 16*(*a1) + n, str++);
    if (*str++ != ':') return 1;
    while ((n=hexdigit(*str))>=0)
        (*a2 = 16*(*a2) + n, str++);
    if (*str++ != ':') return 2;
    while ((n=hexdigit(*str))>=0)
        (*a3 = 16*(*a3) + n, str++);
    if (*str++ != ':') return 3;
    while ((n=hexdigit(*str))>=0)
        (*a4 = 16*(*a4) + n, str++);
    if (*str++ != ':') return 4;
    while ((n=hexdigit(*str))>=0)
        (*a5 = 16*(*a5) + n, str++);
    if (*str++ != ':') return 5;
    while ((n=hexdigit(*str))>=0)
        (*a6 = 16*(*a6) + n, str++);

    return 6;
}

Outcome WLANAddr::str2wlan(char * s)
{
   int a[6], i;

   // parse the address
   if (sscanf6(s, a, a+1, a+2, a+3, a+4, a+5) < 6)
   {
      return NOK;
   }

   // make sure the value of every component does not exceed on byte
   for (i=0; i < 6; i++)
   {
      if (a[i] > 0xff) return NOK;
   }

   // assign the result to the "data" data member
   for (i=0; i < 6; i++)
   {
      data[i] = a[i];
   }

   return OK;
}


Outcome Receiver::init(int n)
{
    device = new char[6];
    device = "wlan2";
   // create device level socket
   // - AF_PACKET : packet interface on device level
   // - SOCK_RAW : raw packets including link level header
   // - ETH_P_ALL : all frames will be received
   if ((ifconfig.sockid = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) == -1)
   {
      printf("cannot open socket: %s\n", strerror(errno));
      return NOK;
   }

   // fetch the interface index
   struct ifreq ifr;
   strcpy(ifr.ifr_name, device);
   if (ioctl(ifconfig.sockid, SIOGIFINDEX, &ifr) < 0)
   {
      printf("failed to fetch ifindex: %s\n", strerror(errno));
      return NOK;
   }
   ifconfig.ifindex = ifr.ifr_ifindex;
   printf("ifindex: %d\n", ifconfig.ifindex);

   // fetch the hardware address
   if (ioctl(ifconfig.sockid, SIOCGIFHWADDR, &ifr) == -1)
   {
      printf("failed to fetch hardware address: %s\n", strerror(errno));
      return NOK;
   }
   memcpy(&ifconfig.hwaddr.data, &ifr.ifr_hwaddr.sa_data, WLAN_ADDR_LEN);
   printf("hwaddr: %s\n", ifconfig.hwaddr.wlan2asc());

   // fetch the MTU
   if (ioctl(ifconfig.sockid, SIOCGIFMTU, &ifr) == -1)
   {
      printf("WLANProtocol, failed to the MTU: %s\n", strerror(errno));
      return NOK;
   }
   ifconfig.mtu = ifr.ifr_mtu;
   printf("MTU: %d\n", ifconfig.mtu);

   // add the promiscuous mode
   struct packet_mreq mr;
   memset(&mr,0,sizeof(mr));
   mr.mr_ifindex = ifconfig.ifindex;
   mr.mr_type =  PACKET_MR_PROMISC;
   if (setsockopt(ifconfig.sockid, SOL_PACKET, PACKET_ADD_MEMBERSHIP,
      (char *)&mr, sizeof(mr)) < 0)
   {
      printf("WLANProtocol, failed to add the promiscuous mode: %s\n",
         strerror(errno));
      return NOK;
   }

   // bind the socket to the interface
   // only traffic from that interface will be recieved
   struct sockaddr_ll sll;
   memset(&sll, 0, sizeof(sll));
   sll.sll_family = AF_PACKET;
   sll.sll_ifindex = ifconfig.ifindex;
   sll.sll_protocol = htons(ETH_P_ALL);
   if (bind(ifconfig.sockid, (struct sockaddr*)&sll, sizeof(sll)) < 0)
   {
      printf("WLANProtocol, failed to bind the socket: %s\n", strerror(errno));
      return NOK;
   }
   numAP = n;
   return OK;
}

// receive data over a socket
SignalStrength* Receiver::Receive()
{
   // pointer to received data
   unsigned char * buff = new unsigned char[ifconfig.mtu];
   unsigned int i; // frame length
   struct sockaddr_ll from; // source address of the  message
   socklen_t fromlen = sizeof(struct sockaddr_ll);
   int error;

   // infinite loop
   while (true)
   {
      // loop until a non-empty frame has been received on "device"
      while (true)
      {
         // wait and receive a frame
         fromlen = sizeof(from);
         i = recvfrom(ifconfig.sockid, buff, ifconfig.mtu, 0,
            (struct sockaddr *) &from, &fromlen);
         if (i == -1)
         {
            printf("cannot receive data: %s\n", strerror(errno));
            // sleep for 10 milliseconds before re-trying
            usleep(10000);
         }
     else
     {
            break; // exit the loop
     }
      }
      cout << "frame received\n" << flush;
      //printf("frame received\n");

      char asciiSrcAddr[32], asciiDestAddr[32];

      // casting to the WLAN header format
      WLANHeader * wlanHdr = (WLANHeader *) buff;

      char * dest = wlanHdr->destAddr.wlan2asc();
      strcpy(asciiDestAddr, dest);

      char * src = wlanHdr->srcAddr.wlan2asc();
      strcpy(asciiSrcAddr, src);
      //Only convert data that were to itself
      if(strcmp(asciiDestAddr, ifconfig.hwaddr.wlan2asc()) == 0){
        SignalStrength * accessPoints = new SignalStrength[numAP];
        accessPoints = (SignalStrength *)(buff + WLAN_HEADER_LEN);
        cout << accessPoints[0].address << " "<< accessPoints[1].address << flush << endl;
        return accessPoints;
      }
   }
}

void Receiver::shutdown()
{
   printf("shutdown\n");
   // close the socket
   if (ifconfig.sockid != -1) close(ifconfig.sockid);
}

