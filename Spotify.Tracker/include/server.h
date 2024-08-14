#ifndef server_h
#define server_h

#include "spotify_api.h"
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 3131
#define LOCALHOST "http://localhost:%d"
#define LOCALHOST_API_PATH "http://localhost:%d%s"

typedef struct Server {
    int client_sock, server_sock;
} Server;

extern const struct Server_Const {
    struct Server (*new)(void);
} Server_Const;

void init_server(struct Server*);
void init_jobs(struct Server*); //TODO: refresh auth token hourly
void handle_client(struct Server*);

//INFO
#define PRINT_SUCCESS(STR) printf("[Info] : %s is successful.\n", STR);

//ENDPOINTS
/// GET_CURRENT_TRACK
#define GET_CURRENT_TRACK "%s/current-track"
#define GET_CURRENT_TRACK_RESPONSE "{ \"track\":{} }"

/// CALLBACK
#define AUTHORIZATION_CODE_ENDPOINT "/api/authorization-code"
#define AUTHORIZATION_CODE_PREFIX "/api/authorization-code?code="

#endif /* server_h */
