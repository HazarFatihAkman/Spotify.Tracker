#include "../include/server.h"

void handle_authorization_code(char*);
void remove_url(char*, char*);

static struct Server new(void) {
    return (struct Server) { };
}

const struct Server_Const Server_Const = { .new = &new };

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

void handle_client(struct Server *server)
{
    char *data = malloc(BUFFER_SIZE * sizeof(char));
    server->client_sock = accept(server->server_sock, NULL, NULL);

    read(server->client_sock, data, BUFFER_SIZE * sizeof(char));
    handle_get_requests(data);
    shutdown(server->client_sock, SHUT_RDWR);
}

void handle_get_requests(char *data) {
    if (strstr(data, "GET")) {
        handle_authorization_code(data);
    }
}

void handle_authorization_code(char *data) {
    if (strstr(data , AUTHORIZATION_CODE_PREFIX)) {
        data = find_str(data, AUTHORIZATION_CODE_PREFIX, " ");
        remove_str(data, AUTHORIZATION_CODE_PREFIX);
        
        printf("%s\n", data);
    }
}