#include "../include/server.h"

static struct Server new(void) {
    return (struct Server) { };
}

const struct Server_Const Server_Const = { .new = &new };

void init_server(struct Server *server) {
    server->server_sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    shutdown(server->server_sock, SHUT_RDWR);

    while (bind(server->server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        PRINT_TRY_AGAIN("bind()");
    }
    PRINT_SUCCESS("bind()");

    while (listen(server->server_sock, 1) < 0) {
        PRINT_TRY_AGAIN("listen()");
    }

    PRINT_SUCCESS("listen()");
}
