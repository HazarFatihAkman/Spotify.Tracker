#ifndef spotify_app_h
#define spotify_app_h

#include "app_resources.h"
#include "base_64.h"
#include <stdbool.h>
#include <curl/curl.h>

CURL *curl;
CURLcode c_codes;

struct SpotifyChunk
{
    char *mem;
    size_t size;
};

// RESPONSE_JSON
#define RESPONSE_JSON "{\n\t\"name\": %s,\n\t\"artist\": %s,\n\t\"image_src\":%s\n}"

char* create_json_data(char*, char*, char*);

void get_authorization_token(Json_Object[SETTINGS_SIZE], char*);
char* get_access_token(Json_Object[SETTINGS_SIZE], char*);
char* refresh_token(Json_Object[SETTINGS_SIZE]);
char* get_current_playing(Json_Object[SETTINGS_SIZE]);

int update_setting(Json_Object[SETTINGS_SIZE]);

///AUTHORIZATIONS
#define AUTHORIZATION_BASIC "Authorization:Basic %s"
#define AUTHORIZATION_CUSTOM "Authorization: %s %s"

///CONTENTS
#define FORM_CONTENT_TYPE "Content-Type: application/x-www-form-urlencoded"
#define JSON_CONTENT_TYPE "Content-Type: application/json"
#define SCOPE "user-read-currently-playing"

/// AUTHORIZATION_GET
#define AUTHORIZATION_URL "https://accounts.spotify.com/authorize?response_type=code&client_id=%s&scope=%s&redirect_uri=%s"

//TOKENS
#define TOKEN_URL "https://accounts.spotify.com/api/token"

/// ACCESS_TOKEN_POST
#define ACCESS_TOKEN_CONTENT "code=%s&redirect_uri=%s&grant_type=authorization_code"

/// REFRESH_TOKEN
#define REFRESH_TOKEN_CONTENT "grant_type=refresh_token&refresh_token=%s&client_id=%s"

/// CURRENT_PLAYING
#define CURRENT_PLAYING_URL "https://api.spotify.com/v1/me/player/currently-playing?additional_types=track"

/// CURRENT_TRACK
#define CURRENT_TRACK_RESPONSE "{ \"items\": { \"track\" : { \"album\": { \"images\" : { \"url\" : %s } }, \"artist\" { \"name\" : %s } }, \"duration_ms\" : %d, \"status\" : %d, \"message\" : %s }"

#endif /* spotify_app_h */
