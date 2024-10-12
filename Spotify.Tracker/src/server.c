#include "../include/server.h"

const char server_char_array[SERVER_CHAR_ARRAY_SIZE] = { '{', '}', '\"', '\n', '\\', '\t', '\"', '\v', '\f', '\r', ':' };

const char *GET_RESPONSE = "HTTP/1.1 200 OK\r\n"
    "Content-Type: application/json\r\n"
    "Access-Control-Allow-Origin: *\r\n"
    "Access-Control-Allow-Methods: GET\r\n"
    "Access-Control-Allow-Headers: Content-Type\r\n"
    "Connection: close\r\n"  // Ensure the connection is closed properly
    "\r\n"
    "%s";

static Server* new(void) {
    return (Server*)malloc(sizeof(Server));
}

const struct Server_Const Server_Const = { .new = &new };

void init_server(Server *server) {
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

    if (listen(server->server_sock, 5) < 0) {
        PRINT_ERROR("listen()");
        exit(1);
    }
    PRINT_SUCCESS("listen()");
}

int handle_authorization_code(Server *server) {
    Json_Object *authorization_token = get_json_object_by_key(app_settings, AUTHORIZATION_CODE);
    if (authorization_token != NULL) {
        char *content = malloc(BUFFER_SIZE * sizeof(char));
        char *data = malloc(BUFFER_SIZE * sizeof(char));

        server->client_sock = accept(server->server_sock, NULL, NULL);
        read(server->client_sock, data, BUFFER_SIZE * sizeof(char));

        if (data[0] == '\0' || data == NULL) {
            shutdown(server->client_sock, SHUT_RDWR);
            return 0;
        }

        find_str(content, data, AUTHORIZATION_CODE_PREFIX, " ");
        trim(content);
        remove_str(content, AUTHORIZATION_CODE_PREFIX);

        authorization_token->value = content;
        update_setting();
        bind_settings();

        char *response = malloc(BUFFER_SIZE * sizeof(char));
        snprintf(response, BUFFER_SIZE * sizeof(char) + strlen(AUTHORIZATION_SUCCESS_HTML), OK, AUTHORIZATION_SUCCESS_HTML);
        send(server->client_sock, response, strlen(response), 0);
        free(response);
    }
    else {
        PRINT_MISSING_SETTINGS(AUTHORIZATION_CODE);
        exit(1);
    }

    shutdown(server->client_sock, SHUT_RDWR);
    if(authorization_token != NULL && authorization_token->value == NULL) {
        exit(1);
    }

    sleep(3);
    return 1;
}

void handle_get_requests(Thread_Args thread_args) {
    if ((thread_args.data == NULL && thread_args.data[0] == '\0') || !strstr(thread_args.data, GET_CURRENT_TRACK)) {
        return;
    }

    INFO_TRACKING();
    char *response = get_current_playing();

    if (response[0] == '\0') {
        printf("Pls, open and play a song on spotify.\n");
        return;
    }

    char *response_json = malloc(BUFFER_SIZE * sizeof(char)),
         *get_response = malloc(BUFFER_SIZE * sizeof(char)),
         *track_data = malloc(BUFFER_SIZE * sizeof(char)),
         *track_name = malloc(BUFFER_SIZE * sizeof(char)),
         *artist_data = malloc(BUFFER_SIZE * sizeof(char)),
         *artist_name = malloc(BUFFER_SIZE * sizeof(char)),
         *image_data = malloc(BUFFER_SIZE * sizeof(char)),
         *image_src = malloc(BUFFER_SIZE * sizeof(char));

    find_str(track_data, response, "\"is_local\" :", "\"popularity");
    PRINT_SUCCESS("track_data");

    find_str(track_name, track_data, "\"name\" :", ",\n");
    PRINT_SUCCESS("track_name");
    trim(track_name);
        
    find_str(artist_data, response, "\"artists\" : [ {", "],");
    PRINT_SUCCESS("artist_data");

    find_str(artist_name, artist_data, "\"name\" :", ",\n");
    PRINT_SUCCESS("artist_name");
    trim(artist_name);

    find_str(image_data, response, "\"images\" : [ {", "\"width\"");
    PRINT_SUCCESS("image_data");

    find_str(image_src, image_data, "\"url\" :", ",\n");
    PRINT_SUCCESS("image_src");
    trim(image_src);
    
    printf("%s\n", track_name);
    printf("%s\n", artist_name);
    printf("%s\n", image_src);

    snprintf(response_json, BUFFER_SIZE * sizeof(char), RESPONSE_JSON, track_name, artist_name, image_src); 
    snprintf(get_response, BUFFER_SIZE * sizeof(char), GET_RESPONSE, response_json);
    int success = send(thread_args.server->client_sock, get_response, strlen(get_response), 0);
    if (success < 0) {
        free(get_response);
        PRINT_ERROR("send()");
        exit(1);
    }
    else {
        free(get_response);
        PRINT_SUCCESS("send()");
    }

    // free(response);
    // free(response_json);
    // free(track_data);
    // free(artist_data);
    // free(artist_name);
    // free(image_data);
    // free(image_src);
}

void* handle_requests(void *args) {
    Server *server = (Server*)args;
    char *data = malloc(BUFFER_SIZE * sizeof(char));
    while (1) {
        server->client_sock = accept(server->server_sock, NULL, NULL);
        printf("client_sock : %d\n", server->client_sock);
        printf("server_sock : %d\n", server->server_sock);
        if (server->client_sock > 0) {
             read(server->client_sock, data, BUFFER_SIZE * sizeof(char));
             if (data != NULL || strlen(data) > 0) {
                 Thread_Args thread_args = { .data = data, .server = server };
                 if (strstr(thread_args.data, "GET")) {
                     handle_get_requests(thread_args);
                 }   
             }
            shutdown(server->client_sock, SHUT_RDWR);
        }
    }
    free(data);
}

void update_token(char *response, int grant_type) {
    Json_Object *access_token = get_json_object_by_key(app_settings, ACCESS_TOKEN);
    access_token->value = get_settings_value(response, access_token->key);

    Json_Object *expires_in = get_json_object_by_key(app_settings, EXPIRES_IN);
    expires_in->value = get_settings_value(response, expires_in->key);

    Json_Object *refresh_token = get_json_object_by_key(app_settings, REFRESH_TOKEN);
    if (grant_type == 1) {
        char *refresh_token_response = get_settings_value(response, refresh_token->key);
        if (refresh_token_response != NULL && refresh_token_response[0] != '\0' && strlen(refresh_token_response) > 0) {
            refresh_token->value = refresh_token_response;
        }
    }
    else {
        refresh_token->value = get_settings_value(response, refresh_token->key);
    }

    Json_Object *token_type = get_json_object_by_key(app_settings, TOKEN_TYPE);
    token_type->value = get_settings_value(response, token_type->key);

    update_setting();
    bind_settings();
}

void handle_access_token(Server *server) {
    char *redirect_uri = malloc(BUFFER_SIZE * sizeof(char));
    snprintf(redirect_uri, BUFFER_SIZE * sizeof(char), LOCALHOST_API_PATH, PORT, AUTHORIZATION_CODE_ENDPOINT);
    get_authorization_token(redirect_uri);

    int status = 0;
    while (status == 0) {
        status = handle_authorization_code(server);
    }

    if (status == 1) {
        PRINT_SUCCESS("Authorization");
        update_token(get_access_token(redirect_uri), 0);
        free(redirect_uri);
    }

    sleep(3);
}

void* refresh_token_job(void* args) {
    while (1) {
        Json_Object *expires_in = get_json_object_by_key(app_settings, EXPIRES_IN);
        if (expires_in->value != NULL || expires_in->value[0] != '\0') {
            char *token_response = refresh_token();
            update_token(token_response, 1);
            PRINT_SUCCESS("Update Token");
            sleep(atoi(expires_in->value));
        }
        else {
            sleep(10);
        }
    }
}

void init_jobs(Server *server) {
    pthread_t thread_refresh_token;

    if (pthread_create(&thread_refresh_token, NULL, refresh_token_job, &app_settings)) {
        printf("thread error\n");
        exit(1);
    }

    pthread_join(thread_refresh_token, NULL);
}