#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ctype.h>

int isImageFile(const char *fileName) {
    const char *imageExtensions[] = {".jpg", ".jpeg", ".png", ".bmp"}; // Add more image extensions if needed
    int numExtensions = sizeof(imageExtensions) / sizeof(imageExtensions[0]);

    char lowercaseFileName[256];
    strcpy(lowercaseFileName, fileName);
    for (int i = 0; i < strlen(lowercaseFileName); i++) {
        lowercaseFileName[i] = tolower(lowercaseFileName[i]);
    }

    for (int i = 0; i < numExtensions; i++) {
        if (strstr(lowercaseFileName, imageExtensions[i]) != NULL) {
            return 1; // File has an image extension
        }
    }

    return 0; // File does not have an image extension
}

void renameFilesInFolder(const char *folderPath, const char *name, const char *extension) {
    struct _finddata_t fileInfo;
    intptr_t handle;
    int counter = 0;
    char searchPath[256];
    char oldName[256];
    char newName[256];

    snprintf(searchPath, sizeof(searchPath), "%s/*", folderPath);
    handle = _findfirst(searchPath, &fileInfo);

    if (handle != -1) {
        do {
            if (!(fileInfo.attrib & _A_SUBDIR) && isImageFile(fileInfo.name)) {
                snprintf(oldName, sizeof(oldName), "%s/%s", folderPath, fileInfo.name);
                snprintf(newName, sizeof(newName), "%s/%s%d.%s", folderPath, name, counter, extension);

                // Rename the file
                if (rename(oldName, newName) == 0) {
                    printf("Renamed %s to %s\n", oldName, newName);
                } else {
                    printf("Failed to rename %s\n", oldName);
                }

                counter++;
            }
        } while (_findnext(handle, &fileInfo) == 0);

        _findclose(handle);
    } else {
        printf("Error opening directory.\n");
    }
}

int main() {
    char folderPath[256];
    char name[256];
    char extension[32];

    printf("Enter the path to the folder: ");
    scanf("%s", folderPath);

    printf("Enter the desired name: ");
    scanf("%s", name);

    printf("Enter the file extension (e.g., jpg, png): ");
    scanf("%s", extension);

    renameFilesInFolder(folderPath, name, extension);

    return 0;
}