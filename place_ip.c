// File: place_ip.c
// @author Chan-Sung Kim
// @author ck4135
#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trie.h"

const char delim[] = ",\"\n";

int main ( int argc, char *argv[] ) {
    // check for valid arguments
    if (argc != 2) {
        fprintf(stderr, "usage: place_ip filename\n");
        return EXIT_FAILURE;
    }

    // check for valid file
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        perror(argv[1]);
        return EXIT_FAILURE;
    }
    
    char *buf = NULL;
    size_t buf_size = 0;
    ssize_t chars = getline(&buf, &buf_size, fp);
    
    // check for empty file
    if (chars <= 1) {
        fprintf(stderr, "error: empty dataset\n");
        return EXIT_FAILURE;
    }

    // read and insert data into Trie
    Trie trie = ibt_create();
    while (chars > 0) {
        char *code, *name, *province, *city;
        ikey_t from, to;
        // get from and to IP addresses
        from = atoi(strtok(buf, delim));
        to = atoi(strtok(NULL, delim));
    
        // get country code
        code = strtok(NULL, delim);

        // get country name
        name = strtok(NULL, delim);

        // get province
        province = strtok(NULL, delim);

        // get city
        city = strtok(NULL, delim);

        // insert data into Trie
        Entry *from_range = entry_create(from, code, name, province, city);
        Entry *to_range = entry_create(to, code, name, province, city);

        ibt_insert(trie, from_range);
        ibt_insert(trie, to_range);

        free(buf);
        buf = NULL;

        chars = getline(&buf, &buf_size, fp);
    }
    free(buf);
    
    size_t height = ibt_height(trie);
    size_t size = ibt_size(trie);
    size_t internal = ibt_node_count(trie);
    fprintf(stdout, "\nheight: %zu\nsize: %zu\nnode_count: %zu\n", height, size, internal);
    
    // takes in user input
    char ip[RADIX];
    fprintf(stdout, "\n\nEnter an ipv4 string or a number (or a blank line to quit).\n> ");
    fgets(ip, RADIX, stdin);
    while (!feof(stdin) && ip[0] != '\n') {
        Entry *found;
        ikey_t key = 0;
        int boolean = 0;
        ip[strcspn(ip, "\n")] = 0;
        for (size_t i = 0; i < strlen(ip); i++) {
            if (ip[i] == '.') {
                boolean = 1;
            }
        }

        // handle IP input
        if (boolean == 1) {
            int byte1, byte2, byte3, byte4 = 0;
            sscanf(ip, "%d.%d.%d.%d", &byte1, &byte2, &byte3, &byte4);
            if (byte1 > 255) {
                byte1 = 0;
            }
            if (byte2 > 255) {
                byte2 = 0;
            }
            if (byte3 > 255) {
                byte3 = 0;
            }
            if (byte4 > 255) {
                byte4 = 0;
            }
            key |= byte4;
            key |= (byte3 << 8);
            key |= (byte2 << 16);
            key |= (byte1 << 24);
        }

        // number input
        else {
            key = atoi(ip);
        }
        
        found = ibt_search(trie, key);
        if (found == NULL) {
            fprintf(stdout, "%u: (INVALID, -: -, -, -)\n", key);
        }
        else {
            entry_print(found, stdout);
        }
        fprintf(stdout, "> ");
        fgets(ip, BITSPERWORD, stdin);
    }

    //fprintf(stdout, "\nibt_show output:\n\nkeys:\n");
    //ibt_show(trie, stdout);

    ibt_destroy(trie);
    fclose(fp);
    
    return 0;
}

