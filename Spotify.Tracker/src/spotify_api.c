#include "../include/spotify_api.h"

struct SpotifyChunk post_request(char* api_url, char* request_headers[], int request_header_size, char* body);
struct SpotifyChunk token_request(Json_Object settings[SETTINGS_SIZE], int token_type, char *redirect_uri);
size_t write_callback(void* contents, size_t size, size_t nmemb, void *userp);

void get_authorization_token(Json_Object settings[SETTINGS_SIZE], char *redirect_uri) {
    char *api_url = malloc(BUFFER_SIZE * sizeof(char));
    Json_Object *client_id = get_json_object_by_key(settings, CLIENT_ID);
    if (client_id->value == NULL) {
        printf("%s\n", CLIENT_ID_NULL);
        exit(1);
    }

    sprintf(api_url, AUTHORIZATION_URL, client_id->value, SCOPE, redirect_uri);
    open_url(api_url);
    free(api_url);
    api_url = NULL;
}

char* get_access_token(Json_Object settings[SETTINGS_SIZE], char *redirect_uri) {
    exit_resource_null(settings);
    return token_request(settings, 0, redirect_uri).mem;
}

char* refresh_token(Json_Object settings[SETTINGS_SIZE]) {
    exit_resource_null(settings);
    return token_request(settings, 1, NULL).mem;
}

int update_setting(Json_Object settings[SETTINGS_SIZE]) {
    char *settings_path = malloc(BUFFER_SIZE * sizeof(char));
    sprintf(settings_path, "%s/%s", SETTINGS_FOLDER, SETTINGS_JSON_FILE);
    FILE *file = fopen(settings_path, "w");
    char *json_data = settings_to_json(settings);
    fprintf(file, "%s", json_data);
    return fclose(file);
}

char* get_current_playing(Json_Object settings[SETTINGS_SIZE]) {
    exit_resource_null(settings);
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, CURRENT_PLAYING_URL);

    struct SpotifyChunk chunk;
    chunk.mem = malloc(1);
    chunk.size = 0;

    //JSON_OBJECTS
    Json_Object access_token = *get_json_object_by_key(settings, ACCESS_TOKEN);
    Json_Object token_type = *get_json_object_by_key(settings, TOKEN_TYPE);

    //HEADERS
    char *custom_authorization = malloc(BUFFER_SIZE * sizeof(char));
    sprintf(custom_authorization, AUTHORIZATION_CUSTOM, token_type.value, access_token.value);

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

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_URL, api_url);

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

struct SpotifyChunk token_request(Json_Object settings[SETTINGS_SIZE], int token_type, char *redirect_uri) {
    char *api_body = malloc(BUFFER_SIZE * sizeof(char));
    char *basic = malloc(BUFFER_SIZE * sizeof(char));

    //JSON_OBJECTS
    Json_Object *client_id = get_json_object_by_key(settings, CLIENT_ID);
    Json_Object *client_secret = get_json_object_by_key(settings, CLIENT_SECRET);
    Json_Object *authorization_code = get_json_object_by_key(settings, AUTHORIZATION_CODE);

    //HEADERS
    char *basic_token = malloc(strlen(client_id->value) + strlen(client_secret->value) + 1 * sizeof(char));

    sprintf(basic_token, "%s:%s", client_id->value, client_secret->value);
    sprintf(basic, AUTHORIZATION_BASIC, base64_encode(basic_token));
    char* headers[] = { FORM_CONTENT_TYPE, basic };

    if (token_type == 0 && redirect_uri != NULL) {
        sprintf(api_body, ACCESS_TOKEN_CONTENT, authorization_code->value, redirect_uri);
    }
    else {
        Json_Object *refresh_token = get_json_object_by_key(settings, REFRESH_TOKEN);
        sprintf(api_body, REFRESH_TOKEN_CONTENT, refresh_token->value, client_id->value);
    }
    struct SpotifyChunk chunk = post_request(TOKEN_URL, headers, 2, api_body);

    free(basic);
    basic = NULL;

    free(basic_token);
    basic_token = NULL;

    free(api_body);
    api_body = NULL;
    return chunk;
}