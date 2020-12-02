#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>

#define MAXLIGNE 80
/* Source serveur: code fourni pour le tp3 */
int ex_out(char* port, int fd){
  int s,n; /* descripteurs de socket */
  int len, on;
  struct addrinfo * resol;
  struct addrinfo indic = {AI_PASSIVE, /* Toute interface */
                           PF_INET6,SOCK_STREAM,0, /* IP mode connecté */
                           0,NULL,NULL,NULL};
  struct sockaddr_in client; /* adresse de socket du client */

  int err = getaddrinfo(NULL,port,&indic,&resol);
  fprintf(stderr,"Ecoute sur le port %s\n",port);
  if (err<0){
    fprintf(stderr,"Résolution: %s\n",gai_strerror(err));
    exit(2);
  }

  /* Création de la socket, de type TCP / IP */
  if ((s=socket(resol->ai_family,resol->ai_socktype,resol->ai_protocol))<0) {
    perror("allocation de socket");
    exit(3);
  }

  /* On rend le port réutilisable rapidement /!\ */
  on = 1;
  if (setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on))<0) {
    perror("option socket");
    exit(4);
  }
  fprintf(stderr,"Option(s) OK!\n");

  /* Association de la socket s à l'adresse obtenue par résolution */
  if (bind(s,resol->ai_addr,sizeof(struct sockaddr_in6))<0) {
    perror("bind");
    exit(5);
  }
  freeaddrinfo(resol); /* /!\ Libération mémoire */
  fprintf(stderr,"bind!\n");

  /* la socket est prête à recevoir */
  if (listen(s,SOMAXCONN)<0) {
    perror("listen");
    exit(6);
  }
  fprintf(stderr,"listen!\n");


  while(1){
    /* attendre et gérer indéfiniment les connexions entrantes */
    len=sizeof(struct sockaddr_in);
    if( (n=accept(s,(struct sockaddr *)&client,(socklen_t*)&len)) < 0 ) {
      perror("accept");
      exit(7);
    }
    /* Nom réseau du client */
    char hotec[NI_MAXHOST];  char portc[NI_MAXSERV];
    err = getnameinfo((struct sockaddr*)&client,len,hotec,NI_MAXHOST,portc,NI_MAXSERV,0);
    if (err < 0 ){
      fprintf(stderr,"résolution client (%i): %s\n",n,gai_strerror(err));
    }else{
      fprintf(stderr,"accept! (%i) ip=%s port=%s\n",n,hotec,portc);
    }
    /* traitement */

    ssize_t lu; /* nb d'octets reçus */
    char msg[MAXLIGNE+1]; /* tampons pour les communications */
    char tampon[MAXLIGNE+1];
    do { /* Faire echo et logguer */
      lu = recv(n,tampon,MAXLIGNE,0);
      printf("%d\n", lu);
      if (lu > 0 )
      {
          tampon[lu] = '\0';
          /* log */
          fprintf(stderr,"[%s:%s]:%s",hotec, portc, tampon);
          snprintf(msg,MAXLIGNE,"> %s",tampon);
          printf("message: %s\n", msg);
      }
    }while ( 1 );
  }
}

int main (int argc, char** argv){
	int n = ex_out("123", 1);
}
