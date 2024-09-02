#include "../include/app_resources.h"

const char char_array[CHAR_ARRAY_SIZE] = { '{', '}', '"', '\n', '\\', '\t', '\"', ' ', '\v', '\f', '\r' };

Json_Object temp_settings[SETTINGS_SIZE] = {
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
    for (int i = 0; i < CHAR_ARRAY_SIZE; i++) {
        remove_char(str, char_array[i]);
    }

    char *temp_str = malloc(BUFFER_SIZE * sizeof(char)),
         *token = malloc(BUFFER_SIZE * sizeof(char)),
         *old_token = malloc(BUFFER_SIZE * sizeof(char));

    strcpy(temp_str, str);

    char *delimineter = strchr(temp_str, ',') ? "," : ":";
    token = strtok(temp_str, delimineter);

    while (token != NULL && token[0] != ' ') {
        if (strcmp(delimineter, ",") == 0) {
            if (strstr(token, key)) {
                return get_settings_value(token, key);
            }
        }
        else if (strcmp(delimineter, ":") == 0) {
            if (strstr(old_token, key)) {
                remove_char(token, ',');
                return token;
            }
        }

        strcpy(old_token, token);
        token = strtok(NULL, delimineter);
    }
    free(token);
    token = NULL;

    free(temp_str);
    temp_str = NULL;

    free(old_token);
    old_token = NULL;
    return NULL;
}

void bind_settings(Json_Object *settings) {
    char *full_path = malloc(BUFFER_SIZE * sizeof(char));
    char *file_str = malloc(BUFFER_SIZE * sizeof(char));

    sprintf(full_path, "%s/%s", SETTINGS_FOLDER, SETTINGS_JSON_FILE);
    FILE *settings_file = fopen(full_path, "r");

    while (fgets(file_str, (BUFFER_SIZE * sizeof(char)), settings_file)) {
        for (int i = 0; i < SETTINGS_SIZE; i++) {
            settings[i].key = temp_settings[i].key;
            if (strstr(file_str, settings[i].key)) {
                settings[i].value = malloc(BUFFER_SIZE * sizeof(char));
                settings[i].value = get_settings_value(file_str, settings[i].key);
            }
        }
    }

    fclose(settings_file);
    free(full_path);
    full_path = NULL;

    free(file_str);
    file_str = NULL;
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
    if (setting.value == NULL) {
        sprintf(temp_str, format_null, setting.key);
    }
    else {
        sprintf(temp_str, format_not_null, setting.key, setting.value);
    }
}

char* settings_to_json(Json_Object settings[SETTINGS_SIZE]) {
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

void exit_resource_null(Json_Object *settings) {
    Json_Object client_id = *get_json_object_by_key(settings, CLIENT_ID);
    if (client_id.value == NULL) {
        printf("%s\n", CLIENT_ID_NULL);
        exit(1);
    }

    Json_Object client_secret = *get_json_object_by_key(settings, CLIENT_SECRET);
    if (client_secret.value == NULL) {
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
    else if (authorization_token->value == NULL) {
        return 1;
    }

    Json_Object *access_token = get_json_object_by_key(settings, ACCESS_TOKEN);
    if (access_token == NULL) {
        PRINT_MISSING_SETTINGS(ACCESS_TOKEN);
        exit(1);
    }
    else if (access_token->value == NULL) {
        return 1;
    }

    Json_Object *refresh_token = get_json_object_by_key(settings, REFRESH_TOKEN);
    if (refresh_token == NULL) {
        PRINT_MISSING_SETTINGS(REFRESH_TOKEN);
        exit(1);
    }
    else if (refresh_token->value == NULL) {
        return 1;
    }

    Json_Object *expire_in = get_json_object_by_key(settings, EXPIRES_IN);
    if (expire_in == NULL) {
        PRINT_MISSING_SETTINGS(REFRESH_TOKEN);
        exit(1);
    }
    else if (expire_in->value == NULL) {
        return 1;
    }

    Json_Object *token_type = get_json_object_by_key(settings, TOKEN_TYPE);
    if (token_type == NULL) {
        PRINT_MISSING_SETTINGS(TOKEN_TYPE);
        exit(1);
    }
    else if (token_type->value == NULL) {
        return 1;
    }

    return 0;
}