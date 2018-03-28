#ifndef XERXES_HEADERS_H
#define XERXES_HEADERS_H

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>



/* Structure of a TCP header */

struct tcpheader {

    unsigned short int tcph_srcport;

    unsigned short int tcph_destport;

    unsigned int       tcph_seqnum;

    unsigned int       tcph_acknum;

    unsigned char      tcph_reserved:4, tcph_offset:4;

    // unsigned char tcph_flags;

    unsigned int

            tcp_res1:4,

            tcph_hlen:4,      /*length of tcp header in 32-bit words*/

            tcph_fin:1,       /*Finish flag "fin"*/

            tcph_syn:1,       /*Synchronize sequence numbers to start a connection*/

            tcph_rst:1,       /*Reset flag */

            tcph_psh:1,       /*Push, sends data to the application*/

            tcph_ack:1,       /*acknowledge*/

            tcph_urg:1,       /*urgent pointer*/

            tcph_res2:2;

    unsigned short int tcph_win;

    unsigned short int tcph_chksum;

    unsigned short int tcph_urgptr;

};

struct ipheader {
    unsigned char      iph_ihl:5, iph_ver:4;

    unsigned char      iph_tos;

    unsigned short int iph_len;

    unsigned short int iph_ident;

    unsigned char      iph_flag;

    unsigned short int iph_offset;

    unsigned char      iph_ttl;

    unsigned char      iph_protocol;

    unsigned short int iph_chksum;

    unsigned int       iph_sourceip;

    unsigned int       iph_destip;
};


// UDP header's structure
struct udpheader {

    unsigned short int udph_srcport;

    unsigned short int udph_destport;

    unsigned short int udph_len;

    unsigned short int udph_chksum;

};

unsigned short checksum(unsigned short *buf, int len){

    unsigned long sum;

    for(sum=0; len>0; len--){
        sum += *buf++;
    }

    sum = (sum >> 16) + (sum &0xffff);

    sum += (sum >> 16);

    return (unsigned short)(~sum);

}


#endif //XERXES_HEADERS_H
