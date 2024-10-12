#include "../include/spotify_api.h"

struct SpotifyChunk post_request(char* api_url, char* request_headers[], int request_header_size, char* body);
struct SpotifyChunk token_request(int token_type, char *redirect_uri);
size_t write_callback(void* contents, size_t size, size_t nmemb, void *userp);

void get_authorization_token(char *redirect_uri) {
    char *api_url = malloc(BUFFER_SIZE * sizeof(char));
    Json_Object *client_id = get_json_object_by_key(app_settings, CLIENT_ID);
    if (client_id->value == NULL && client_id->value[0] == '\0') {
        printf("%s\n", CLIENT_ID_NULL);
        exit(1);
    }

    snprintf(api_url, BUFFER_SIZE * sizeof(char), AUTHORIZATION_URL, client_id->value, SCOPE, redirect_uri);
    open_url(api_url);
    free(api_url);
}

char* get_access_token(char *redirect_uri) {
    exit_resource_null(app_settings);
    return token_request(0, redirect_uri).mem;
}

char* refresh_token(void) {
    exit_resource_null(app_settings);
    return token_request(1, NULL).mem;
}

int update_setting(void) {
    char *settings_path = malloc(BUFFER_SIZE * sizeof(char));
    snprintf(settings_path, BUFFER_SIZE * sizeof(char), "%s/%s", SETTINGS_FOLDER, SETTINGS_JSON_FILE);
    FILE *file = fopen(settings_path, "w");
    char *json_data = settings_to_json(app_settings);
    fprintf(file, "%s", json_data);
    return fclose(file);
}

char* get_current_playing(void) {
    exit_resource_null(app_settings);
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, CURRENT_PLAYING_URL);

    struct SpotifyChunk chunk;
    chunk.mem = malloc(1);
    chunk.size = 0;

    //JSON_OBJECTS
    Json_Object access_token = *get_json_object_by_key(app_settings, ACCESS_TOKEN);
    Json_Object token_type = *get_json_object_by_key(app_settings, TOKEN_TYPE);

    //HEADERS
    char *custom_authorization = malloc(BUFFER_SIZE * sizeof(char));
    snprintf(custom_authorization, BUFFER_SIZE * sizeof(char), AUTHORIZATION_CUSTOM, token_type.value, access_token.value);

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, custom_authorization);
    headers = curl_slist_append(headers, JSON_CONTENT_TYPE);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

    c_codes = curl_easy_perform(curl);
    if(c_codes != CURLE_OK) {
        printf("curl_easy_perform() failed: %s\n", curl_easy_strerror(c_codes));
        exit(1);
    }
    else if (strstr(chunk.mem, "error")) {
        printf("ERROR : %s\n", chunk.mem);
        exit(1);
    }

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
    return chunk.mem;
}

//DEFINE
struct SpotifyChunk post_request(char* api_url, char* request_headers[], int request_header_size, char* body) {
    curl = curl_easy_init();

    struct SpotifyChunk chunk;
    chunk.mem = malloc(1);
    chunk.size = 0;

    //HEADERS
    struct curl_slist *headers = NULL;
    for (int i = 0; i < request_header_size; i++) {
        headers = curl_slist_append(headers, request_headers[i]);
    }

    curl_easy_setopt(curl, CURLOPT_URL, api_url);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    //FORM_BODY
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

    c_codes = curl_easy_perform(curl);
    if(c_codes != CURLE_OK) {
        printf("curl_easy_perform() failed: %s\n", curl_easy_strerror(c_codes));
        exit(1);
    }
    else if (strstr(chunk.mem, "error")) {
        printf("ERROR : %s\n", chunk.mem);
        exit(1);
    }

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);

    return chunk;
}

size_t write_callback(void* contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct SpotifyChunk* chunk = (struct SpotifyChunk *)userp;

    char *ptr = realloc(chunk->mem, chunk->size + realsize + 1);
    if (!ptr) {
        PRINT_ERROR("write_callback()");
        exit(1);
    }

    chunk->mem = ptr;
    memcpy(&(chunk->mem[chunk->size]), contents, realsize);
    chunk->size += realsize;
    chunk->mem[chunk->size] = 0;

    return realsize;
}

struct SpotifyChunk token_request(int token_type, char *redirect_uri) {
    char *api_body = malloc(BUFFER_SIZE * sizeof(char));
    char *basic = malloc(BUFFER_SIZE * sizeof(char));

    //JSON_OBJECTS
    Json_Object *client_id = get_json_object_by_key(app_settings, CLIENT_ID);
    Json_Object *client_secret = get_json_object_by_key(app_settings, CLIENT_SECRET);
    Json_Object *authorization_code = get_json_object_by_key(app_settings, AUTHORIZATION_CODE);

    //HEADERS
    char *basic_token = malloc(strlen(client_id->value) + strlen(client_secret->value) + 1 * sizeof(char));

    snprintf(basic_token, BUFFER_SIZE * sizeof(char), "%s:%s", client_id->value, client_secret->value);
    snprintf(basic, BUFFER_SIZE * sizeof(char), AUTHORIZATION_BASIC, base64_encode(basic_token));
    char* headers[] = { FORM_CONTENT_TYPE, basic };

    if (token_type == 0 && redirect_uri != NULL && redirect_uri[0] != '\0') {
        snprintf(api_body, BUFFER_SIZE * sizeof(char), ACCESS_TOKEN_CONTENT, authorization_code->value, redirect_uri);
    }
    else {
        Json_Object *refresh_token = get_json_object_by_key(app_settings, REFRESH_TOKEN);
        snprintf(api_body, BUFFER_SIZE * sizeof(char), REFRESH_TOKEN_CONTENT, refresh_token->value, client_id->value);
    }

    struct SpotifyChunk chunk = post_request(TOKEN_URL, headers, 2, api_body);

    free(basic);
    free(basic_token);
    free(api_body);

    return chunk;
}