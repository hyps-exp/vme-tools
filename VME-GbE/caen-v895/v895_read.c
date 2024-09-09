// test utility for VME-GbE
// Author: Shuhei Ajimura, RCNP, Osaka University

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "vmeg_lib.h"

#define BASEADDR 0xdd000000
#define ADDRVER  0x000000fe

int main(int argc, char *argv[])
{
  char *IpAddr;
  int IpPort;
  int sock;
  char line[100];
  int loop;
  int length;
  int ret;
  unsigned short data16[128];
  int i,j;
  unsigned int address;
  /* for time */
  struct timespec ts0, ts1;
  double t0,t1;
  unsigned short ver;
  int waitcount;

  if (argc<3){
    printf("vme_test <ipaddr> <ipport>\n");
    exit(0);
  }

  IpAddr=argv[1];
  IpPort=atoi(argv[2]);

  if ((sock=VMEGopen(IpAddr, IpPort))<0) exit(1);

  /* get current status */
  address=BASEADDR+ADDRVER;
  if ((ret=VMEGread(sock,A24|D16,address,&ver,2))<0){
    printf("Error!\n");
    VMEGclose(sock);
    exit(-1);
  }
  waitcount=0;
  /* wait "not empty" */
  address=BASEADDR+ADDRVER;
  if ((ret=VMEGread(sock,A24|D16,address,&ver,2))<0){
    printf("Error!\n");
    VMEGclose(sock);
    exit(-1);
  }
  clock_gettime(CLOCK_MONOTONIC,&ts0);

  /* print data */
  printf("version : %04x\n", (ver>>12)&  0xf);
  printf("serial  : %04x\n", (ver>> 0)&0xfff);

  VMEGclose(sock);
  exit(0);
}
