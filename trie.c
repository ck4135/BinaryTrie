// 
// File: trie.c 
// Starter trie module for an integer-keyed trie ADT data type
// @author CS@RIT.EDU
// @author TODO add YOUR FULL NAME AND LOGIN HERE
// @author Chan-Sung Kim, ck4135
// // // // // // // // // // // // // // // // // // // // // // // // 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "trie.h"

/// Node_s: binary tree node TODO 
/// Node is a pointer to a dynamically allocated struct Node_s.
/// NodeH is a pointer to a pointer to a struct Node_s.

typedef
struct Node_s {
    //TODO define Node_s structure content here
    struct Node_s *left;
    Entry *value;
    struct Node_s *right;
} * Node;

typedef Node * NodeH;          ///< handle to a trie node

/// Trie_s: TODO define the Trie ADT here

struct Trie_s {
    /// Trie_s: TODO define the Trie ADT here
    Node root;
};

// global constants for bit operations and sizes
// used by the application program

//TODO implementation for constants
const size_t BITSPERBYTE = 8;
const size_t BITSPERWORD = 32;
const size_t BYTESPERWORD = 4;
const size_t RADIX = 256;
const size_t BIT = 1 << 31;

//TODO implementation for everything after this
size_t bit( ikey_t key, int pos ) {
     size_t bit;
     size_t shift = 1 << pos;
     if (( shift & key) == shift) {
         bit = 1;
     }
     else {
         bit = 0;
     }
 
     return bit;
}

//ikey_t convert_ip( ikey_t num ) {

Node node_create( Entry *payload ) {
    Node t = malloc(sizeof(Node));
    t->left = NULL;
    t->right = NULL;
    if (payload == NULL) {
        t->value = NULL;
    }
    else {
        t->value = payload;
    }

    return t;
}

Trie ibt_create( void ) {
    Trie trie;
    trie = malloc( sizeof(Trie) * sizeof(Node) );
    trie->root = malloc(sizeof(Node));
    trie->root->value = NULL;
    trie->root->left = NULL;
    trie->root->right = NULL;
    return trie;
}

void ibt_destroy( Trie trie ) {

}

Node split( Node node1, Node node2, int pos ) {
    Node curr = malloc(sizeof(Node));
    ikey_t key1 = node1->value->key;
    ikey_t key2 = node2->value->key;
    switch (bit(key1, pos) * 2 + bit(key2, pos)) {
        case 0: 
        curr->left = split(node1, node2, pos - 1);
        break;

        case 1:
        curr->left = node1; 
        curr->right = node2;
        break;

        case 2: 
        curr->right = node1;
        curr->left = node2; 
        break;

        case 3:
        curr->right = split(node1, node2, pos - 1);
        break;
    }
    return curr;
}

Node node_insert( Node node, Entry *payload, int pos ) {
    if (node == NULL) {
        return node_create(payload);
    }
    if (node->left == NULL && node->right == NULL) {
        return split(node_create(payload), node, pos);
    }
    if (bit(payload->key, pos) == 0) {
        node->left = node_insert(node->left, payload, pos - 1);
    }
    else {
        node->right = node_insert(node->right, payload, pos - 1);
    }
    return node;
}

void ibt_insert( Trie trie, Entry *payload, int pos ) {
    trie->root = node_insert(trie->root, payload, pos);
}

Entry *node_search( Node node, ikey_t key, int pos ) {
    if (node == NULL) {
        return NULL;
    }
    if (node->left == NULL && node->right == NULL) {
        if (key == node->value->key) {
            return node->value;
        }
        else {
            return NULL;
        }
    }
    if (bit(key, pos) == 0) {
        return node_search(node->left, key, pos - 1);
    }
    else {
        return node_search(node->right, key, pos - 1);
    }
}

Entry *ibt_search( Trie trie, ikey_t key , int pos ) {
    if (trie == NULL) {
        return NULL;
    }
    if (trie->root->left == NULL && trie->root->right == NULL) {
        if (key == trie->root->value->key) {
            return trie->root->value;
        }
        else {
            return NULL;
        }
    }
    if (bit(key, pos) == 0) {
        return node_search(trie->root->left, key, pos - 1);
    }
    else {
        return node_search(trie->root->right, key, pos - 1);
    }
}

size_t ibt_height( Trie trie ) {
    return 0;
}

size_t ibt_size( Trie trie ) {
    return 0;
}

size_t ibt_node_count( Trie trie ) {
    return 0;
}

void ibt_show( Trie trie, FILE * stream ) {



}
