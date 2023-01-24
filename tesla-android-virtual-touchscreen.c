#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ws.h>

int pingFrameCounter = 0;

void webSocketOnConnectionOpened(ws_cli_conn_t *client) {
  char *cli;
  cli = ws_getaddress(client);
  printf("Connection opened, addr: %s\n", cli);
}

void webSocketOnConnectionClosed(ws_cli_conn_t *client) {
  char *cli;
  cli = ws_getaddress(client);
  printf("Connection closed, addr: %s\n", cli);
}

void webSocketOnMessage(__attribute__ ((unused)) ws_cli_conn_t *client,
       const unsigned char *msg, __attribute__ ((unused)) uint64_t size, __attribute__ ((unused)) int type) {
  FILE *fp;
  fp = fopen("/dev/virtual_touchscreen", "w+");
  fputs((const char*) msg, fp);
  fclose(fp);
  if(pingFrameCounter == 150) {
    ws_ping(NULL, 5);
  } else {
    pingFrameCounter = 0;
  }
  pingFrameCounter++;
}

int main(void)
{
  struct ws_events evs;
  evs.onopen    = &webSocketOnConnectionOpened;
  evs.onclose   = &webSocketOnConnectionClosed;
  evs.onmessage = &webSocketOnMessage;
  ws_socket(&evs, 9999, 0, 1000);
  return (0);
}
