//
//  main.c
//  Spotify.Tracker
//
//  Created by Hazar Fatih Akman on 6.07.2024.
//

#include "include/server.h"

//TODO: send json data

int main(void) {
    Server server = ServerConst.new();
    char temp_args[CHAR_SIZE];
    run_command(GET_CURRENT_TRACK_ALBUM_NAME_SCRIPT, temp_args);
    fprintf(stderr, "%s\n", temp_args);
    ServerConst.start_server(&server);
    ServerConst.launch(server);
}
