//
//  spotify_app.c
//  Spotify.Tracker
//
//  Created by Hazar Fatih Akman on 1.08.2024.
//

#include "../include/spotify_api.h"

void get_authorization_token(JsonObject *settings, char *redirect_uri) {
    CURL *curl = curl_easy_init();
    CURLcode res;
    char *api_url = malloc(BUFFER_SIZE * sizeof(char));

    JsonObject* client_id = get_json_object_by_key(settings, CLIENT_ID);
    if (client_id->value == NULL) {
        printf("%s\n", CLIENT_ID_NULL);
        exit(1);
    }
    
    sprintf(api_url, AUTHORIZATION_URL, client_id->value, SCOPE, redirect_uri);
    curl_easy_setopt(curl, CURLOPT_URL, api_url);

    res = curl_easy_perform(curl);
    printf("%u\n", res);
    curl_easy_cleanup(curl);
    free(api_url);
    api_url = NULL;
}
