#include "../include/utilities.h"

void remove_char(char *str, char remove_char) {
    int i, j = 0;
    for (i = 0; i < strlen(str) - 1; i++) {
        if (str[i] != remove_char) {
            str[j++] = str[i];
        }
    }
}

void remove_str(char* str, char *remove_str) {
    char *response_str = malloc(BUFFER_SIZE * sizeof(char));
    int i, j, k = 0, x = 0;

    for (i = 0; i < strlen(str) - 1; i++) {
        if (str[i] == remove_str[0] && str[i + 1] == remove_str[1]) {
            x = i;
            for (j = 0; j < strlen(remove_str) - 1; j++) {
                if (str[i] != remove_str[j]) {
                    i = x;
                    break;
                }
                else {
                    i++;
                }
            }
        }
        else {
            response_str[k] = str[i];
            k++;
        }
    }

    strcpy(str, response_str);
}

char* find_str(char* str, char* find, char* deliminiter) {
    char* token = malloc(BUFFER_SIZE * sizeof(char));
    token = strtok(str, deliminiter);

    while (token != NULL) {
        if (strstr(token, find)) {
            return token;
        }
        token = strtok(NULL, deliminiter);
    }

    return NULL;
}

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
        return 0;
    }
    else {
        fclose(file);
        PRINT_FILE_CREATED(full_path);
        free(full_path);
        return  1;
    }
}

int create_file_with_content(char *folder_path, char *file_path, char *content) {
    if (create_file(folder_path, file_path)) {
        char *full_path = malloc(BUFFER_SIZE * sizeof(char));
        sprintf(full_path, "%s/%s", folder_path, file_path);
        FILE *file = fopen(full_path, "w");

        fprintf(file, "%s", content);
        fclose(file);

        PRINT_FILE_CREATED(full_path);
        free(full_path);
        return  1;
    }
    return 0;
}



void open_url(const char *url) {
    #ifdef _WIN32
        // Windows
        ShellExecute(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
    #elif __APPLE__
        // macOS
        char command[BUFFER_SIZE];
        snprintf(command, sizeof(command), "open \"%s\"", url);
        system(command);
    #elif __linux__
        // Linux
        char command[BUFFER_SIZE];
        snprintf(command, sizeof(command), "xdg-open \"%s\"", url);
        system(command);
    #else
        #error "Unsupported OS"
    #endif
}