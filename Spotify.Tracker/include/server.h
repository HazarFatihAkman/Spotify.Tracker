#ifndef server_h
#define server_h

#include "spotify_api.h"
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 3131
#define LOCALHOST "http://localhost:%d/api"

typedef struct Server {
    int client_sock, server_sock;
} Server;

extern const struct Server_Const {
    struct Server (*new)(void);
} Server_Const;

void init_server(struct Server*);
void init_jobs(struct Server*); //TODO: refresh auth token hourly
void listen_client(struct Server*);

//INFO
#define PRINT_SUCCESS(STR) printf("[Info] : %s is successful.\n", STR);
#define PRINT_TRY_AGAIN(STR) printf("[Info] : %s failed... trying again...\n", STR);

//ENDPOINTS
///GETS
#define GET_CURRENT_TRACK "%s/current-track"

//JSON_RESPONSES
///GETS
#define GET_CURRENT_TRACK_RESPONSE "{ \"track\":{} }"

#endif /* server_h */
