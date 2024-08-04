//
//  spotify_app.h
//  Spotify.Tracker
//
//  Created by Hazar Fatih Akman on 1.08.2024.
//

#ifndef spotify_app_h
#define spotify_app_h

#include "app_resources.h"
#include <stdbool.h>

#define BUFFER_SIZE 1024

typedef struct Spotify_Api_Request {
    JsonObject client_id, client_secret, access_token;
} Spotify_Api_Request;

typedef struct Spotify_Api_Response {
    char track_name[BUFFER_SIZE], track_artist[BUFFER_SIZE], track_cover_src[BUFFER_SIZE], json_data[BUFFER_SIZE];
    int track_current_time;
    long track_total_time;
    bool paused;
} Spotify_Api_Response;

extern const struct Spotify_App_Const {
    struct Spotify_Api_Response (*new)(void);
} Spotify_App_Const;

char *create_json_data(struct Spotify_Api_Response*);

//JSON
#define JSON_DATA_TEMPLATE "{ \"track\" : { \"name\": \"%s\", \"artist\" : \"%s\", \"album\" : \"%s\", \"cover_src\" : \"%s\", \"paused\" : %d, \"total_time\" : %ld, \"current_time\" : %d } }"

//ENDPOINTS
#define ACCESS_TOKEN_URL "https://accounts.spotify.com/api/token"
#define ACCESS_TOKEN_GRANT_TYPE "access_token"
#define REFRESH_TOKEN_GRANT_TYPE "refresh_token"

#endif /* spotify_app_h */
