// -*- C++ -*-

#include "bbt-vme-gbe.h"

#include <string>
#include <cstring>

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <time.h>

#define DEBUG 0

namespace bbt
{
namespace vmeg
{
int sock;

struct packet_buf {
  unsigned int address;
  unsigned int length;
  unsigned short mode;
  unsigned char id;
  unsigned char crc;
  unsigned char data[MAXBUFSIZ];
};
unsigned char ID=0;

unsigned char CRCTbl [] = {
  0x00, 0x07, 0x0e, 0x09, 0x1c, 0x1b, 0x12, 0x15,   0x38, 0x3f, 0x36, 0x31, 0x24, 0x23, 0x2a, 0x2d,
  0x70, 0x77, 0x7e, 0x79, 0x6c, 0x6b, 0x62, 0x65,   0x48, 0x4f, 0x46, 0x41, 0x54, 0x53, 0x5a, 0x5d,
  0xe0, 0xe7, 0xee, 0xe9, 0xfc, 0xfb, 0xf2, 0xf5,   0xd8, 0xdf, 0xd6, 0xd1, 0xc4, 0xc3, 0xca, 0xcd,
  0x90, 0x97, 0x9e, 0x99, 0x8c, 0x8b, 0x82, 0x85,   0xa8, 0xaf, 0xa6, 0xa1, 0xb4, 0xb3, 0xba, 0xbd,
  0xc7, 0xc0, 0xc9, 0xce, 0xdb, 0xdc, 0xd5, 0xd2,   0xff, 0xf8, 0xf1, 0xf6, 0xe3, 0xe4, 0xed, 0xea,
  0xb7, 0xb0, 0xb9, 0xbe, 0xab, 0xac, 0xa5, 0xa2,   0x8f, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9d, 0x9a,
  0x27, 0x20, 0x29, 0x2e, 0x3b, 0x3c, 0x35, 0x32,   0x1f, 0x18, 0x11, 0x16, 0x03, 0x04, 0x0d, 0x0a,
  0x57, 0x50, 0x59, 0x5e, 0x4b, 0x4c, 0x45, 0x42,   0x6f, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7d, 0x7a,
  0x89, 0x8e, 0x87, 0x80, 0x95, 0x92, 0x9b, 0x9c,   0xb1, 0xb6, 0xbf, 0xb8, 0xad, 0xaa, 0xa3, 0xa4,
  0xf9, 0xfe, 0xf7, 0xf0, 0xe5, 0xe2, 0xeb, 0xec,   0xc1, 0xc6, 0xcf, 0xc8, 0xdd, 0xda, 0xd3, 0xd4,
  0x69, 0x6e, 0x67, 0x60, 0x75, 0x72, 0x7b, 0x7c,   0x51, 0x56, 0x5f, 0x58, 0x4d, 0x4a, 0x43, 0x44,
  0x19, 0x1e, 0x17, 0x10, 0x05, 0x02, 0x0b, 0x0c,   0x21, 0x26, 0x2f, 0x28, 0x3d, 0x3a, 0x33, 0x34,
  0x4e, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5c, 0x5b,   0x76, 0x71, 0x78, 0x7f, 0x6a, 0x6d, 0x64, 0x63,
  0x3e, 0x39, 0x30, 0x37, 0x22, 0x25, 0x2c, 0x2b,   0x06, 0x01, 0x08, 0x0f, 0x1a, 0x1d, 0x14, 0x13,
  0xae, 0xa9, 0xa0, 0xa7, 0xb2, 0xb5, 0xbc, 0xbb,   0x96, 0x91, 0x98, 0x9f, 0x8a, 0x8d, 0x84, 0x83,
  0xde, 0xd9, 0xd0, 0xd7, 0xc2, 0xc5, 0xcc, 0xcb,   0xe6, 0xe1, 0xe8, 0xef, 0xfa, 0xfd, 0xf4, 0xf3
};

unsigned char calc_crc(void *buf,unsigned int len) {
  unsigned int i;
  unsigned char crc;
  unsigned char *ptr = (unsigned char *)buf;
  crc = 0xff;
  for (i = 0; i < len; i++) crc = CRCTbl[crc^*ptr++];
  return crc;
}

unsigned char calc_crc11(void* buf) {
  return calc_crc(buf, 11);
}

int open(const std::string& host, int port) {
  struct sockaddr_in SockAddr;
  int one;

  sock = ::socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
  SockAddr.sin_family = AF_INET;
  SockAddr.sin_port = htons(port);
  SockAddr.sin_addr.s_addr = ::inet_addr(host.c_str());

  one=1;
  ::setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));

  if (::connect(sock,(struct sockaddr *)&SockAddr, sizeof(SockAddr)) < 0) {
    printf("fail in connecting socket...\n");
    return -1;
  }
  return sock;
}

void close() {
  ::close(sock);
}

int write(unsigned short mode, unsigned int address,
          void *data, int bytes) {
  struct packet_buf buf;
  unsigned char *buftop;
  unsigned char *buf8;
  unsigned short *buf16;
  unsigned int *buf32;
  unsigned int recv_len;
  unsigned int ret;
  int i;

  /* set pointer for data */
  buftop=(unsigned char *)&buf;
  buf8=(unsigned char *)(buftop+HEADER_SIZE);
  buf16=(unsigned short *)(buftop+HEADER_SIZE);
  buf32=(unsigned int *)(buftop+HEADER_SIZE);

  /* create packet header */
  buf.address=htonl(address);
  buf.length =htonl((unsigned int)(bytes));
  mode|=0x8000;
  buf.mode=htons(mode);
  buf.id=ID;
  buf.crc=calc_crc11(&buf);

  /* put data */
  if ((mode&0x0c00)==D8){
    std::memcpy(buf8, data, bytes);
  }else if ((mode&0x0c00)==D16){
    for(i=0;i<bytes/2;i++) *(buf16+i)=htons(*((unsigned short *)data+i));
  }else{
    for(i=0;i<bytes/4;i++) *(buf32+i)=htonl(*((unsigned int *)data+i));
  }
  /* send packet */
#if DEBUG
  printf("send packet -- "); for(i=0;i<(HEADER_SIZE+bytes);i++) printf("%02x ",*(buftop+i)); printf("\n");
#endif
  if (::send(sock, buftop, HEADER_SIZE+bytes, 0) < 0) {
    printf("packet send failed\n");
    return -1;
  }
  /* receive header packet */
  recv_len=0;
  while(recv_len<HEADER_SIZE){
    if ((ret = ::recv(sock, buftop+recv_len, HEADER_SIZE-recv_len, 0)) <= 0) {
      printf("packet recv failed\n");
      return -1;
    }
    recv_len+=ret;
  }
#if DEBUG
  printf("recv header packet -- "); for(i=0;i<HEADER_SIZE;i++) printf("%02x ",*(buftop+i)); printf("\n");
#endif
  /* check error status */
  if (buf.id!=ID){                    printf("ID check NG\n");    return -1;  }
  if (calc_crc11(&buf)!=buf.crc){     printf("CRC check NG\n");   return -1;  }
  if (buf.mode&0x0100){               printf("corrupt packet\n"); return -1;  }
  if (buf.mode&0x0400){               printf("VME timeout\n");    return -1;  }
  ID++;
  return 0;
}

int read(unsigned short mode, unsigned int address,
         void *data, int bytes) {
  struct packet_buf buf;
  unsigned char *buftop;
  unsigned char *buf8;
  unsigned short *buf16;
  unsigned int *buf32;
  unsigned int recv_len;
  unsigned int ret;
  int i;
#if DEBUG
  struct timespec ts0, ts1, ts2, ts3, ts4;
  double t0,t1,t2,t3,t4;
#endif
  /* set pointer for data */
  buftop=(unsigned char *)&buf;
  buf8=(unsigned char *)(buftop+HEADER_SIZE);
  buf16=(unsigned short *)(buftop+HEADER_SIZE);
  buf32=(unsigned int *)(buftop+HEADER_SIZE);

  /* create packet header */
  buf.address=htonl(address);
  buf.length =htonl((unsigned int)(bytes));
  buf.mode=htons(mode);
  buf.id=ID;
  buf.crc=calc_crc11(&buf);

  /* send packet */
#if DEBUG
  printf("send packet -- "); for(i=0;i<HEADER_SIZE;i++) printf("%02x ",*(buftop+i)); printf("\n");
  clock_gettime(CLOCK_MONOTONIC,&ts0);
#endif
  if (::send(sock, buftop, HEADER_SIZE, 0) < 0) {
    printf("packet send failed\n");
    return -1;
  }
  /* receive packet */
  recv_len=0;
#if DEBUG
  clock_gettime(CLOCK_MONOTONIC,&ts1);
#endif
  while(recv_len<HEADER_SIZE){
    if ((ret = ::recv(sock, buftop+recv_len, HEADER_SIZE-recv_len, 0)) <= 0) {
      printf("packet recv failed\n");
      return -1;
    }
    recv_len+=ret;
  }
#if DEBUG
  clock_gettime(CLOCK_MONOTONIC,&ts2);
  printf("recv header packet -- "); for(i=0;i<HEADER_SIZE;i++) printf("%02x ",*(buftop+i)); printf("\n");
#endif
  /* check error status */
  if (buf.id!=ID){                    printf("ID check NG\n");    return -1;  }
  if (calc_crc11(&buf)!=buf.crc){     printf("CRC check NG\n");   return -1;  }
  if (buf.mode&0x0100){               printf("corrupt packet\n"); return -1;  }
  if (buf.mode&0x0400){               printf("VME timeout\n");    return -1;  }
  if (ntohl(buf.length)!=(unsigned int)bytes){      printf("size diff.\n");     return -1;  }
  /* receive data packet */
#if DEBUG
  clock_gettime(CLOCK_MONOTONIC,&ts3);
#endif
  recv_len=0;
  while(recv_len<(unsigned int)bytes){
    if ((ret = ::recv(sock, buf8+recv_len, bytes-recv_len, 0)) <= 0) {
      printf("packet recv failed\n");
      return -1;
    }
    recv_len+=ret;
  }
#if DEBUG
  clock_gettime(CLOCK_MONOTONIC,&ts4);
#endif
  if ((mode&0x0c00)==D8){
    std::memcpy(data,buf8,bytes);
  }else if ((mode&0x0c00)==D16){
    for(i=0;i<bytes/2;i++)
      *((unsigned short *)data+i)=ntohs(*(buf16+i));
  }else{
    for(i=0;i<bytes/4;i++)
      *((unsigned int *)data+i)=ntohl(*(buf32+i));
  }
#if DEBUG
  printf("recv data packet -- "); for(i=0;i<bytes;i++) printf("%02x ",*(buf8+i)); printf("\n");
  t0=(ts0.tv_sec*1.)+(ts0.tv_nsec/1000000000.);
  t1=(ts1.tv_sec*1.)+(ts1.tv_nsec/1000000000.);
  t2=(ts2.tv_sec*1.)+(ts2.tv_nsec/1000000000.);
  t3=(ts3.tv_sec*1.)+(ts3.tv_nsec/1000000000.);
  t4=(ts4.tv_sec*1.)+(ts4.tv_nsec/1000000000.);
  printf("time: %lf %lf %lf %lf\n",t1-t0,t2-t1,t3-t2,t4-t3);
#endif
  ID++;
  return 0;
}
} // namespace vmeg
} // namespace bbt
