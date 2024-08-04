//
//  app_resources.h
//  Spotify.Tracker
//
//  Created by Hazar Fatih Akman on 1.08.2024.
//

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

typedef struct JsonObject {
    char *key, *value;
} JsonObject;

JsonObject settings[];

//SETTINGS_SOURCE
#define SETTINGS_FOLDER "settings"
#define SETTINGS_JSON_FILE "app_settings.json"
#define SETTINGS_JSON_DATA "{\n\t\"client_id\":\"\",\n\t\"client_secret\" : \"\",\n\t\"client_secret\" : \"\"\n}"

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

int create_sources(void);
JsonObject* read_settings(void);

#endif /* app_resources_h */
