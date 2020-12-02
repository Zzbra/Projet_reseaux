#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "extremite.h"
#include "iftun.h"


typedef struct Conf {
  char *nameTun;
  char *inIp;
  char *inPort;
  char *options;
  char *outIp;
  char *outPort;
} Conf;

char* getValue(char* line){
  int i;
  for(i = 0; line[i] != '='; i++);
  i++;
  char* value = malloc(sizeof(char)*(strlen(line)-i));
  strncpy(value, line+i, strlen(line)-1);
  value[strlen(value)-1] = '\0';
  return value;
}

Conf* readConfFile(char* filename){
  Conf* confFile = (Conf*)malloc(sizeof(Conf));
  FILE* file = fopen(filename, "r");

  if(file == NULL){
    printf("Erreur ouverture fichier configration\n");
    return NULL;
  }

  char* line = NULL;
  size_t len;
  ssize_t read;
  int i = 0;
  while((read = getline(&line, &len, file)) != -1){

    /* On saute les commentaires */
    if(line[0] == '#'){
      continue;
    }
    char* value = getValue(line);
    switch (i) {
      case 0:
        confFile->nameTun = (char*)malloc(sizeof(char)*strlen(value));
        strcpy(confFile->nameTun, value);
        break;
      case 1:
        confFile->inIp = (char*)malloc(sizeof(char)*strlen(value));
        strcpy(confFile->inIp, value);
        break;
      case 2:
        confFile->inPort = (char*)malloc(sizeof(char)*strlen(value));
        strcpy(confFile->inPort, value);
        break;
      case 3:
        confFile->options = (char*)malloc(sizeof(char)*strlen(value));
        strcpy(confFile->options, value);
        break;
      case 4:
        confFile->outIp = (char*)malloc(sizeof(char)*strlen(value));
        strcpy(confFile->outIp, value);
        break;
      case 5:
        confFile->outPort = (char*)malloc(sizeof(char)*strlen(value));
        strcpy(confFile->outPort, value);
        break;
    }
    i++;
  }
  return confFile;
}

void printConf(Conf *conf){
  printf("name : %s\n", conf->nameTun);
  printf("OUT IP : %s\n", conf->outIp);
  printf("OUT PORT : %s\n", conf->outPort);
  printf("OPTIONS : %s\n", conf->options);
  printf("IN IP : %s\n", conf->inIp);
  printf("IN PORT : %s\n", conf->inPort);
}

int main(int argc, char **argv){
  if (argc != 2) {
		printf("Usage Error \n");
		exit(1);
	};
  Conf *conf = readConfFile(argv[1]);

  printConf(conf);
  int fdTun = tun_alloc(conf->nameTun);
  if(fdTun == -1) {
      fprintf(stderr, "ERREUR ALLOCATION TUNNEL\n");
      return 1;
  }
  char cmd2[256];
  char cmd[256];
  sprintf(cmd, "./configure-tun.sh %s %s ", conf->nameTun, conf->inIp);
  printf("Configuration en cours\n");
  system("chmod +x configure-tun.sh");
  system(cmd);
  sprintf(cmd2, "./configure-route.sh %s %s %s", conf->options, conf->inIp, conf->nameTun);
  printf("Configuration route\n");
  system("chmod +x configure-route.sh");
  system(cmd2);

  asyncInOut(conf->outIp, conf->outPort, conf->inPort, fdTun);
}
