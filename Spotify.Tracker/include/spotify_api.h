#ifndef spotify_app_h
#define spotify_app_h

#include "app_resources.h"
#include <stdbool.h>
#include <curl/curl.h>

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
void get_authorization_token(JsonObject*, char*);

#define AUTHORIZATION_BEARER "Bearer %s"
#define AUTHORIZATION_BASIC "Basic %s"
#define ACCESS_TOKEN_GRANT_TYPE "access_token"
#define REFRESH_TOKEN_GRANT_TYPE "refresh_token"

///CONTENTS
#define CONTENT_TYPE "application/x-www-form-urlencoded"
#define SCOPE "user-read-currently-playing"

/// AUTHORIZATION_GET
#define AUTHORIZATION_URL "https://accounts.spotify.com/authorize?response_type=code&client_id=%s&scope=%s&redirect_uri=%s" //TODO: Refresh hourly
#define AUTHORIZATION_RESPONSE "%s?code=%s&state=%d"
#define AUTHORIZATION_ERROR_RESPONSE "%s?error=%s&state=%d"

/// ACCESS_TOKEN_POST
#define ACCESS_TOKEN_URL "https://accounts.spotify.com/api/token"
#define ACCESS_TOKEN_CONTENT "grant_type=%s&client_id=%s&client_secret=%s"

/// REFRESH_TOKEN
#define REFRESH_TOKEN_URL "h"

/// CURRENT_TRACK
#define CURRENT_TRACK_RESPONSE "{ \"items\": { \"track\" : { \"album\": { \"images\" : { \"url\" : %s } }, \"artist\" { \"name\" : %s } }, \"duration_ms\" : %d, \"status\" : %d, \"message\" : %s }"

#endif /* spotify_app_h */
