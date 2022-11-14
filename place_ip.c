// File: place_ip.c
// @author Chan-Sung Kim
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trie.h"

const char delim[] = ",\"";

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

    printf("%u\n", 1 << 31);

    // construct Trie
    char buf[RADIX];
    memset(buf, 0, RADIX);
    char *token;
    Trie trie = ibt_create();
    Entry *from_range, *to_range;
    while (fgets(buf, RADIX, fp) != NULL) {
        // set from and to IP addresses
        from_range = malloc(sizeof(Entry));
        to_range = malloc(sizeof(Entry));
        
        from_range->key = atoi(strtok(buf, delim));
        to_range->key = atoi(strtok(NULL, delim));
        
        // set country codes
        token = strtok(NULL, delim);
        from_range->code = token;
        to_range->code = token;

        // set country names
        token = strtok(NULL, delim);
        from_range->name = token;
        to_range->name = token;

        // set provinces
        token = strtok(NULL, delim);
        from_range->province = token;
        to_range->province = token;

        // set cities
        token = strtok(NULL, delim);
        from_range->city = token;
        to_range->city = token;

        // insert data into Trie
        //ibt_insert(trie, from_range, 31);
        //ibt_insert(trie, to_range, 31);
    }

    // take in IPV4 number to search for
    char ip[BITSPERWORD];
    printf("Enter an IPV4 string or a number (or a blank line to quit).\n>");
    fgets(ip, BITSPERWORD, stdin);
    while (ip[0] != '\n') {
        printf(": ");
        ibt_search(trie, atoi(ip), 31);
        printf(">");
        fgets(ip, BITSPERWORD, stdin);
    }
    return 0;
}
