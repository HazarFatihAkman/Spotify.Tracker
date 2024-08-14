#include "../include/app_resources.h"

const char char_array[CHAR_ARRAY_SIZE] = { '{', '}', '"', '\n', '\\', '\t', '\"', ' ', '\v', '\f', '\r' };

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
        if (create_file_with_content(SETTINGS_FOLDER, SETTINGS_JSON_FILE, SETTINGS_JSON_CONTENT) == -1) {
            PRINT_ERROR(SETTINGS_JSON_FILE);
            exit(EXIT_FAILURE);
        }
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
            if (strstr(old_token, settings_data.key)) {
                remove_char(token, ',');
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

    fclose(settings_file);

    return temp_settings;
}

JsonObject* get_json_object_by_key(JsonObject *settings, char *key) {
    for (int i = 0; i < SETTINGS_SIZE; i++) {
        if (strstr(settings[i].key, key)) {
            return &settings[i];
        }
    }

    return NULL;
}

void to_json(char *temp_str, JsonObject setting, char *format_null, char *format_not_null) {
    if (setting.value == NULL) {
        sprintf(temp_str, format_null, setting.key);
    }
    else {
        sprintf(temp_str, format_not_null, setting.key, setting.value);
    }
}

char* settings_to_json(JsonObject *settings) {
    char *temp_str = malloc(BUFFER_SIZE * sizeof(char));
    char *out = malloc(BUFFER_SIZE * sizeof(char));
    for (int i = 0; i < SETTINGS_SIZE; i++) {
        if (i == 0) {
            to_json(temp_str, settings[i], "{\n\t\"%s\" : \"\",\n\t", "{\n\t\"%s\" : \"%s\",\n\t");
        }
        else if (i == (SETTINGS_SIZE - 1)) {
            to_json(temp_str, settings[i], "\"%s\" : \"\"\n}", "\"%s\" : \"%s\"\n}");
        }
        else {
            to_json(temp_str, settings[i], "\"%s\" : \"\",\n\t", "\"%s\" : \"%s\",\n\t");
        }
        strcat(out, temp_str);
    }
    free(temp_str);
    temp_str = NULL;

    return out;
}

void exit_resource_null(JsonObject *settings) {
    JsonObject* client_id = get_json_object_by_key(settings, CLIENT_ID);
    if (client_id->value == NULL) {
        printf("%s\n", CLIENT_ID_NULL);
        exit(1);
    }
    free(client_id);
    client_id = NULL;

    JsonObject* client_secret = get_json_object_by_key(settings, CLIENT_SECRET);
    if (client_secret->value == NULL) {
        printf("%s\n", CLIENT_SECRET_NULL);
        exit(1);
    }
    free(client_secret);
    client_secret = NULL; 
}