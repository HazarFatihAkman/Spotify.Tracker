#ifndef app_resources_h
#define app_resources_h

#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define SETTINGS_SIZE 7
#define CHAR_ARRAY_SIZE 8

typedef struct JsonObject {
    char *key, *value;
} JsonObject;

int create_sources(void);
JsonObject* bind_settings(void);
JsonObject* get_json_object_by_key(struct JsonObject*, char*);

//SETTINGS_SOURCE
#define SETTINGS_FOLDER "settings"
#define SETTINGS_JSON_FILE "app_settings.json"
#define SETTINGS_JSON_DATA "{\n\t\"client_id\" : \"\",\n\t\"client_secret\" : \"\",\n\t\"access_token\" : \"\",\n\t\"expire_in\" : \"\",\n\t\"refresh_token\" : \"\",\n\t\"token_type\" : \"\",\n\t\"authorization_code\" : \"\"\n}"

//SETTINGS_SOURCE_JSON
#define CLIENT_ID "client_id"
#define CLIENT_SECRET "client_secret"
#define ACCESS_TOKEN "access_token"
#define REFRESH_TOKEN "refresh_token"
#define TOKEN_TYPE "token_type"
#define EXPIRE_IN "expire_in"
#define AUTHORIZATION_CODE "token_type"

//SETTINGS_SOURCE_ERROR
#define CLIENT_ID_NULL "Client Id's null."
#define CLIENT_SECRET_NULL "Client Secret's null."
#define ACCESS_TOKEN_NULL "Access Token's null."
#define REFRESH_TOKEN_NULL "Refresh Token's null."
#define TOKEN_TYPE_NULL "Token Type's null."

//HTML_SOURCE
#define SPOTIFY_HTML_FOLDER "spotify_html_templates"
#define SPOTIFY_HTML_FILE "index.html"

//INFOS
#define APP_RESOURCES_INFO "[Info - App Resources] : %s\n"
#define APP_RESOURCES_INFO_MESSAGE "[Info - App Resources] : %s %s\n"

//PRINTS
#define PRINT_FOLDER_CREATED(FOLDER_NAME) printf(APP_RESOURCES_INFO_MESSAGE, FOLDER_NAME, "folder created.");
#define PRINT_FOLDER_EXISTS(FOLDER_NAME) printf(APP_RESOURCES_INFO_MESSAGE, FOLDER_NAME, "folder exists.");
#define PRINT_FILE_CREATED(FILE_NAME) printf(APP_RESOURCES_INFO_MESSAGE, FILE_NAME, "file created.");
#define PRINT_FILE_EXISTS(FILE_NAME) printf(APP_RESOURCES_INFO_MESSAGE, FILE_NAME, "file exists.");
#define PRINT_ERROR(ERROR_SOURCE) printf(APP_RESOURCES_INFO_MESSAGE, ERROR_SOURCE, "error.");
#define PRINT_SETTINGS(SETTINGS) printf("%s : %s\n", SETTINGS.key, SETTINGS.value);

#endif /* app_resources_h */
