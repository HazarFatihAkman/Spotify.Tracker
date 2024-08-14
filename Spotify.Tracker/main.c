#include "include/app_resources.h"
#include "include/spotify_api.h"
#include "include/server.h"

void exit_resource_null(JsonObject *settings);

int main(void) {
    create_sources();
    JsonObject* settings = bind_settings();
    exit_resource_null(settings);

    struct Server server = Server_Const.new();
    init_server(&server);

    char *redirect_uri = malloc(BUFFER_SIZE * sizeof(char));
    sprintf(redirect_uri, LOCALHOST_API_PATH, PORT, AUTHORIZATION_CODE_ENDPOINT);
    get_authorization_token(settings, redirect_uri);

    while (1) {
        handle_client(&server);
    }

    free(redirect_uri);
    redirect_uri = NULL;
}

void exit_resource_null(JsonObject *settings) {
    JsonObject client_id = *get_json_object_by_key(settings, CLIENT_ID);
    if (client_id.value == NULL) {
        printf("%s\n", CLIENT_ID_NULL);
        exit(1);
    }

    JsonObject client_secret = *get_json_object_by_key(settings, CLIENT_SECRET);
    if (client_secret.value == NULL) {
        printf("%s\n", CLIENT_SECRET_NULL);
        exit(1);
    }
}
