#include "include/app_resources.h"
#include "include/spotify_api.h"
#include "include/server.h"

void exit_resource_null(JsonObject *settings);

int main(void) {
    create_sources();
    JsonObject* settings = bind_settings();
    exit_resource_null(settings);

    Server server = Server_Const.new();
    init_server(&server);

    pid_t child_pid = fork();
    if (child_pid < 0) {
        exit(1);
    }
    else if (child_pid == 0) {
        JsonObject* authorization_token = get_json_object_by_key(settings, AUTHORIZATION_CODE);
        if (authorization_token->value == NULL) {
            char *redirect_uri = malloc(BUFFER_SIZE * sizeof(char));
            sprintf(redirect_uri, LOCALHOST_API_PATH, PORT, AUTHORIZATION_CODE_ENDPOINT);
            get_authorization_token(settings, redirect_uri);
        }
    }
    else {
        wait(NULL);
        pthread_t thread_handle_client, thread_jobs;
        Client_Handler client_handler = Client_Handler_Const.new(&server, settings);

        if (pthread_create(&thread_handle_client, NULL, handle_client, &client_handler)) {
            printf("thread error\n");
            exit(1);
        }

        pthread_join(thread_handle_client, NULL);
    }
}

