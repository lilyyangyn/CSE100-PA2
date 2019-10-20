/**
 * This File shows the implementation details of
 * DictionaryTrie class methods, which are declared
 * in "DictionaryTrie.hpp"
 *
 * Author: Yuening YANG, Shenlang ZHOU
 * Email: y3yang@ucsd.edu
 */
#include "DictionaryTrie.hpp"
#include <iostream>

/* TODO */
DictionaryTrie::DictionaryTrie() { root = 0; }

/* TODO */
bool DictionaryTrie::insert(string word, unsigned int freq) {
    if (word.length() == 0) {
        return false;
    }

    Node* ptr;
    char letter = word[0];
    int i = 1;
    if (root == 0) {
        root = new Node(letter);
        ptr = root;
    } else {
        ptr = root;
        while (true) {
            if (letter < ptr->letter) {
                // search the left subtree
                if (ptr->left != nullptr) {
                    ptr = ptr->left;
                } else {
                    // not found, create new left child
                    ptr->left = new Node(letter);
                    ptr = ptr->left;
                    break;
                }
            } else if (letter > ptr->letter) {
                // search the right subtree
                if (ptr->right != nullptr) {
                    ptr = ptr->right;
                } else {
                    // not found, create new right child
                    ptr->right = new Node(letter);
                    ptr = ptr->right;
                    break;
                }
            } else {
                // search the middle tree
                if (i == word.length()) {
                    if (ptr->is_word) {
                        // the word is already in the trie
                        return false;
                    } else {
                        // the word is not in the trie. mark it as 'word'
                        ptr->is_word = true;
                        ptr->freq = freq;
                        return true;
                    }
                } else {
                    // next letter
                    letter = word[i];
                    i++;
                    if (ptr->mid != nullptr) {
                        ptr = ptr->mid;
                    } else {
                        // not found, create new mid node
                        ptr->mid = new Node(letter);
                        ptr = ptr->mid;
                        break;
                    }
                }
            }
        }
    }

    // loop through the word, create new mid nodes for the rest letters
    while (i < word.length()) {
        letter = word[i];
        i++;
        ptr->mid = new Node(letter);
        ptr = ptr->mid;
    }
    // set the last letter as 'word'. store its frequency
    ptr->is_word = true;
    ptr->freq = freq;
    return true;
}

/* TODO */
bool DictionaryTrie::find(string word) const {
    if (word.length() == 0) {
        return false;
    }
    if (root == 0) {
        return false;
    }

    Node* ptr = root;
    char letter = word[0];
    int i = 1;
    while (true) {
        if (letter < ptr->letter) {
            // search the left subtree
            if (ptr->left != nullptr) {
                ptr = ptr->left;
            } else {
                return false;
            }
        } else if (letter > ptr->letter) {
            // search the right subtree
            if (ptr->right != nullptr) {
                ptr = ptr->right;
            } else {
                return false;
            }
        } else {
            // search the middle subtree
            if (i == word.length()) {
                return ptr->is_word;
            } else {
                if (ptr->mid != nullptr) {
                    letter = word[i];
                    ptr = ptr->mid;
                    i = i + 1;
                } else {
                    return false;
                }
            }
        }
    }
}

/* TODO */
vector<string> DictionaryTrie::predictCompletions(string prefix,
                                                  unsigned int numCompletions) {
    return {};
}

/* TODO */
std::vector<string> DictionaryTrie::predictUnderscores(
    string pattern, unsigned int numCompletions) {
    return {};
}

/* TODO */
DictionaryTrie::~DictionaryTrie() { deleteAll(root); }

void DictionaryTrie::deleteAll(Node* ptr) {
    if (ptr == 0) {
        return;
    }
    deleteAll(ptr->left);
    deleteAll(ptr->mid);
    deleteAll(ptr->right);
    delete ptr;
}