TP_Réseaux (LAB sessions - Ensea TFTP client) ZART Aurélien - LIN Kévin

Nous avions pour objectif dans ce TP, d'implémenter un programme client qui utilise les commandes 'gettftp' et 'puttftp' afin d'envoyer ou recevoir des fichiers
depuis un serveur interne de l'école via le TFTP (trivial file transfert protocol). Le serveur utilisé: 'srvtpinfo1.ensea.fr' disponible sur le port 69.

EX1: On commence par recréer comme dans le TP sur le microshell, une REPL afin de pouvoir récupérer les commandes saisient par l'utilisateur. Avec la fonction strToken,
on 'découpe' la commande en sous-chaînes de caractères afin de pouvoir isoler le host et le filename: '$ gettftp host filename'.

EX2: Ensuite, avec une structure addrinfo appelée hints pour créer une liste de structure addrinfo, c'est avec un pointeur res vers une autre structure addrinfo que l'on obtient l'adresse du serveur.

EX3: Avec ces informations, on peut créer un socket qui nous permettra d'échanger des données avec le serveur.

EX4: Après avoir lancé la connexion du socket avec le serveur, on consulte le RFC 1350 pour comprendre comment sont façonnés les paquets de données. Pour l'instant ce qui nous intéresse ce sont
les paquets de type RRQ (read request) qui vont nous servir à demander un accès aux fichier présents sur le serveur. On les construits de la manière suivante: 

                                 opcode RQQ 01 (2 bytes) + string(filename) + 0 (1 byte) + string(mode) + 0 (1 byte)

L'idée ensuite va être d'envoyer cette requete via send (utilisable car on a bien connect le socket avant) au lieu de sendto (version sans connect) puis d'attendre une réponse du serveur avec 
recv (au lieu de recvfrom sans connect). Et de la même façon que pour RRQ, il faut séparer les données reçues qui sont de la forme DAT:

                                 opcode DAT 03 (2 bytes) + block (2 bytes) + data (n bytes)

Et enfin, il faut confirmer la réception pour chaque pacquet DAT (indiqué par le numéro dans block) qu'on les a bien réceptionnés. On envoie alors un paquet de type ACK (acknowledgment) qui sont constitué
d'un opcode de 04 (sur 2 bytes) puis du numéro du block DAT reçu (sur 2 bytes).

Nous n'avons malheureusement pas eu le temps de terminer cette question.

