// File: place_ip.c
// @author Chan-Sung Kim
// @author ck4135
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trie.h"

const char delim[] = ",\"\n";

int main ( int argc, char *argv[] ) {
    // check for valid arguments
    if (argc != 2) {
        fprintf(stderr, "Usage: $ ./place_ip filename\n");
        return -1;
    }

    // check for valid file
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        perror(argv[1]);
        return -1;
    }

    // construct Trie
    char buf[RADIX];
    memset(buf, 0, RADIX);
    Trie trie = ibt_create(NULL);
    while (fgets(buf, RADIX, fp) != NULL) {
        char *code, *name, *province, *city;
        ikey_t from, to;
        Entry *from_range, *to_range;
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
        from_range = entry_create(from, code, name, province, city);
        to_range = entry_create(to, code, name, province, city);

        ibt_insert(trie, from_range);
        ibt_insert(trie, to_range);
    }

    size_t height = ibt_height(trie);
    size_t size = ibt_size(trie);
    size_t internal = ibt_node_count(trie);
    printf("height: %zu\nsize: %zu\nnode_count: %zu\n", height, size, internal);
    
    // takes in user input
    char ip[BITSPERWORD];
    printf("Enter an IPV4 string or a number (or a blank line to quit).\n> ");
    fgets(ip, BITSPERWORD, stdin);
    while (ip[0] != '\n') {
        ip[strcspn(ip, "\n")] = 0;
        printf("%s: ", ip);
        ibt_search(trie, atoi(ip));
        printf("\n> ");
        fgets(ip, BITSPERWORD, stdin);
    }

    printf("\nkeys:\n");
    ibt_show(trie, stdout);

    ibt_destroy(trie);
    fclose(fp);
    return 0;
}
