#include "../include/server.h"

static Server new(void) {
    return (Server) { };
}

const struct Server_Const Server_Const = { .new = &new };

static Client_Handler new_client_handler(Server *server, JsonObject *settings) {
    return (Client_Handler) {
        .server = server,
        .settings = settings
    };
}

const struct Client_Handler_Const Client_Handler_Const = { .new = &new_client_handler };

void init_server(struct Server *server) {
    struct sockaddr_in server_addr;

    server->server_sock = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server->server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        PRINT_ERROR("bind()");
        exit(1);
    }
    PRINT_SUCCESS("bind()");

    if (listen(server->server_sock, 1) < 0) {
        PRINT_ERROR("listen()");
        exit(1);
    }
    PRINT_SUCCESS("listen()");
}

void handle_authorization_code(char *data, JsonObject *settings, int client_sock) {
    if (strstr(data , AUTHORIZATION_CODE_PREFIX)) {
        JsonObject* authorization_token = get_json_object_by_key(settings, AUTHORIZATION_CODE);
        if (authorization_token != NULL) {
            data = find_str(data, AUTHORIZATION_CODE_PREFIX, " ");
            remove_str(data, AUTHORIZATION_CODE_PREFIX);
            authorization_token->value = data;
            update_setting(settings);
            char *response = malloc(BUFFER_SIZE * sizeof(char));
            sprintf(response, GET_HTML, AUTHORIZATION_SUCCESS_HTML);
            send(client_sock, response, strlen(response), 0);
        }
        else {
            PRINT_MISSING_SETTINGS(AUTHORIZATION_CODE);
        }
    }
}

void handle_get_requests(char *data, JsonObject *settings, int client_sock) {
    if (strstr(data, "GET")) {
        handle_authorization_code(data, settings, client_sock);
    }
}

void* handle_client(void *args)
{
    Client_Handler* client_handler = (Client_Handler*)args;
    while (1) {
        char *data = malloc(BUFFER_SIZE * sizeof(char));
        client_handler->server->client_sock = accept(client_handler->server->server_sock, NULL, NULL);

        read(client_handler->server->client_sock, data, BUFFER_SIZE * sizeof(char));
        handle_get_requests(data, client_handler->settings, client_handler->server->client_sock);
        shutdown(client_handler->server->client_sock, SHUT_RDWR);
    }
}

void init_jobs(Server *server) {

}