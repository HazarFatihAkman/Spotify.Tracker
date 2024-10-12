#ifndef server_h
#define server_h

#include "spotify_api.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define PORT 3131
#define LOCALHOST "http://localhost:%d"
#define LOCALHOST_API_PATH "http://localhost:%d%s"
#define SERVER_CHAR_ARRAY_SIZE 11

typedef struct Server_Sock {
    int client_sock, server_sock;
} Server;

extern const struct Server_Const {
    Server* (*new)(void);
} Server_Const;

typedef struct ThreadArgs {
    char *data;
    Server *server;
} Thread_Args;


void init_server(Server*);
void init_jobs(Server*);
void* handle_requests(void*);
int handle_authorization_code(Server*);
void handle_access_token(Server*);
void* refresh_token_job(void*);

//INFO
#define INFO_TRACKING() printf("[Info - App] : Tracking Spotify.\n-------------------------------------------------\n")
#define PRINT_SUCCESS(STR) printf("[Info] : %s is successful.\n", STR);

//ENDPOINTS
/// GET_CURRENT_TRACK
#define GET_CURRENT_TRACK "/api/current-track"

/// CALLBACK
#define AUTHORIZATION_CODE_ENDPOINT "/api/authorization-code"
#define AUTHORIZATION_CODE_PREFIX "/api/authorization-code?code="

//REQUEST_RESPONSE_TYPES
#define OK "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nAccess-Control-Allow-Origin: *\r\nAccess-Control-Allow-Methods: GET\r\nAccess-Control-Allow-Headers: Content-Type\r\nConnection: close\r\n\r\n%s"

//HTML
#define AUTHORIZATION_SUCCESS_HTML "<!DOCTYPE html><html><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>Authorization Confirmed</title><style>body{display:flex;justify-content:center;align-items:center;height:100vh;margin:0;background-color:#f0f8ff;font-family:Arial,sans-serif}.message{padding:20px;border:2px solid #4caf50;border-radius:10px;background-color:#ffffff;box-shadow:0 4px 8px rgba(0,0,0,0.2)}</style><script>setTimeout(()=>window.close(),3000);</script></head><body><div class=\"message\">Confirmed your authorization</div></body></html>"

#endif /* server_h */
