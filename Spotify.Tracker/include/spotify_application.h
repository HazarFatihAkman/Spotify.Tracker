//
//  spotify_application.h
//  Spotify.Tracker
//
//  Created by Hazar Fatih Akman on 25.07.2024.
//

#ifndef spotify_application_h
#define spotify_application_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define JSON_TRACK_INFO_TEMPLATE "{ \"name\": \"%s\", \"artist\" : \"%s\", \"album\" : \"%s\", \"cover_src\" : \"%s\", \"paused\" : %d, \"total_time\" : %ld, \"current_time\" : %d }"
#define JSON_TRACK_INFO_NULL_TEMPLATE "\"NULL\""
#define JSON_DATA_TEMPLATE "{ \"track\" : %s }"
#define DEFAULT_SIZE 1024

typedef struct TrackInfo {
    char name[DEFAULT_SIZE], artist[DEFAULT_SIZE], album[DEFAULT_SIZE], cover_src[DEFAULT_SIZE];
    long total_time;
    int current_time;
    bool paused;
} TrackInfo;

typedef struct SpotifyApp {
    char *json_data;
    TrackInfo current_track;
    pid_t pid;
} SpotifyApp;

extern const struct SpotifyAppConst {
    struct SpotifyApp (*new)(void);
    void (*convert_json_data)(struct SpotifyApp *);
} SpotifyAppConst;

void set_track_info(struct TrackInfo *);
pid_t set_pid(pid_t);
void run_command(char*, char*, FILE*);

#if defined(__APPLE__) || defined(__MACH__)
    //TODO: MacOS
    #include <TargetConditionals.h>
    #define GET_CURRENT_TRACK_SCRIPT "osascript -e 'tell application \"Spotify\"\n set currentTrack to current track\n set trackName to name of currentTrack\n return trackName\n end tell'"
    #define GET_CURRENT_ARTIST_NAME_SCRIPT "osascript -e 'tell application \"Spotify\"\n set currentTrack to current track\n set artistName to artist of currentTrack\n return artistName\n end tell'"
    #define GET_CURRENT_TRACK_ALBUM_NAME_SCRIPT "osascript -e 'tell application \"Spotify\"\n set currentTrack to current track\n set albumName to album of currentTrack\n return albumName\n end tell'"
    #define GET_CURRENT_TRACK_COVER_URL_SCRIPT "osascript -e 'tell application \"Spotify\"\n set currentTrack to current track\n set coverURL to artwork url of currentTrack\n return coverURL\n end tell'"
    #define GET_CURRENT_PLAYER_STATE_SCRIPT "osascript -e 'tell application \"Spotify\"\n set playerState to player state\n return playerState\n end tell'"
    #define GET_DURATION "osascript -e 'tell application \"Spotify\" to return duration of current track'"
    #define GET_CURRENT_POSITION "osascript -e 'tell application \"Spotify\" to return player position'"
    #define GET_APP_PID "pgrep -x Spotify"

#elif defined(_WIN32)
    //TODO: Windows
    #include <windows.h>
    #define GET_CURRENT_TRACK_SCRIPT '\0'
    #define GET_CURRENT_ARTIST_NAME_SCRIPT '\0'
    #define GET_CURRENT_TRACK_ALBUM_NAME_SCRIPT '\0'
    #define GET_CURRENT_TRACK_COVER_URL_SCRIPT '\0'
    #define GET_CURRENT_PLAYER_STATE_SCRIPT '\0'
    #define GET_APP_PID '\0'

#elif defined(_WIN64)
    //TODO: Windows
    #include <windows.h>
    #define GET_CURRENT_TRACK_SCRIPT '\0'
    #define GET_CURRENT_ARTIST_NAME_SCRIPT '\0'
    #define GET_CURRENT_TRACK_ALBUM_NAME_SCRIPT '\0'
    #define GET_CURRENT_TRACK_COVER_URL_SCRIPT '\0'
    #define GET_CURRENT_PLAYER_STATE_SCRIPT '\0'
    #define GET_APP_PID '\0'

#elif defined(unix) || defined(__unix__) || defined(__unix)
    //TODO: Linux
    #define GET_CURRENT_TRACK_SCRIPT '\0'
    #define GET_CURRENT_ARTIST_NAME_SCRIPT '\0'
    #define GET_CURRENT_TRACK_ALBUM_NAME_SCRIPT '\0'
    #define GET_CURRENT_TRACK_COVER_URL_SCRIPT '\0'
    #define GET_CURRENT_PLAYER_STATE_SCRIPT '\0'
    #define GET_APP_PID '\0'

#else
    #error "Unknown"
#endif

#endif /* spotify_application_h */

