//
//  server.c
//  Spotify.Tracker
//
//  Created by Hazar Fatih Akman on 10.07.2024.
//

#include "../include/server.h"
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 3131

void* check_app_status(void *args) {
    SpotifyApp* spotify_app = (SpotifyApp*)args;
    spotify_app->pid = malloc(sizeof(pid_t));

    while (1) {
        set_pid(spotify_app->pid);

        if (spotify_app->pid == 0) {
            fprintf(stderr, INFO_SPOTIYF_CLOSED);
        }
        else {
            fprintf(stderr, INFO_SPOTIYF_RUNNING);
        }
        sleep(1);
    }
}

void* track_app(void *args) {
    SpotifyApp* spotify_app = (SpotifyApp*)args;

    while (1) {
        if (spotify_app->pid != 0) {
            fprintf(stderr, INFO_TRACKING);
            set_track_info(&spotify_app->current_track);

            if (spotify_app->current_track.paused == false) {
                fprintf(stderr, INFO_CURRENT_TRACK, spotify_app->current_track.name, spotify_app->current_track.artist, spotify_app->current_track.album);
                fprintf(stderr, INFO_CURRENT_TRACK_IMAGE, spotify_app->current_track.cover_src);
            }
        }
        else {
            fprintf(stderr, INFO_CURRENT_TRACK_NOT_FOUND);
        }

        sleep(2);
    }
}

void start_server(struct Server *server) {
    server->server_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if (bind(server->server_socket, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        fprintf(stderr, "bind() failed.\n");
        exit(1);
    }

    if (listen(server->server_socket, 5) < 0) {
        fprintf(stderr, "listen() failed.\n server socket : %d.\n", server->server_socket);
        exit(1);
    }
}

void* send_spotify_info(void *args) {
    Server* server = (Server*) args;
    while(1) {
        server->client_socket = accept(server->server_socket, NULL, NULL);

        char buffer[1024] = {0};
        read(server->client_socket, buffer, 1024);
        printf("Received request:\n%s\n", buffer);

        char *server_message = malloc(DEFAULT_SIZE * sizeof(char));

        char *template =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: application/json\r\n"
            "Access-Control-Allow-Origin: *\r\n"
            "Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\r\n"
            "Access-Control-Allow-Headers: Content-Type\r\n"
            "Connection: close\r\n"  // Ensure the connection is closed properly
            "\r\n"
            "%s";

        SpotifyAppConst.convert_json_data(&server->spotify_app);
        sprintf(server_message, template, server->spotify_app.json_data);

        send(server->client_socket, server_message, strlen(server_message), 0);
        fprintf(stderr, "[Server Info] : Message Send successful. client : %d, server : %d\n-------------------------------------------------\n", server->client_socket, server->server_socket);
        
        shutdown(server->client_socket, SHUT_RDWR);
    }
}

void launch(struct Server server) {
    pthread_t check_app_status_thread, track_app_thread, send_current_track_thread;

    if (pthread_create(&check_app_status_thread, NULL, check_app_status, &server.spotify_app)) {
        fprintf(stderr, ERROR_CREATE_THREAD);
        exit(1);
    }

    if (pthread_create(&track_app_thread, NULL, track_app, &server.spotify_app)) {
        fprintf(stderr, ERROR_CREATE_THREAD);
        exit(1);
    }

    if (pthread_create(&send_current_track_thread, NULL, send_spotify_info, &server)) {
        fprintf(stderr, ERROR_CREATE_THREAD);
        exit(1);
    }

    pthread_join(check_app_status_thread, NULL);
    pthread_join(track_app_thread, NULL);
    pthread_join(send_current_track_thread, NULL);
}

static struct Server new(void) {
    return (struct Server) {
        .spotify_app = SpotifyAppConst.new()
    };
}

const struct ServerConst ServerConst = { .new = &new, .start_server = &start_server, .launch = &launch };
