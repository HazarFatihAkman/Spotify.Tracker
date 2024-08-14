#include "../include/spotify_api.h"

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

int update_setting(JsonObject *settings, char *prefix, char *value) {
    char *settings_path = malloc(BUFFER_SIZE * sizeof(char));
    sprintf(settings_path, "%s/%s", SETTINGS_FOLDER, SETTINGS_JSON_FILE);
    FILE *file = fopen(settings_path, "w");
}
