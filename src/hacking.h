// include "hacking.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>


// A function to display an aerror message and then exit
void fatal(char *message){
    int error_message_size = 100;
    int message_max_size, message_size, prelogue_size;
    char error_message[error_message_size];

    strcpy(error_message, "[!] Fatal error ");
    prelogue_size = strlen(error_message);
    message_max_size = error_message_size - prelogue_size;
    message_size = strlen(message);

    if(message_size == 0 | message_size > message_max_size ){
        strncat(error_message, "unknown", 8);
    } else {
        strncat(error_message, message, message_size);
    }
    
    perror(error_message);
    exit(-1);
}


// An error-checked malloc wrapper() function
void *ec_malloc(unsigned int size){
    void *ptr;
    char ec_malloc_error[36];

    strcpy(ec_malloc_error, "in ec_malloc() on memory allocation");
    
    ptr = malloc(size);
    if(ptr==NULL)
        fatal(ec_malloc_error);
    return ptr;
}


// A function to dump raw memory
// in hex byte and printable split format
void dump(const unsigned char *data_buffer, const unsigned int length){
    unsigned char byte;
    unsigned int i, j;
    for(i = 0; i < length; i++){
        byte = data_buffer[i];
        printf("%02x", data_buffer[i]); // Display byte in hex
        if( ((i % 16) == 15) || (i == length-1) ){
            for(j = 0; j < 15-(i % 16); j++){
                printf(" ");
            }
            printf("| ");
            for(j = (i-(i % 16)); j < i; j++){ // Display printable bytes
                byte = data_buffer[j];
                if( (byte > 31) && (byte < 127) ){ // Printable range
                    printf("%c", byte);
                } else {
                    printf(".");
                }
            }
            printf("\n"); // End of dump line (each is 16 bytes long)
        }
    }
}

/* Returns size of file */
static unsigned get_file_size(const char * file_name){
    struct stat sb;

    if (stat(file_name, & sb) != 0){
        fprintf(stderr, "'stat' failed for '%s': %s.\n", file_name, strerror (errno));
        exit(EXIT_FAILURE);
    }
    return sb.st_size;
}


/* Reads entire file into memory */
static unsigned char * read_whole_file (const char * file_name){
    unsigned s;
    unsigned char * contents;
    FILE * f;
    size_t bytes_read;
    int status;

    s = get_file_size(file_name);
    contents = ec_malloc(s + 1);
    if (!contents) {
        fprintf(stderr, "Not enough memory.\n");
        exit(EXIT_FAILURE);
    }
    f = fopen(file_name, "r");
    if (!f) {
        fprintf(stderr, "Could not open '%s': %s.\n", file_name, strerror (errno));
        exit(EXIT_FAILURE);
    }
    bytes_read = fread (contents, sizeof (unsigned char), s, f);
    if (bytes_read != s) {
        fprintf (stderr, "Short read of '%s': expected %d bytes " "but got %d: %s.\n", file_name, s, bytes_read, strerror (errno));
        exit(EXIT_FAILURE);
    }
    status = fclose(f);
    if (status != 0) {
        fprintf(stderr, "Error closing '%s': %s.\n", file_name, strerror (errno));
        exit(EXIT_FAILURE);
    }
    return contents;
} 

// Clears terminal screen in unix systems
#define clear() printf("\033[H\033[J")
