Projet Réseaux M1
Barry William Roux Etienne.

Le projet est fonctionnel, il suffit d'appeler "salt-call state.apply" sur
les répertoires /vagrant des différentes VMs et appeler ./tunnel64d confVMx sur les VM1 et VM3 pour que tout fonctionne.

Veuillez noter que nous n'avons pas réussi à installer iperf3 via la configuration salt. Il faut donc exécuter cette ligne de code pour l'obtenir:

$sudo dhclient eth0 && sudo apt install iperf3 && sudo ip route del default

Nous n'avons été pris de court et n'avons malheureusement pas pu complêter la partie 5 intitulée "Améliorations"

Merci.
