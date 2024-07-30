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
    ServerConst.start_server(&server);
    ServerConst.launch(server);
}
