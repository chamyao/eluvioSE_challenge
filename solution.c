#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Result {
    int length;
    int index1;
    int index2;
} result;


struct Result compare(char* file1, char* file2, long len1, long len2) {
    int* prefix = malloc(len1 * len2 * sizeof(int));
    int longest_match = 0;
    int index1;
    int index2;

    for (int j = 0; j < len2; j++) {
        if (file1[0] == file2[j]) {
            prefix[j] = 1;
            longest_match = 1;
            index1 = 0;
            index2 = j;
        } else {
            prefix[j] = 0;
        }
    }

    for (int i = 0; i < len1; i++) {
        if (file1[i] == file2[0]) {
            prefix[i*len2] = 1;
            longest_match = 1;
            index1 = i*len2;
            index2 = 0;
        } else {
            prefix[i*len2] = 0;
        }
    }

    for (int i = 1; i < len1; i++) {
        for (int j = 1; j < len2; j++) {
            int last = prefix[(i - 1) * len2 + j - 1];
            if (file1[i] == file2[j]) {
                prefix[i * len2 + j] = last + 1;
                if (last + 1 > longest_match) {
                    longest_match = last + 1;
                    index1 = i - last;
                    index2 = j - last;
                }
            } else {
                prefix[0] = last;
            }
        }
    }
    struct Result result;
    result.length = longest_match;
    result.index1 = index1;
    result.index2 = index2;

    free(prefix);
    return result;
}

struct File_read {
    char* file;
    long len;
    char* file_name;
} file_read;

struct File_read read_file(char* filename) {
    long len;
    char* load;
    FILE* file = fopen(filename, "rb");
    fseek(file, 0, SEEK_END);
    len = ftell(file);
    rewind(file);

    load = (char *) malloc(len * sizeof(char));
    fread(load, len, 1, file);
    fclose(file);

    struct File_read file_read;
    file_read.file = load;

    file_read.len = len;
    file_read.file_name = filename;
    return file_read;
}

int main() {
    struct File_read sample1 = read_file("sample.1");
    struct File_read sample2 = read_file("sample.2");
    struct File_read sample3 = read_file("sample.3");
    struct File_read sample4 = read_file("sample.4");
    struct File_read sample5 = read_file("sample.5");
    struct File_read sample6 = read_file("sample.6");
    struct File_read sample7 = read_file("sample.7");
    struct File_read sample8 = read_file("sample.8");
    struct File_read sample9 = read_file("sample.9");
    struct File_read sample10 = read_file("sample.10");

    struct File_read samples[10];
    samples[0] = sample1;
    samples[1] = sample2;
    samples[2] = sample3;
    samples[3] = sample4;
    samples[4] = sample5;
    samples[5] = sample6;
    samples[6] = sample7;
    samples[7] = sample8;
    samples[8] = sample9;
    samples[9] = sample10;

    int longest_match = 0;
    int num_matches = 0;
    char* matches[10];
    int indices[10];

    for (int i = 0; i < 10; i++) {
        for (int j = i + 1; j < 10; j++) {
            result = compare(samples[i].file, samples[j].file, samples[i].len, samples[j].len);
            if (result.length > longest_match) {
                longest_match = result.length;
                num_matches = 2;
                matches[0] = samples[i].file_name;
                matches[1] = samples[j].file_name;
                indices[0] = result.index1;
                indices[1] = result.index2;
            } else if (result.length == longest_match) { /* check if more than 2 files contain match */
                int end = num_matches;
                for (int k = 0; k < end; k++) {
                    if (strcmp(samples[i].file_name, matches[k]) == 0) {
                        matches[num_matches] = samples[j].file_name;
                        indices[num_matches] = result.index2;
                        num_matches = num_matches + 1;
                    } else if (strcmp(samples[j].file_name, matches[k]) == 0) {
                        matches[num_matches] = samples[i].file_name;
                        indices[num_matches] = result.index1;
                        num_matches = num_matches + 1;
                    }
                }
            } 
        }
        free(samples[i].file);
    }

    printf("length (in bytes): %d\n", longest_match);
    printf("matches:\n");
    for (int i = 0; i < num_matches; i++) {
        printf("filename: %s index: %d \n", matches[i], indices[i]);
    }
    return 0;
}