// A appelle B quand une ligne complète est reçue
void CommandHandler::execute(Client &client, const std::string &line);

// B appelle A pour envoyer un message à un client
void Server::sendTo(Client &client, const std::string &message);


//A
//Tout ce qui touche aux sockets et à la boucle poll(). Le sujet est strict là-dessus : un seul poll(), tout en non-bloquant, pas de errno après recv/send, et une note de 0 si le serveur plante.

//main : vérification des arguments <port> <password>, gestion des signaux (Ctrl+C propre).
//Classe Server : socket, setsockopt, bind, listen, accept, fcntl(F_SETFL, O_NONBLOCK).
//Boucle poll() unique : lecture, écriture et écoute dans le même poll.
//Classe Client (côté réseau) : fd, buffer de réception, file d’envoi.
//Reconstruction des commandes à partir de paquets partiels. C’est le test nc avec Ctrl+D : on accumule dans le buffer jusqu’au \n.
//Envoi différé : si send ne peut pas tout envoyer, on garde le reste et on active POLLOUT.
//Déconnexions propres, sans fuite ni crash.
//Makefile ($(NAME), all, clean, fclean, re, sans relink inutile).


//B
//Tout ce qui se passe une fois qu’une ligne complète a été reçue.

//Parsing d’une ligne : commande + paramètres (+ le : du dernier paramètre).
//Authentification : PASS, NICK, USER, avec l’enregistrement du client et le message de bienvenue (001).
//Classe Channel : membres, opérateurs, topic, modes, liste des invités.
//JOIN, PART, PRIVMSG (vers un utilisateur ou un canal), QUIT, PING/PONG.
//Les commandes opérateur : KICK, INVITE, TOPIC, et MODE avec i, t, k, o, l.
//Les réponses numériques (401, 403, 431, 433, 461, 462, 464, 471, 473, 475, 482…).