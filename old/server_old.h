//
//  server.h
//  Spotify.Tracker
//
//  Created by Hazar Fatih Akman on 10.07.2024.
//
/*
#ifndef server_h
#define server_h

#include "spotify_application.h"
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>

typedef struct Server {
    int server_socket, client_socket;
    struct SpotifyApp spotify_app;
} Server;

extern const struct ServerConst {
    struct Server (*new)(void);
    void (*init_server)(struct Server*);
    void (*launch)(struct Server);
} ServerConst;

#define INFO_TRACKING "[Info - App] : Tracking Spotify.\n-------------------------------------------------\n"
#define INFO_SPOTIYF_RUNNING "[Info - Spotify] : Spotify's running.\n-------------------------------------------------\n"
#define INFO_SPOTIYF_PID_HAS_BEEN_CHANGED "[Info - Spotify] : Spotify's been changed.\n-------------------------------------------------\n"
#define INFO_SPOTIYF_CLOSED "[Info - Spotify] : Spotify's closed.\n-------------------------------------------------\n"
#define INFO_CURRENT_TRACK "[Info - Current Track] : %s by %s - %s\n-------------------------------------------------\n"
#define INFO_CURRENT_TRACK_IMAGE "[Info - Current Track/Image] : %s\n-------------------------------------------------\n"
#define INFO_CURRENT_TRACK_NOT_FOUND "[Info - Current Track] : Current track not found.\n-------------------------------------------------\n"
#define ERROR_CREATE_THREAD "[Info - App] : Error creating thread\n-------------------------------------------------\n"
#define INFO_APP_TIME "[Info - App Time] : Passed - %d\n-------------------------------------------------\n"
#define SERVER_APP_INFO "[Server Info] : Message Send successful. client : %d, server : %d\n-------------------------------------------------\n"

#endif /* server_h */
*/