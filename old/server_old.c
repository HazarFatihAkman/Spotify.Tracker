//
//  server.c
//  Spotify.Tracker
//
//  Created by Hazar Fatih Akman on 10.07.2024.
//
/*
#include "../include/server.h"

#define PORT 3131

const char *template = "HTTP/1.1 200 OK\r\n"
    "Content-Type: application/json\r\n"
    "Access-Control-Allow-Origin: *\r\n"
    "Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\r\n"
    "Access-Control-Allow-Headers: Content-Type\r\n"
    "Connection: close\r\n"  // Ensure the connection is closed properly
    "\r\n"
    "%s";

void* check_app_status(void *args) {
    int passed_seconds = 0;
    SpotifyApp* spotify_app = (SpotifyApp*)args;
    spotify_app->pid = 0;

    while (1) {
        spotify_app->pid = set_pid(spotify_app->pid);

        if (spotify_app->pid == 0) {
            //printf(INFO_SPOTIYF_CLOSED);
        }
        else {
            //printf(INFO_SPOTIYF_RUNNING);
        }

        ++passed_seconds;
        //printf(INFO_APP_TIME, passed_seconds);
        sleep(1);
    }
}

void* send_spotify_info(void *args) {
    Server* server = (Server*) args;
    char *server_message = malloc(DEFAULT_SIZE * sizeof(char));
    char buffer[DEFAULT_SIZE] = {0};

    while(1) {
        server->client_socket = accept(server->server_socket, NULL, NULL);

        if (server->spotify_app.pid != 0) {
            read(server->client_socket, buffer, 1024);
            set_track_info(&server->spotify_app.current_track);

            SpotifyAppConst.convert_json_data(&server->spotify_app);
            sprintf(server_message, template, server->spotify_app.json_data);

            send(server->client_socket, server_message, strlen(server_message), 0);
            printf(SERVER_APP_INFO, server->client_socket, server->server_socket);
            shutdown(server->client_socket, SHUT_RDWR);
        }
        else {
            shutdown(server->client_socket, SHUT_RDWR);
        }
    }
}

void init_server(struct Server *server) {
    server->server_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if (bind(server->server_socket, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        printf("bind() failed.\n");
        exit(1);
    }

    if (listen(server->server_socket, 5) < 0) {
        printf("listen() failed.\n server socket : %d.\n", server->server_socket);
        exit(1);
    }
}

void launch(struct Server server) {
    pthread_t check_app_status_thread, send_current_track_thread;

    if (pthread_create(&check_app_status_thread, NULL, check_app_status, &server.spotify_app)) {
        printf(ERROR_CREATE_THREAD);
        exit(1);
    }

    if (pthread_create(&send_current_track_thread, NULL, send_spotify_info, &server)) {
        printf(ERROR_CREATE_THREAD);
        exit(1);
    }

    pthread_join(check_app_status_thread, NULL);
    pthread_join(send_current_track_thread, NULL);
}

static struct Server new(void) {
    return (struct Server) {
        .spotify_app = SpotifyAppConst.new()
    };
}

const struct ServerConst ServerConst = { .new = &new, .init_server = &init_server, .launch = &launch };
*/
