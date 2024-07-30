//
//  spotify_application.c
//  Spotify.Tracker
//
//  Created by Hazar Fatih Akman on 25.07.2024.
//

#include "../include/spotify_application.h"

static void convert_json_data(struct SpotifyApp *spotify_app);

void run_command(char *command, char args[CHAR_SIZE]) {
    FILE *file_pipe;
    size_t n = 0;
    int c = '\0';

    if (args[0] != '\0') {
        memset(args, '\0', CHAR_SIZE * sizeof(char));
    }

    if ((file_pipe = (FILE*)popen(command, "r")) == 0) {
        fprintf(stderr, "popen failed.");
        exit(1);
    }

    while ((c = fgetc(file_pipe)) != EOF) {
        if (c == '\n' || c == '\"') {
            c = '\0';
        }

        args[n++] = (char)c;
    }

    args[n] = '\0';

    pclose(file_pipe);
    fprintf(stderr, "[PCLOSE] : closed %s : %p.\n", args, args);
}

static struct TrackInfo new_track_info(void) {
    return (struct TrackInfo) {
        .paused = true
    };
}

static struct SpotifyApp new(void) {
    return (struct SpotifyApp) {
        .current_track = new_track_info(),
    };
}

const struct SpotifyAppConst SpotifyAppConst = { .new = &new, .convert_json_data = &convert_json_data };

void set_track_info(struct TrackInfo *track_info) {
    char temp_args[CHAR_SIZE];
    run_command(GET_CURRENT_PLAYER_STATE_SCRIPT, temp_args);
    track_info->paused = strcmp(temp_args, "paused") == 0;

    if (track_info->paused == false || track_info->name[0] == '\0') {
        run_command(GET_CURRENT_TRACK_SCRIPT, track_info->name);
        run_command(GET_CURRENT_ARTIST_NAME_SCRIPT, track_info->artist);
        run_command(GET_CURRENT_TRACK_ALBUM_NAME_SCRIPT, track_info->album);
        run_command(GET_CURRENT_TRACK_COVER_URL_SCRIPT, track_info->cover_src);
        run_command(GET_DURATION, temp_args);
        track_info->total_time = atoi(temp_args);
        run_command(GET_CURRENT_POSITION, temp_args);
        track_info->current_time = atoi(temp_args);
    }
}

static void convert_json_data(struct SpotifyApp *spotify_app) {
    char *json_data = malloc(DEFAULT_SIZE * sizeof(char));
    char *track = malloc(DEFAULT_SIZE * sizeof(char));

    if (spotify_app->current_track.name[0] != '\0') {
        sprintf(track,
                JSON_TRACK_INFO_TEMPLATE,
                spotify_app->current_track.name,
                spotify_app->current_track.artist,
                spotify_app->current_track.album,
                spotify_app->current_track.cover_src,
                spotify_app->current_track.paused,
                spotify_app->current_track.total_time,
                spotify_app->current_track.current_time);
    }
    else {
        strcat(track, JSON_TRACK_INFO_NULL_TEMPLATE);
    }

    sprintf(json_data,
            JSON_DATA_TEMPLATE,
            track);
    
    spotify_app->json_data = json_data;
    free(track);
    free(json_data);
}

void set_pid(pid_t pid) {
    char temp_args[CHAR_SIZE];
    run_command(GET_APP_PID, temp_args);
    if (pid != atoi(temp_args))
    {
        pid = atoi(temp_args);
    }
}
