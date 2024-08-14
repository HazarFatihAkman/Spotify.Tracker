#include "../include/spotify_api.h"

CURL *curl;
CURLcode curl_code;

void get_authorization_token(JsonObject *settings, char *redirect_uri) {
    char *api_url = malloc(BUFFER_SIZE * sizeof(char));

    JsonObject* client_id = get_json_object_by_key(settings, CLIENT_ID);
    if (client_id->value == NULL) {
        printf("%s\n", CLIENT_ID_NULL);
        exit(1);
    }

    sprintf(api_url, AUTHORIZATION_URL, client_id->value, SCOPE, redirect_uri);
    open_url(api_url);
    free(api_url);
    api_url = NULL;
}

int update_setting(JsonObject *settings) {
    char *settings_path = malloc(BUFFER_SIZE * sizeof(char));
    sprintf(settings_path, "%s/%s", SETTINGS_FOLDER, SETTINGS_JSON_FILE);
    FILE *file = fopen(settings_path, "w");
    char *json_data = settings_to_json(settings);
    fprintf(file, "%s", json_data);
    return fclose(file);
}

void get_access_token(JsonObject *settings) {
    exit_resource_null(settings);
    char *api_body = malloc(BUFFER_SIZE * sizeof(char));

    JsonObject* client_id = get_json_object_by_key(settings, CLIENT_ID);
    JsonObject* client_secret = get_json_object_by_key(settings, CLIENT_SECRET);
    sprintf(api_body, ACCESS_TOKEN_CONTENT, ACCESS_TOKEN_GRANT_TYPE, client_id->value, client_secret->value);

    curl_easy_setopt(curl, CURLOPT_URL, ACCESS_TOKEN_URL);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, api_body);
    //TODO: READ RESPONSE

    curl_code = curl_easy_perform(curl);

    curl_easy_cleanup(curl);
}