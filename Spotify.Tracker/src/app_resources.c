#include "../include/app_resources.h"

const char char_array[CHAR_ARRAY_SIZE] = { '{', '}', '"', '\n', '\\', '\t', '\"', ' ' };

JsonObject temp_settings[SETTINGS_SIZE] = {
    {CLIENT_ID, NULL},
    {CLIENT_SECRET, NULL},
    {ACCESS_TOKEN, NULL},
    {EXPIRE_IN, NULL},
    {REFRESH_TOKEN, NULL},
    {TOKEN_TYPE, NULL},
    {AUTHORIZATION_CODE, NULL}
};

int create_sources(void) {
    if (create_folder(SETTINGS_FOLDER) == 0) {
        char *json_data = malloc(BUFFER_SIZE * sizeof(char));
        sprintf(json_data, SETTINGS_JSON_CONTENT, "", "", "", "", "", "", "");
        if (create_file_with_content(SETTINGS_FOLDER, SETTINGS_JSON_FILE, SETTINGS_JSON_CONTENT) == -1) {
            PRINT_ERROR(SETTINGS_JSON_FILE);
            exit(EXIT_FAILURE);
        }
        free(json_data);
        json_data = NULL;
    }
    else {
        PRINT_ERROR(SETTINGS_FOLDER);
        exit(EXIT_FAILURE);
    }

    if (create_folder(SPOTIFY_HTML_FOLDER) == 0) {
        if (create_file(SPOTIFY_HTML_FOLDER, SPOTIFY_HTML_FILE) == -1) {
            PRINT_ERROR(SPOTIFY_HTML_FILE);
            exit(EXIT_FAILURE);
        }
    }
    else {
        PRINT_ERROR(SPOTIFY_HTML_FOLDER);
        exit(EXIT_FAILURE);
    }
    return 1;
}


char* get_settings_value(char *str, struct JsonObject settings_data) {
    char *temp_str = malloc(BUFFER_SIZE * sizeof(char)),
         *token = malloc(BUFFER_SIZE * sizeof(char)),
         *old_token = malloc(BUFFER_SIZE * sizeof(char));

    strcpy(temp_str, str);

    char *delimineter = strchr(temp_str, ',') ? "," : ":";
    token = strtok(temp_str, delimineter);

    while (token != NULL && token[0] != ' ') {
        if (strcmp(delimineter, ",") == 0) {
            if (strstr(token, settings_data.key)) {
                return get_settings_value(token, settings_data);
            }
        }
        else if (strcmp(delimineter, ":") == 0) {
            remove_char(token, ',');
            if (strstr(old_token, settings_data.key)) {
                return token;
            }
        }
        old_token = token;
        token = strtok(NULL, delimineter);
    }

    return NULL;
}

JsonObject* bind_settings(void) {
    char *full_path = malloc(BUFFER_SIZE * sizeof(char));
    char *file_str = malloc(BUFFER_SIZE * sizeof(char));

    sprintf(full_path, "%s/%s", SETTINGS_FOLDER, SETTINGS_JSON_FILE);
    FILE *settings_file = fopen(full_path, "r");

    while (fgets(file_str, (BUFFER_SIZE * sizeof(char)), settings_file)) {
        for (int i = 0; i < CHAR_ARRAY_SIZE; i++) {
            remove_char(file_str, char_array[i]);
        }

        for (int i = 0; i < SETTINGS_SIZE; i++) {
            if (strstr(file_str, temp_settings[i].key)) {
                temp_settings[i].value = malloc(BUFFER_SIZE * sizeof(char));
                temp_settings[i].value = get_settings_value(file_str, temp_settings[i]);
            }
        }
    }

    return temp_settings;
}

JsonObject *get_json_object_by_key(JsonObject *settings, char *key) {
    for (int i = 0; i < SETTINGS_SIZE; i++) {
        if (strstr(settings[i].key, key)) {
            return &settings[i];
        }
    }

    return NULL;
}
