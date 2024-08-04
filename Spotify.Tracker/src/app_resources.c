//
//  app_resources.c
//  Spotify.Tracker
//
//  Created by Hazar Fatih Akman on 1.08.2024.
//

#include "../include/app_resources.h"

JsonObject settings[] = {
    {"client_id", NULL},
    {"client_secret", NULL},
    {"access_token", NULL},
    {"refresh_token", NULL},
    {"token_type", NULL}
};

int create_folder(char *folder_path) {
    DIR *dir = opendir(folder_path);

    if (dir) {
        PRINT_FOLDER_EXISTS(folder_path);
        closedir(dir);
        return 0;
    }
    else if (ENOENT == errno) {
        int status = mkdir(folder_path, 0700);
        PRINT_FOLDER_CREATED(folder_path);
        return status;
    }

    return -1;
}

int create_file(char *folder_path, char *file_path) {
    char *full_path = malloc(BUFFER_SIZE * sizeof(char));
    sprintf(full_path, "%s/%s", folder_path, file_path);
    FILE *file = fopen(full_path, "r");

    if (file) {
        PRINT_FILE_EXISTS(full_path);
        free(full_path);
        fclose(file);
        return 1;
    }
    else {
        fclose(file);

        file = fopen(full_path, "w");
        fprintf(file, SETTINGS_JSON_DATA);
        fclose(file);

        PRINT_FILE_CREATED(full_path);
        free(full_path);
        return  1;
    }
}

int create_sources(void) {
    int settings_folder_status = create_folder(SETTINGS_FOLDER);
    if (settings_folder_status == 0) {
        int settings_file_status = create_file(SETTINGS_FOLDER, SETTINGS_JSON_FILE);
        if (settings_file_status == -1) {
            PRINT_ERROR(SETTINGS_JSON_FILE);
            exit(EXIT_FAILURE);
        }
    }
    else {
        PRINT_ERROR(SETTINGS_FOLDER);
        exit(EXIT_FAILURE);
    }

    int spotify_folder_status = create_folder(SPOTIFY_HTML_FOLDER);
    if (spotify_folder_status == 0) {
        int spotify_html_file_status = create_file(SPOTIFY_HTML_FOLDER, SPOTIFY_HTML_FILE);
        if (spotify_html_file_status == -1) {
            PRINT_ERROR(SPOTIFY_HTML_FILE);
            exit(EXIT_FAILURE);
        }
    }
    else {
        PRINT_ERROR(SPOTIFY_HTML_FOLDER);
        exit(EXIT_FAILURE);
    }

    return 0;
}

void remove_char(char *str, char remove_char) {
    int i, j = 0;
    for (i = 0; i <= strlen(str); i++) {
        if (str[i] != remove_char) {
            str[j++] = str[i];
        }
    }
}

void get_settings_value(char *file_str, struct JsonObject *settings_data) {
    char *temp_file_str = malloc(BUFFER_SIZE * sizeof(char));
    char *token = malloc(BUFFER_SIZE * sizeof(char));
    char *new_token = malloc(BUFFER_SIZE * sizeof(char));
    strcpy(temp_file_str, file_str);

    if (strstr(temp_file_str, ",")) {
        token = strtok(temp_file_str, ",");

        while (token != NULL) {
            if (strstr(token, settings_data->key)) {
                new_token = strtok(token, ":");

                while (new_token != NULL) {
                    remove_char(new_token, ',');
                    if (strstr(new_token, settings_data->key) == 0) {
                        settings_data->value = new_token;
                    }
                    new_token = strtok(NULL, ":");
                }
            }
            token = strtok(NULL, ",");
        }
    }
    else {
        token = strtok(temp_file_str, ":");

        while (token != NULL) {
            remove_char(token, ',');
            if (strstr(token, settings_data->key) == 0) {
                settings_data->value = token;
            }

            token = strtok(NULL, ":");
        }
    }

    free(token);
    token = NULL;

    free(new_token);
    new_token = NULL;

    free(temp_file_str);
    temp_file_str = NULL;
}

JsonObject* read_settings(void) {
    char *full_path = malloc(BUFFER_SIZE * sizeof(char));
    char *file_str = malloc(BUFFER_SIZE * sizeof(char));
    FILE *settings_file = fopen(full_path, "r");
    sprintf(full_path, "%s/%s", SETTINGS_FOLDER, SETTINGS_JSON_FILE);

    while (fgets(file_str, (BUFFER_SIZE * sizeof(char)), settings_file)) {
        remove_char(file_str, '{');
        remove_char(file_str, '}');
        remove_char(file_str, ' ');
        remove_char(file_str, '"');
        remove_char(file_str, '\n');
        remove_char(file_str, '\\');
        remove_char(file_str, '\t');

        for (int i = 0; i < sizeof(settings) / sizeof(settings[0]); i++) {
            if (strstr(file_str, settings[i].key)) {
                settings[i].value = malloc(BUFFER_SIZE * sizeof(char));
                get_settings_value(file_str, &settings[0]);
            }
        }
    }

    return settings;
}
