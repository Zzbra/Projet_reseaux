#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>

#define MAXLIGNE 1500

/* Source serveur: code fourni pour le tp3 */
int ext_out(char* port, int fd){
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
  		if (lu > 0 )
  		{
  			//tampon[lu] = '\0';
            /* log */

  			snprintf(msg,MAXLIGNE,"> %s",tampon);
  			write(fd, tampon, lu);
  		}
	   }while ( 1 );
  }
  close(n);
  return 0;
}

/* Source client: code fourni pour le tp3 */
int ext_in(char* ipServ, char* port, int fd) {
	char ip[INET_ADDRSTRLEN];

	struct addrinfo *resol;
	char * tmpdst; /* chaine pour la notation en IPv6 */
	int soc;

	if (getaddrinfo(ipServ, port, NULL, &resol) < 0 ){
		perror("résolution adresse");
		exit(2);
	}

	/* On extrait l'addresse IP */
  	/* il faut faire l'allocation mémoire */
	tmpdst = malloc(INET6_ADDRSTRLEN);
	sprintf(ip,"%d",inet_ntop(AF_INET6,
		((struct sockaddr_in6*)resol->ai_addr)->sin6_addr.s6_addr,tmpdst,INET6_ADDRSTRLEN));

	soc = socket(resol->ai_family, resol->ai_socktype, resol->ai_protocol);

	/* Création de la socket, de type TCP / IP */
	/* On ne considère que la première adresse renvoyée par getaddrinfo */
	if ((soc=socket(resol->ai_family,resol->ai_socktype, resol->ai_protocol))<0) {
		perror("allocation de socket");
		exit(3);
	}
	fprintf(stderr,"le n° de la socket est : %i\n",soc);

	printf("Tentative de connexion à %s /// ip = %s sur le port %s \n", ipServ, ip, port);

  	/* Connexion */
	fprintf(stderr,"Essai de connexion à %s (%s) sur le port %s\n\n",
		ipServ,ip,port);
	if (connect(soc,resol->ai_addr,sizeof(struct sockaddr_in6))<0) {
		perror("connection");
		exit(4);
	}
	freeaddrinfo(resol); /* /!\ Libération mémoire */

	while (1) {
		char buffer[1500];
		int nread;

		/* Note that "buffer" should be at least the MTU size of the interface, eg 1500   bytes */
		nread = read(fd,buffer,sizeof(buffer));

		if(nread < 0) {
			perror("Reading from interface");
			return -1;
		}

		write(soc, buffer, nread);
	}

	close(soc);

	return 0;
}

void asyncInOut(char *ipOut, char* portOut, char *portIn, int fdTun) {
    int f = fork();

    if(f < 0){
	  perror("Fork\n");
	  exit(1);
	}
	else if(f == 0){
	  sleep(5);
    ext_in(ipOut, portOut, fdTun);
  }
	else {
    ext_out(portIn, fdTun);
  }
}
