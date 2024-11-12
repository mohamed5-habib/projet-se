# Serveur de Chat Multi-Utilisateurs en C

## Description
Ce projet implémente un serveur de chat multi-utilisateurs en utilisant des sockets et des threads en C. Le serveur permet à plusieurs clients de se connecter simultanément et d'échanger des messages en temps réel.

## Fonctionnalités
- Connexion multiple de clients.
- Gestion de messages en temps réel.
- Chaque client peut envoyer des messages à tous les autres clients connectés.
- Affichage des connexions et déconnexions des utilisateurs dans le chat.
- Validation des entrées pour éviter les erreurs courantes.

## Prérequis
- Compiler avec un compilateur C (gcc ou clang).
- Exécuter le serveur sur une machine avec accès au réseau.

## Compilation
Pour compiler le programme, utilisez la commande suivante dans le terminal :

```bash
gcc chat_server.c -o chat_server -lpthread
