#include "../include/utilities.h"

int trim(char *str) {
    if (str == NULL || str[0] == '\0') {
        return 0;
    }

    bool is_word = false;
    int j = 0, first_char_index = -1, last_char_index;

    for (int i = 0; i < strlen(str); i++) {
        is_word = str[i] != ' ';
        if (is_word && first_char_index == -1) {
            first_char_index = i;
        }
        else if (is_word) {
            last_char_index = i;
        }
    }
    for (first_char_index; first_char_index <= last_char_index; first_char_index++) {
        str[j++] = str[first_char_index];
    }

    str[j] = '\0';
    return 1;
}

int remove_char(char *str, char remove_char) {
    if (str == NULL || str[0] == '\0') {
        return 0;
    }

    int i, j = 0;
    for (i = 0; i <= strlen(str); i++) {
        if (str[i] != remove_char) {
            str[j++] = str[i];
        }
    }
    return 1;
}

int remove_str(char* str, char *remove_str) {
    if (str == NULL || str[0] == '\0' || remove_str == NULL || remove_str[0] == '\0') {
        return 0;
    }

    int k = 0, x = 0;

    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == remove_str[0]) {
            x = i;
            for (int j = 0; j < strlen(remove_str); j++) {
                if (str[i] != remove_str[j]) {
                    i = x - 1;
                    break;
                }
                else if (j == strlen(remove_str) - 1) {
                    break;
                }
                else {
                    i++;
                }
            }
        }
        else {
            str[k] = str[i];
            k++;
        }
    }

    str[k] = '\0';
    return 1;
}

int find_str(char *value, char *str, char *starts_with, char *ends_with) {
    if (str == NULL || str[0] == '\0' || starts_with == NULL || starts_with[0] == '\0' || ends_with == NULL || ends_with[0] == '\0') {
        return 0;
    }

    char *temp_starts_with = malloc(BUFFER_SIZE * sizeof(char)),
         *temp_ends_with = malloc(BUFFER_SIZE * sizeof(char));

    if (temp_starts_with == NULL || temp_ends_with == NULL) {
        PRINT_ERROR("find_str()");
        exit(1);
    }

    temp_starts_with[0] = '\0';
    temp_ends_with[0] = '\0';

    int starts_with_index = -1,
        ends_with_index = -1,
        starts_with_len = strlen(starts_with) - 1,
        ends_with_len = strlen(ends_with) - 1,
        x = 0,
        j = 0;

    bool is_starts_with = false, is_ends_with = false;

    for (int i = 0; i < strlen(str); i++) {
        is_starts_with = temp_starts_with[0] != '\0' && strstr(temp_starts_with, starts_with) != NULL;
        is_ends_with = temp_ends_with[0] != '\0' && strstr(temp_ends_with, ends_with) != NULL;

        if (!is_starts_with) {
            if (str[i] == starts_with[0]) {
                for (j = 0; j <= starts_with_len; j++) {
                    if (str[i] == starts_with[j]) {
                        temp_starts_with[j] = str[i++];
                    }
                    else {
                        break;
                    }

                    if (j == starts_with_len) {
                        starts_with_index = i;
                    }
                }
            }
        }
        else if (!is_ends_with) {
            if (str[i] == ends_with[0]) {
                x = i;
                for (j = 0; j <= ends_with_len; j++) {
                    if (str[i] == ends_with[j]) {
                        temp_ends_with[j] = str[i++];
                    }
                    if (j == ends_with_len) {
                        ends_with_index = x;
                    }
                }
            }
        }
        else {
            break;
        }
    }

    j = 0;
    for (starts_with_index; starts_with_index < ends_with_index; starts_with_index++) {
        value[j++] = str[starts_with_index];
    }

    value[j] = '\0';

    free(temp_starts_with);
    free(temp_ends_with);

    return 1;
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
    snprintf(full_path, BUFFER_SIZE * sizeof(char), "%s/%s", folder_path, file_path);

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
        snprintf(full_path, BUFFER_SIZE * sizeof(char), "%s/%s", folder_path, file_path);
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
