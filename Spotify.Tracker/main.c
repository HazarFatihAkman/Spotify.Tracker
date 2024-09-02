#include "include/server.h"

void signature(void);

int main(void) {
    signature();
    Server_Info server_info = Server_Info_Const.new();
    create_sources();
    bind_settings(server_info.settings);
    exit_resource_null(server_info.settings);

    init_server(server_info.server);

    pid_t child_pid = fork();
    if (child_pid < 0) {
        exit(1);
    }
    else if (child_pid == 0) {
        if (is_api_credentials_null(server_info.settings)) {
            handle_access_token(&server_info);
        }
    }
    else {
        wait(NULL);
        pthread_t thread_handle_client;
        bind_settings(server_info.settings);

        if (pthread_create(&thread_handle_client, NULL, handle_requests, &server_info)) {
            printf("thread error\n");
            exit(1);
        }

        init_jobs(&server_info);

        pthread_join(thread_handle_client, NULL);
    }
}

// SIGNATURE
void signature(void) {
    int top_bottom_border = 20;
    int center_space = 18;
    int height = 4;
    const char *author = "Author : Hazar Fatih Akman";
    const char *github = "Github : Meow meow meow - https://github.com/HazarFatihAkman";
    int custom_str_lenght = strlen(author) + strlen(github);

    for (int i = 0; i <= height; i++) {
        if (i == 0) {
            for (int j = 0; j < top_bottom_border + custom_str_lenght; j++) {
                if (j == top_bottom_border + custom_str_lenght - 1) {
                    printf("-\n");
                }
                else {
                    printf("-");
                }
            }
        }
        else if (i == height) {
            for (int j = 0; j < top_bottom_border + custom_str_lenght; j++) {
                if (j == top_bottom_border + custom_str_lenght - 1) {
                    printf("-\n");
                }
                else {
                    printf("-");
                }
            }
        }
        else if (i == 1) {
            printf("|");
            for (int j = 0; j <= center_space + custom_str_lenght; j++) {
                if (j == 0) {
                    printf("");
                }
                else {
                    printf(" ");
                }
            }
            printf("|\n");
        }
        else if (i == 2) {
            printf("|");
            for (int j = 0; j <= center_space + strlen(github); j++) {
                if (j == (center_space + strlen(github)) / 2) {
                    printf("%s", author);
                }
                else {
                    printf(" ");
                }
            }
            printf("|\n");
        }
        else if (i == 3) {
            printf("|");
            for (int j = 0; j <= center_space + strlen(author); j++) {
                if (j == (center_space + strlen(author)) / 2) {
                    printf("%s", github);
                }
                else {
                    printf(" ");
                }
            }
            printf("|\n");
        }
        else {
            printf("|");
            for (int j = 0; j < center_space + custom_str_lenght; j++) {
                    printf(" ");
            }
            printf("|\n");
        }
    }
}
