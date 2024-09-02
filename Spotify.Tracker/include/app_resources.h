#ifndef app_resources_h
#define app_resources_h

#include "utilities.h"

#define SETTINGS_SIZE 7
#define CHAR_ARRAY_SIZE 11

typedef struct JsonObject {
    char *key, *value;
} Json_Object;

//FILES
int create_sources(void);

//JsonObjects
void bind_settings(Json_Object*);
Json_Object* get_json_object_by_key(Json_Object[SETTINGS_SIZE], char*);
char* get_settings_value(char*, char*);
char* settings_to_json(Json_Object[SETTINGS_SIZE]);
void exit_resource_null(Json_Object*);
int is_api_credentials_null(Json_Object*);

//SETTINGS_SOURCE
#define SETTINGS_FOLDER "settings"
#define SETTINGS_JSON_FILE "app_settings.json"
#define SETTINGS_JSON_CONTENT "{\n\t\"client_id\" : \"\",\n\t\"client_secret\" : \"\",\n\t\"access_token\" : \"\",\n\t\"expires_in\" : \"\",\n\t\"refresh_token\" : \"\",\n\t\"token_type\" : \"\",\n\t\"authorization_code\" : \"\"\n}"

//HISTORY TODO: MAYBE
#define HISTORY_FOLDER "history"
#define HISTORY_JSON_FILE "%s_history.json"
#define HISTORY_JSON_CONTENT "\"history\" : [\n]"
#define HISTORY_DATA "{\t\"name\" : \"%s\", \"clock\" : \"%s\"\t}"

//SPOTIFY_HTML_SOURCE
#define SPOTIFY_HTML_FOLDER "spotify_html_templates"
#define SPOTIFY_HTML_FILE "index.html"

//SETTINGS_SOURCE_JSON
#define CLIENT_ID "client_id"
#define CLIENT_SECRET "client_secret"
#define ACCESS_TOKEN "access_token"
#define REFRESH_TOKEN "refresh_token"
#define TOKEN_TYPE "token_type"
#define EXPIRES_IN "expires_in"
#define AUTHORIZATION_CODE "authorization_code"

//SETTINGS_SOURCE_ERROR
#define CLIENT_ID_NULL "Client Id's null."
#define CLIENT_SECRET_NULL "Client Secret's null."
#define ACCESS_TOKEN_NULL "Access Token's null."
#define REFRESH_TOKEN_NULL "Refresh Token's null."
#define TOKEN_TYPE_NULL "Token Type's null."

#endif /* app_resources_h */
