#include "include/app_resources.h"
#include "include/spotify_api.h"
#include "include/server.h"

void exit_resource_null(JsonObject *settings);

int main(void) {
    printf("hello world");
    create_sources();
    JsonObject* settings = bind_settings();
    exit_resource_null(settings);

    struct Server server = Server_Const.new();
    init_server(&server);
    printf("%d\n",server.server_sock);

    char *redirect_uri = malloc(BUFFER_SIZE * sizeof(char));
    sprintf(redirect_uri, LOCALHOST, PORT);

    get_authorization_token(settings, redirect_uri);
    free(redirect_uri);
    redirect_uri = NULL;
    shutdown(server.server_sock, SHUT_RDWR);
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
