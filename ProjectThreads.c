%%writefile ProjectThreads.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/resource.h>

#define MAX_THREADS 4
#define MAX_WORDS 1000
#define CHUNK_SIZE 1024

typedef struct {
    char **target_words;
    int target_word_count;
    char *chunk;
    int *word_counts;
} ThreadArgs;

typedef struct {
    double processing_time;
    double cpu_usage;
    double memory_usage;
    int *word_counts;
} FileStats;

// Function to split a string into words and count target words
void *process_chunk(void *args) {
    ThreadArgs *thread_args = (ThreadArgs *)args;
    char *token = strtok(thread_args->chunk, " \n\t\r");
    while (token != NULL) {
        for (int i = 0; i < thread_args->target_word_count; i++) {
            if (strcasecmp(token, thread_args->target_words[i]) == 0) {
                thread_args->word_counts[i]++;
            }
        }
        token = strtok(NULL, " \n\t\r");
    }
    return NULL;
}

// Function to read the entire content of a file
char *read_file(const char *filepath, long *file_size) {
    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    *file_size = ftell(file);
    rewind(file);

    char *content = (char *)malloc((*file_size + 1) * sizeof(char));
    fread(content, 1, *file_size, file);
    content[*file_size] = '\0';

    fclose(file);
    return content;
}

// Function to process a single file using multiple threads
FileStats process_single_file(const char *filename, char **target_words, int target_word_count) {
    struct timeval start_time, end_time;
    struct rusage usage_start, usage_end;

    // Start time and resource usage
    gettimeofday(&start_time, NULL);
    getrusage(RUSAGE_SELF, &usage_start);

    long file_size;
    char *content = read_file(filename, &file_size);
    if (content == NULL) {
        perror("Error reading file");
        exit(1);
    }

    int *word_counts = (int *)calloc(target_word_count, sizeof(int));

    // Split content into chunks for threading
    pthread_t threads[MAX_THREADS];
    ThreadArgs thread_args[MAX_THREADS];
    int chunk_size = file_size / MAX_THREADS;

    for (int i = 0; i < MAX_THREADS; i++) {
        thread_args[i].target_words = target_words;
        thread_args[i].target_word_count = target_word_count;
        thread_args[i].chunk = content + i * chunk_size;
        thread_args[i].word_counts = word_counts;
        pthread_create(&threads[i], NULL, process_chunk, (void *)&thread_args[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // End time and resource usage
    gettimeofday(&end_time, NULL);
    getrusage(RUSAGE_SELF, &usage_end);

    // Calculate metrics
    double processing_time = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec) / 1e6;
    double cpu_usage = (usage_end.ru_utime.tv_sec - usage_start.ru_utime.tv_sec) +
                       (usage_end.ru_utime.tv_usec - usage_start.ru_utime.tv_usec) / 1e6;
    double memory_usage = usage_end.ru_maxrss / 1024.0;

    free(content);

    // Fill stats and return
    FileStats stats;
    stats.processing_time = processing_time;
    stats.cpu_usage = cpu_usage;
    stats.memory_usage = memory_usage;
    stats.word_counts = word_counts;

    return stats;
}

// Function to print the results
void print_results(const char *filename, FileStats stats, char **target_words, int target_word_count) {
    printf("\nFile: %s\n", filename);
    printf("Processing Time: %.2f seconds\n", stats.processing_time);
    printf("CPU Usage: %.2f%%\n", stats.cpu_usage * 100);
    printf("Memory Usage: %.2f MB\n", stats.memory_usage);
    printf("Word Counts:\n");
    for (int i = 0; i < target_word_count; i++) {
        printf("  - '%s': %d\n", target_words[i], stats.word_counts[i]);
    }
    free(stats.word_counts);  // Free the word counts array
}

// Main function
int main() {
    char *directory_path = "calgary_corpus";
    char *target_words[] = {"the", "a", "in", "is", "at", "this", "could", "would", "can't"};
    int target_word_count = 9;
    char *files_to_process[] = {"bib", "paper1", "paper2", "progc", "progl", "progp", "trans"};
    int num_files = 7;

    printf("\nStarting file processing...\n");

    for (int i = 0; i < num_files; i++) {
        char filepath[256];
        snprintf(filepath, sizeof(filepath), "%s/%s", directory_path, files_to_process[i]);
        printf("\nProcessing file: %s\n", filepath);

        FileStats stats = process_single_file(filepath, target_words, target_word_count);
        print_results(filepath, stats, target_words, target_word_count);
    }

    return 0;
}

//// Set this code below in a separate block in google collab
%%shell
gcc ProjectThreads.c -o ProjectThreads
./ProjectThreads
