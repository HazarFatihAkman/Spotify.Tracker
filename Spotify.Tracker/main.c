//
//  main.c
//  Spotify.Tracker
//
//  Created by Hazar Fatih Akman on 6.07.2024.
//

#include "include/spotify_api.h"
#include "include/app_resources.h"
#include <curl/curl.h>

int main(void) {
    create_sources();
    read_settings();
    printf("%s\n", settings[0].value);
    printf("%d\n", settings[0].value != NULL);
}
