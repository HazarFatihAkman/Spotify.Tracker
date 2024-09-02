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

static Server_Info new_client_handler(void) {
    return (Server_Info) {
        .server = Server_Const.new(),
        .settings = {}
    };
}

const struct Server_Info_Const Server_Info_Const = { .new = &new_client_handler };

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

int handle_authorization_code(Server_Info *server_info) {
    Json_Object *authorization_token = get_json_object_by_key(server_info->settings, AUTHORIZATION_CODE);
    if (authorization_token != NULL) {
        char *data = malloc(BUFFER_SIZE * sizeof(char));
        server_info->server->client_sock = accept(server_info->server->server_sock, NULL, NULL);
        read(server_info->server->client_sock, data, BUFFER_SIZE * sizeof(char));

        data = get_str_between(data, AUTHORIZATION_CODE_PREFIX, " ");
        authorization_token->value = data;
        update_setting(server_info->settings);

        char *response = malloc(BUFFER_SIZE * sizeof(char));
        sprintf(response, GET_HTML, AUTHORIZATION_SUCCESS_HTML);
        send(server_info->server->client_sock, response, strlen(response), 0);
    }
    else {
        PRINT_MISSING_SETTINGS(AUTHORIZATION_CODE);
        exit(1);
    }

    shutdown(server_info->server->client_sock, SHUT_RDWR);
    if(authorization_token != NULL && authorization_token->value == NULL) {
        exit(1);    
    }

    sleep(3);
    return 1;
}

void handle_get_requests(Thread_Args thread_args) {
    if (strstr(thread_args.data, GET_CURRENT_TRACK)) {
        INFO_TRACKING();
        char *response = malloc(BUFFER_SIZE * sizeof(char));
        response = get_current_playing(thread_args.server_info->settings);
        if (response[0] == '\0') {
            printf("Pls, open and play a song on spotify.\n");
            return;
        }

        char *response_json = malloc(BUFFER_SIZE * sizeof(char)),
             *get_response = malloc(BUFFER_SIZE * sizeof(char));

        char *track_data = get_str_between(response, "\"is_local\" :", "\"popularity");
        PRINT_SUCCESS("track_data");
        //printf("track_data :%s\n", track_data);

        char *track_name = get_str_between(track_data, "\"name\" :", ",\n");
        PRINT_SUCCESS("track_name");
        trim(track_name);
        // printf("track_name : %s\n", track_name);

        char *artist_data = get_str_between(response, "\"artists\" : [ {", "],");
        PRINT_SUCCESS("artist_data");
        // printf("artist_data :%s\n", artist_data);

        char *artist_name = get_str_between(artist_data, "\"name\"", ",\n");
        PRINT_SUCCESS("artist_name");
        remove_char(artist_name, ':');
        trim(artist_name);
        // printf("artist_name : %s\n", artist_name);

        char *image_data = get_str_between(response, "\"images\" : [ {", "\"width\"");
        PRINT_SUCCESS("image_data");
        // printf("image_data :%s\n", image_data);

        char *image_src = get_str_between(image_data, "\"url\" :", ",\n");
        PRINT_SUCCESS("image_src");
        trim(image_src);
        //printf("image_src : %s\n", image_src);

        sprintf(response_json, RESPONSE_JSON, track_name, artist_name, image_src);
        printf("%s\n", response_json);
        
        sprintf(get_response, GET_RESPONSE, response_json);

        free(response);
        response = NULL;

        free(response_json);
        response_json = NULL;

        free(track_data);
        track_data = NULL;

        free(artist_data);
        artist_data = NULL;

        free(artist_name);
        artist_name = NULL;

        free(image_data);
        image_data = NULL;

        free(image_src);
        image_src = NULL;

        if (send(thread_args.server_info->server->client_sock, get_response, strlen(get_response), 0) < 0) {
            free(get_response);
            get_response = NULL;

            PRINT_ERROR("send()");
            exit(1);
        }
        else {
            free(get_response);
            get_response = NULL;

            PRINT_SUCCESS("send()");
        }
    }
}

void* handle_requests(void *args) {
    Server_Info* server_info = (Server_Info*)args;
    while (1) {
        char *data = malloc(BUFFER_SIZE * sizeof(char));
        server_info->server->client_sock = accept(server_info->server->server_sock, NULL, NULL);
        read(server_info->server->client_sock, data, BUFFER_SIZE * sizeof(char));

        Thread_Args thread_args = { .data = data, .server_info = server_info };
        if (strstr(thread_args.data, "GET")) {
            handle_get_requests(thread_args);
        }

        free(data);
        data = NULL;
        shutdown(server_info->server->client_sock, SHUT_RDWR);
    }
}

void update_token(Json_Object *settings, char *response, int grant_type) {
    Json_Object *access_token = get_json_object_by_key(settings, ACCESS_TOKEN);
    access_token->value = get_settings_value(response, access_token->key);

    Json_Object *expires_in = get_json_object_by_key(settings, EXPIRES_IN);
    expires_in->value = get_settings_value(response, expires_in->key);

    Json_Object *refresh_token = get_json_object_by_key(settings, REFRESH_TOKEN);
    if (grant_type == 1) {
        char *refresh_token_response = get_settings_value(response, refresh_token->key);
        if (refresh_token_response != NULL) {
            refresh_token->value = refresh_token_response;
        }
    }
    else {
        refresh_token->value = get_settings_value(response, refresh_token->key);
    }

    Json_Object *token_type = get_json_object_by_key(settings, TOKEN_TYPE);
    token_type-> value = get_settings_value(response, token_type->key);

    update_setting(settings);
    bind_settings(settings);
}

void handle_access_token(Server_Info *server_info) {
    char *redirect_uri = malloc(BUFFER_SIZE * sizeof(char));
    sprintf(redirect_uri, LOCALHOST_API_PATH, PORT, AUTHORIZATION_CODE_ENDPOINT);
    get_authorization_token(server_info->settings, redirect_uri);

    int status = 0;
    while (status == 0) {
        status = handle_authorization_code(server_info);
    }

    if (status == 1) {
        PRINT_SUCCESS("Authorization");
    }

    update_token(server_info->settings, get_access_token(server_info->settings, redirect_uri), 0);

    free(redirect_uri);
    redirect_uri = NULL;

    sleep(5);
}

void* refresh_token_job(void* args) {
    Json_Object *settings = (Json_Object *)args;
    while (1) {
        Json_Object *expires_in = get_json_object_by_key(settings, EXPIRES_IN);
        if (expires_in->value != NULL) {
            update_token(settings, refresh_token(settings), 1);
            PRINT_SUCCESS("Update Token");
            sleep(atoi(expires_in->value));
        }
        else {
            sleep(10);
        }
    }
}

void init_jobs(Server_Info *server_info) {
    pthread_t thread_refresh_token;

    if (pthread_create(&thread_refresh_token, NULL, refresh_token_job, &server_info->settings)) {
        printf("thread error\n");
        exit(1);
    }

    pthread_join(thread_refresh_token, NULL);
}