#ifndef utilities_h
#define utilities_h

#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BUFFER_SIZE 512

//CHAR
int trim(char*);
int remove_char(char *, char);
int remove_str(char*, char*);
int find_str(char*, char*, char*, char*);

//FOLDER_FILES
int create_folder(char*);
int create_file(char*, char*);
int create_file_with_content(char*, char*, char*);

//INFOS
#define APP_RESOURCES_INFO "[Info - App Resources] : %s\n"
#define APP_RESOURCES_INFO_MESSAGE "[Info - App Resources] : %s %s\n"

//PRINTS
#define PRINT_FOLDER_CREATED(FOLDER_NAME) printf(APP_RESOURCES_INFO_MESSAGE, FOLDER_NAME, "folder created.");
#define PRINT_FOLDER_EXISTS(FOLDER_NAME) printf(APP_RESOURCES_INFO_MESSAGE, FOLDER_NAME, "folder exists.");
#define PRINT_FILE_CREATED(FILE_NAME) printf(APP_RESOURCES_INFO_MESSAGE, FILE_NAME, "file created.");
#define PRINT_FILE_WRITTEN(FILE_NAME) printf(APP_RESOURCES_INFO_MESSAGE, FILE_NAME, "content written.");
#define PRINT_FILE_EXISTS(FILE_NAME) printf(APP_RESOURCES_INFO_MESSAGE, FILE_NAME, "file exists.");
#define PRINT_ERROR(ERROR_SOURCE) printf(APP_RESOURCES_INFO_MESSAGE, ERROR_SOURCE, "error.");
#define PRINT_MISSING_SETTINGS(SETTINGS_KEY) printf(APP_RESOURCES_INFO_MESSAGE, SETTINGS_KEY, "missing");
#define PRINT_SETTINGS(SETTINGS) printf("%s : %s\n", SETTINGS.key, SETTINGS.value);

//SYSTEM
void open_url(const char *url);

#endif /* utilities_h */