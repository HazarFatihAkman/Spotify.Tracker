#include "../include/utilities.h"

void trim(char *str) {
    int k = 0, is_word = 0;
    for (int i = 0; i <= strlen(str); i++) {
        if (is_word == 0) {
            is_word = str[i] != ' ';
        }

        if (is_word == 1) {
            str[k] = str[i];
            k++;
        }
    }
}

void remove_char(char *str, char remove_char) {
    int i, j = 0;
    for (i = 0; i <= strlen(str); i++) {
        if (str[i] != remove_char) {
            str[j++] = str[i];
        }
    }
}

void remove_str(char* str, char *remove_str) {
    char *response_str = malloc(BUFFER_SIZE * sizeof(char));
    int k = 0, x = 0;

    for (int i = 0; i <= strlen(str); i++) {
        if (str[i] == remove_str[0] && str[i + 1] == remove_str[1]) {
            x = i;
            for (int j = 0; j < strlen(remove_str) - 1; j++) {
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

char* find_str(char *str, char *find, char *deliminiter) {
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

char* get_str_between(char *str, char *starts_with, char *ends_with) {
    char *response_str = malloc(BUFFER_SIZE * sizeof(char)),
         *temp_starts_with = malloc(BUFFER_SIZE * sizeof(char)),
         *temp_ends_with = malloc(BUFFER_SIZE * sizeof(char));

    int k = 0,
        x = 0,
        z = 0;

    bool is_starts_with = false,
        is_ends_with = false;

    for (int i = 0; i <= strlen(str); i++) {
        is_starts_with = strstr(temp_starts_with, starts_with);
        is_ends_with = strstr(temp_ends_with, ends_with);

        if (is_starts_with == 0) {
            for (int j = 0; j <= strlen(starts_with); j++) {
                x = i;
                if (str[i] == starts_with[j]) {
                    temp_starts_with[j] = str[i];
                    i++;
                }
                else {
                    i = x;
                    break;
                }
            }
        }
        else if (is_ends_with == 0) {
            if (str[i] == ends_with[0]) {
                z = i;
                for (int j = 0; j < strlen(ends_with); j++) {
                    if (str[i] == ends_with[j]) {
                        temp_ends_with[j] = str[i];
                        i++;
                        if (j == strlen(ends_with) - 1) {
                            is_ends_with = strstr(temp_ends_with, ends_with);
                            i = x - 1;
                            break;
                        }
                    }
                    else {
                        z += 2;
                        break;
                    }
                }
            }
        }
        else if (is_starts_with == 1 && is_ends_with == 1 && i < z) {
            response_str[k] = str[i];
            k++;
        }
        else if (i == z) {
            break;
        }
    }

    free(temp_starts_with);
    temp_starts_with = NULL;

    free(temp_ends_with);
    temp_ends_with = NULL;

    return response_str;
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

    if (access(full_path, F_OK) != -1) {
        PRINT_FILE_EXISTS(full_path);
        free(full_path);
        full_path = NULL;
        return 1;
    }
    else {
        FILE *file = fopen(full_path, "w");
        if (file == NULL) {
            return -1;
        }

        PRINT_FILE_CREATED(full_path);
        fclose(file);
        free(full_path);
        full_path = NULL;
        return 0;
    }

    return -1;
}

int create_file_with_content(char *folder_path, char *file_path, char *content) {
    if (create_file(folder_path, file_path) == 0) {
        char *full_path = malloc(BUFFER_SIZE * sizeof(char));
        sprintf(full_path, "%s/%s", folder_path, file_path);
        FILE *file = fopen(full_path, "w");
        if (file == NULL) {
            return -1;
        }

        fprintf(file, "%s", content);
        fclose(file);

        PRINT_FILE_WRITTEN(full_path);
        free(full_path);
        full_path = NULL;
        return  0;
    }
    else {
        return 0;
    }
    return -1;
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
