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
    Entry         *value;
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
const size_t SHIFT = 1 << 31;

//TODO implementation for everything after this
static size_t bit( ikey_t key, int pos ) {
     size_t bit = key >> (31 - pos);
     return bit & 1;
}

void entry_print( Entry * payload, FILE * stream ) {
    unsigned char bytes[4];
    ikey_t ip = payload->key;
    bytes[0] = ip & 0xFF;
    bytes[1] = (ip >> 8) & 0xFF;
    bytes[2] = (ip >> 16) & 0xFF;
    bytes[3] = (ip >> 24) & 0xFF;
    fprintf(stream, "%u: (%d.%d.%d.%d,", ip, bytes[3], bytes[2], bytes[1], bytes[0]);
    fprintf(stream, " %s: %s, %s, %s)\n", payload->code, payload->name, 
        payload->city, payload->province);
}

static Node node_create( Entry *payload ) {
    Node t = malloc(sizeof(struct Node_s));
    t->left = NULL;
    t->right = NULL;
    t->value = payload;

    return t;
}

Trie ibt_create( void ) {
    Trie trie;
    trie = malloc(sizeof(struct Trie_s));
    trie->root = NULL;

    return trie;
}

static void entry_destroy( Entry * payload ) {
    // check if entry exists
    if (payload == NULL) {
        return;
    }

    free(payload->code);
    free(payload->name);
    free(payload->province);
    free(payload->city);
    free(payload);
}

static void node_destroy( Node node ) {
    // check if node exists
    if (node == NULL) {
        return;
    }
    node_destroy(node->left);
    node_destroy(node->right);
    
    entry_destroy(node->value);
    free(node);
}

void ibt_destroy( Trie trie ) {
    // check if trie exists
    if (trie == NULL) {
        return;
    }
    node_destroy(trie->root);
    free(trie);
}

static Node split( Node node1, Node node2, int pos ) {
    Node curr = node_create(NULL); 
    ikey_t key1 = node1->value->key;
    ikey_t key2 = node2->value->key;
    switch (bit(key1, pos) * 2 + bit(key2, pos)) {
        case 0: 
        curr->left = split(node1, node2, pos + 1);
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
        curr->right = split(node1, node2, pos + 1);
        break;
    }
    return curr;
}

static Node node_insert( Node node, Entry *payload, int pos ) {
    if (node == NULL) {
        return node_create(payload);
    }
    if (node->value == NULL && node->left == NULL && node->right == NULL) {
        return node_create(payload);
    }
    if (node->left == NULL && node->right == NULL) {
        return split(node_create(payload), node, pos);
    }
    if (bit(payload->key, pos) == 0) {
        node->left = node_insert(node->left, payload, pos + 1);
    }
    else {
        node->right = node_insert(node->right, payload, pos + 1);
    }
    return node;
}

void ibt_insert( Trie trie, Entry *payload ) {
    trie->root = node_insert(trie->root, payload, 0);
}

static Entry *node_search( Node node, ikey_t key, int pos ) {
    if (node == NULL) {
        return NULL;
    }

    // check if leaf node
    if (node->left == NULL && node->right == NULL) {
        return node->value;
    }

    // search for closest match
    if (bit(key, pos) == 0 && node->left == NULL) {
        Node left_search = node->right;
        while (left_search != NULL) {
            if (left_search->left == NULL && left_search->right == NULL) {
                break;
            }
            if (left_search->left == NULL) {
                left_search = left_search->right;
            }
            else {
                left_search = left_search->left;
            }
        }
        return left_search->value;
    }
    if (bit(key, pos) == 1 && node->right == NULL) {
        Node right_search = node->left;
        while (right_search != NULL) {
            if (right_search->left == NULL && right_search->right == NULL) {
                break;
            }
            if (right_search->right == NULL) {
                right_search = right_search->left;
            }
            else {
                right_search = right_search->right;
            }
        }
        return right_search->value;
    }

    // continue recursive search for key
    if (bit(key, pos) == 0) {
        return node_search(node->left, key, pos + 1);
    }
    else {
        return node_search(node->right, key, pos + 1);
    }
}

Entry *ibt_search( Trie trie, ikey_t key ) {
    if (trie == NULL) {
        return NULL;
    }
    if (trie->root->left == NULL && trie->root->right == NULL) {
        return trie->root->value;
    }
    if (bit(key, 0) == 0) {
        return node_search(trie->root->left, key, 1);
    }
    else {
        return node_search(trie->root->right, key, 1);
    }
}

static size_t node_height( Node node ) {
    /*if (node == NULL) {
        return 0;
    }
    size_t left = node_height(node->left);
    size_t right = node_height(node->right);
    if (left > right) {
        return left + 1;
    }
    return right + 1;*/
    if( node == NULL ) {
        return -1;
    } 
    //else if( node->value != NULL && node->left == NULL && node->right == NULL ) {
    //    return 0;
    //} 
    else {
        size_t left = node_height( node->left );
        size_t right = node_height( node->right );
        if ( left >= right ) {
            return left + 1;
        } 
        return right + 1;
    }
}

size_t ibt_height( Trie trie ) {
    if (trie == NULL) {
        return -1;
    }
    else if (trie->root->left == NULL && trie->root->right == NULL) {
        return 0;
    }
    size_t left = node_height(trie->root->left);
    size_t right = node_height(trie->root->right);
    if (left > right) {
        return left + 1;
    }
    return right + 1;
}

static size_t node_size( Node node ) {
    //size_t size = 0;
    if (node == NULL) {
        return 0;
    }
    if (node->left == NULL && node->right == NULL) {
        //size++;
        return 1;
    } //else {
      //  return size + node_size(node->left) + node_size(node->right);
    //}
    return node_size(node->left) + node_size(node->right);
}

size_t ibt_size( Trie trie ) {
    if (trie == NULL) {
        return 0;
    }
    return node_size(trie->root);
}

static size_t count_node( Node node ) {
    if (node == NULL) {
        return 0;
    }
    if (node->left == NULL && node->right == NULL) {
        return 0;
    }
    return 1 + count_node(node->left) + count_node(node->right);
}

size_t ibt_node_count( Trie trie ) {
    if (trie == NULL) {
        return 0;
    }
    return count_node(trie->root);
}

static void node_show( Node node, FILE * stream ) {
    if (node == NULL) {
        return;
    }

    node_show(node->left, stream);
    if (node->value != NULL) {
        entry_print(node->value, stream);
    }
    node_show(node->right, stream);
}

void ibt_show( Trie trie, FILE * stream ) {
    node_show(trie->root, stream);
}

Entry* entry_create( ikey_t key, char *code, char *name, char *province, char *city ) {
    Entry *payload = malloc(sizeof(struct Entry_s));
    
    payload->key = key;
    
    payload->code = malloc(strlen(code) + 1);
    strcpy(payload->code, code);

    payload->name = malloc(strlen(name) + 1);
    strcpy(payload->name, name);

    payload->province = malloc(strlen(province) + 1);
    strcpy(payload->province, province);

    payload->city = malloc(strlen(city) + 1);
    strcpy(payload->city, city);

    return payload;
}

