#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

// Structure représentant un client
typedef struct {
    int socket;
    struct sockaddr_in address;
    char pseudo[50];
} Client;

// Déclaration des fonctions
void *handleClient(void *arg);
void broadcastMessage(const char *message, int senderSocket);
void *receiveMessage(void *arg);
void sendMessageToClient(const char *message, int clientSocket);

#endif