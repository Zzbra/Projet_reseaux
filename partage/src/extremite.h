#ifndef H_extremite
#define H_extremite

int ext_out(char* port, int fd);
int ext_in(char* ipServ, char* port, int fd);
void asyncInOut(char *ipOut, char* portOut, char *portIn, int fdTun);
#endif
