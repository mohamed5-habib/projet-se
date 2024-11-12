#include "chat_server.h"

int clients[MAX_CLIENTS]; // Liste des clients connectés
int clientCount = 0;      // Compteur de clients

// Fonction pour gérer chaque client
void *handleClient(void *arg) {
    Client *client = (Client *)arg;
    char buffer[BUFFER_SIZE];
    char message[BUFFER_SIZE + 50];
    
    // Salutation de bienvenue
    snprintf(message, sizeof(message), "Bienvenue sur le serveur, %s!\n", client->pseudo);
    sendMessageToClient(message, client->socket);
    
    // Annoncer l'arrivée d'un nouveau client
    snprintf(message, sizeof(message), "%s s'est connecté.\n", client->pseudo);
    broadcastMessage(message, client->socket);

    // Recevoir et envoyer des messages en continu
    while (1) {
        int bytesReceived = recv(client->socket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived <= 0) {
            break; // Connexion fermée ou erreur
        }
        buffer[bytesReceived] = '\0'; // Terminer la chaîne
        snprintf(message, sizeof(message), "%s: %s", client->pseudo, buffer);
        broadcastMessage(message, client->socket);
    }

    // Annoncer la déconnexion du client
    snprintf(message, sizeof(message), "%s s'est déconnecté.\n", client->pseudo);
    broadcastMessage(message, client->socket);

    // Fermer la connexion et libérer la place dans le tableau des clients
    close(client->socket);
    clientCount--;
    free(client);
    return NULL;
}

// Fonction pour diffuser un message à tous les clients sauf l'expéditeur
void broadcastMessage(const char *message, int senderSocket) {
    for (int i = 0; i < clientCount; i++) {
        if (clients[i] != senderSocket) {
            sendMessageToClient(message, clients[i]);
        }
    }
}

// Fonction pour envoyer un message à un client spécifique
void sendMessageToClient(const char *message, int clientSocket) {
    send(clientSocket, message, strlen(message), 0);
}

// Fonction pour accepter les connexions et démarrer les threads des clients
int main() {
    int serverSocket, newClientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrSize;
    pthread_t threadId;

    // Création du socket serveur
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("Erreur lors de la création du socket");
        exit(1);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Lier le socket à l'adresse et au port
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Erreur lors du bind");
        exit(1);
    }

    // Écouter les connexions entrantes
    if (listen(serverSocket, MAX_CLIENTS) == 0) {
        printf("Serveur en attente de connexions sur le port %d...\n", PORT);
    } else {
        perror("Erreur lors de l'écoute");
        exit(1);
    }

    // Accepter les connexions et créer des threads pour chaque client
    while (1) {
        addrSize = sizeof(clientAddr);
        newClientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrSize);
        if (newClientSocket < 0) {
            perror("Erreur lors de l'acceptation");
            continue;
        }

        if (clientCount < MAX_CLIENTS) {
            // Ajouter le nouveau client à la liste
            clients[clientCount] = newClientSocket;
            clientCount++;

            // Créer un nouveau client avec un pseudonyme
            Client *newClient = (Client *)malloc(sizeof(Client));
            newClient->socket = newClientSocket;
            newClient->address = clientAddr;
            printf("Nouveau client connecté. Adresse IP: %s\n", inet_ntoa(clientAddr.sin_addr));

            // Demander un pseudonyme au client
            char pseudo[50];
            recv(newClientSocket, pseudo, sizeof(pseudo), 0);
            strcpy(newClient->pseudo, pseudo);

            // Démarrer un thread pour gérer ce client
            pthread_create(&threadId, NULL, handleClient, (void *)newClient);
        } else {
            char *msg = "Serveur plein. Connexion refusée.\n";
            send(newClientSocket, msg, strlen(msg), 0);
            close(newClientSocket);
        }
    }

    close(serverSocket);
    return 0;
}