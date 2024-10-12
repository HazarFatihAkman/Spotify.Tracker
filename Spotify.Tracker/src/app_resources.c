#include "../include/app_resources.h"

const char char_array[CHAR_ARRAY_SIZE] = { '{', '}', '"', '\n', '\\', '\t', '\"', ' ', '\v', '\f', '\r' };

Json_Object app_settings[SETTINGS_SIZE] = {
    {CLIENT_ID, NULL},
    {CLIENT_SECRET, NULL},
    {ACCESS_TOKEN, NULL},
    {EXPIRES_IN, NULL},
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

char* get_settings_value(char *str, char* key) {
    remove_char(str, '\"');
    remove_char(str, '\n');
    trim(str);

    char *temp_str = malloc(BUFFER_SIZE * sizeof(char)),
         *token = malloc(BUFFER_SIZE * sizeof(char)),
         *old_token = malloc(BUFFER_SIZE * sizeof(char)),
         *response = malloc(BUFFER_SIZE * sizeof(char));

    response[0] = '\0';
    old_token[0] = '\0';
    temp_str = (char*) memcpy(temp_str, str, BUFFER_SIZE * sizeof(char));

    char *delimineter = strchr(temp_str, ',') ? "," : ":";
    token = strtok(temp_str, delimineter);

    while (token != NULL && token[0] != '\0') {
        if (strcmp(delimineter, ",") == 0 && strstr(token, key)) {
            char *temp_response = get_settings_value(token, key);
            memcpy(response, temp_response, strlen(temp_response) + 1);
            free(temp_response);
        }
        else if (strcmp(delimineter, ":") == 0 && old_token != NULL
             && old_token[0] != '\0' && strstr(old_token, key)) {
            remove_char(token, ',');
            memcpy(response, token, strlen(token) + 1);
        }

        if (token != NULL && token[0] != '\0') {
            old_token = (char*) memcpy(old_token, token, strlen(token) + 1);
        }
        token = strtok(NULL, delimineter);
    }

    free(token);
    free(old_token);
    free(temp_str);
    return response;
}

int bind_settings(void) {
    char *full_path = malloc(BUFFER_SIZE * sizeof(char));
    char *file_str = malloc(BUFFER_SIZE * sizeof(char));

    snprintf(full_path, BUFFER_SIZE * sizeof(char), "%s/%s", SETTINGS_FOLDER, SETTINGS_JSON_FILE);
    FILE *settings_file = fopen(full_path, "r");

    while (fgets(file_str, (BUFFER_SIZE * sizeof(char)), settings_file)) {
        for (int i = 0; i < SETTINGS_SIZE; i++) {
            if (strstr(file_str, app_settings[i].key)) {
                if (app_settings[i].value == NULL) {
                    app_settings[i].value = malloc(BUFFER_SIZE * sizeof(char));
                }

                char *temp_value = get_settings_value(file_str, app_settings[i].key);
                trim(temp_value);
                memcpy(app_settings[i].value, temp_value, strlen(temp_value) + 1);
                free(temp_value);
            }
        }
    }

    fclose(settings_file);
    free(full_path);
    free(file_str);
    return 1;
}

Json_Object* get_json_object_by_key(Json_Object settings[SETTINGS_SIZE], char *key) {
    for (int i = 0; i < SETTINGS_SIZE; i++) {
        if (strstr(settings[i].key, key)) {
            return &settings[i];
        }
    }
    return NULL;
}

void to_json(char *temp_str, Json_Object setting, char *format_null, char *format_not_null) {
    if (setting.value == NULL && setting.value[0] == '\0') {
        snprintf(temp_str, BUFFER_SIZE * sizeof(char), format_null, setting.key);
    }
    else {
        snprintf(temp_str, BUFFER_SIZE * sizeof(char), format_not_null, setting.key, setting.value);
    }
}

char* settings_to_json(Json_Object settings[SETTINGS_SIZE]) {
    char *temp_str = malloc(BUFFER_SIZE * sizeof(char));
    char *out = malloc(BUFFER_SIZE * sizeof(char));
    out[0] = '\0';

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
    return out;
}

void exit_resource_null(Json_Object *settings) {
    Json_Object *client_id = get_json_object_by_key(settings, CLIENT_ID);
    if (client_id->value == NULL || client_id->value[0] == '\0') {
        printf("%s\n", CLIENT_ID_NULL);
        exit(1);
    }

    Json_Object *client_secret = get_json_object_by_key(settings, CLIENT_SECRET);
    if (client_secret->value == NULL || client_secret->value[0] == '\0') {
        printf("%s\n", CLIENT_SECRET_NULL);
        exit(1);
    }
}

int is_api_credentials_null(Json_Object *settings) {
    Json_Object *authorization_token = get_json_object_by_key(settings, AUTHORIZATION_CODE);
    if (authorization_token == NULL) {
        PRINT_MISSING_SETTINGS(AUTHORIZATION_CODE);
        exit(1);
    }
    else if (authorization_token->value == NULL || authorization_token->value[0] == '\0') {
        return 1;
    }

    Json_Object *access_token = get_json_object_by_key(settings, ACCESS_TOKEN);
    if (access_token == NULL) {
        PRINT_MISSING_SETTINGS(ACCESS_TOKEN);
        exit(1);
    }
    else if (access_token->value == NULL || access_token->value[0] == '\0') {
        return 1;
    }

    Json_Object *refresh_token = get_json_object_by_key(settings, REFRESH_TOKEN);
    if (refresh_token == NULL) {
        PRINT_MISSING_SETTINGS(REFRESH_TOKEN);
        exit(1);
    }
    else if (refresh_token->value == NULL && refresh_token->value[0] == '\0') {
        return 1;
    }

    Json_Object *expire_in = get_json_object_by_key(settings, EXPIRES_IN);
    if (expire_in == NULL) {
        PRINT_MISSING_SETTINGS(REFRESH_TOKEN);
        exit(1);
    }
    else if (expire_in->value == NULL && expire_in->value[0] == '\0') {
        return 1;
    }

    Json_Object *token_type = get_json_object_by_key(settings, TOKEN_TYPE);
    if (token_type == NULL) {
        PRINT_MISSING_SETTINGS(TOKEN_TYPE);
        exit(1);
    }
    else if (token_type->value == NULL && token_type->value[0] == '\0') {
        return 1;
    }

    return 0;
}