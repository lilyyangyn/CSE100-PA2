/**
 * This File shows the implementation details of
 * DictionaryTrie class methods, which are declared
 * in "DictionaryTrie.hpp"
 *
 * Author: Yuening YANG, Shenlang ZHOU
 * Email: y3yang@ucsd.edu, shzhou@ucsd.edu
 */
#include "DictionaryTrie.hpp"
#include <iostream>

/* It is the constructor*/
DictionaryTrie::DictionaryTrie() { root = 0; }

/* This is the function to insert the word into the trie.
    arguments: word to insert, frequency of that word
    return: true if insertion is successful, false otherwise
 */
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
                    ptr->left->parent = ptr;
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
                    ptr->right->parent = ptr;
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
                        ptr->mid->parent = ptr;
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
        ptr->mid->parent = ptr;
        ptr = ptr->mid;
    }
    // set the last letter as 'word'. store its frequency
    ptr->is_word = true;
    ptr->freq = freq;
    return true;
}

/* This is the function to find whether the word is in the trie.
    arguments: the target word
    return true if the word is found, false otherwise
 */
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

/* Use frequency to complete the predict completions.
    arguments: prefix, number of completions return.
    return: a list of completions, sorted by their frequency
 */
vector<string> DictionaryTrie::predictCompletions(string prefix,
                                                  unsigned int numCompletions) {
    vector<string> results;
    if (root == 0) {
        // empty tree, no completions
        return results;
    }

    Node* ptr = root;
    // if prefix is not empty string, then search whether completion exists
    if (prefix.length() != 0) {
        char letter = prefix[0];
        int i = 1;

        // search whether completion exists in the trie
        //      if exists, ptr pointing to the last letter of prefix
        //      if not exists, return an empty vector
        while (true) {
            if (letter < ptr->letter) {
                // into left subtree
                if (ptr->left != nullptr) {
                    ptr = ptr->left;
                } else {
                    // no completion exists
                    return results;
                }
            } else if (letter > ptr->letter) {
                // into right subtree
                if (ptr->right != nullptr) {
                    ptr = ptr->right;
                } else {
                    // no completion exists
                    return results;
                }
            } else {
                // into middle subtree
                if (i == prefix.length()) {
                    break;
                } else {
                    if (ptr->mid != nullptr) {
                        letter = prefix[i];
                        i++;
                        ptr = ptr->mid;
                    } else {
                        // no completion exists
                        return results;
                    }
                }
            }
        }
    }

    vector<Node*> vtr;
    if (prefix.length() == 0) {
        inorderTraversal(root, vtr);
    } else {
        if (ptr->is_word) {
            vtr.push_back(ptr);
        }
        inorderTraversal(ptr->mid, vtr);
    }
    sort(vtr.begin(), vtr.end(), CompFreq());

    for (int i = 0; i < numCompletions; i++) {
        if (i < vtr.size()) {
            results.push_back(vtr[i]->getWord());
        } else {
            break;
        }
    }
    return results;
}

/* TODO */
std::vector<string> DictionaryTrie::predictUnderscores(
    string pattern, unsigned int numCompletions) {
    return {};
}

/* This is the destructor */
DictionaryTrie::~DictionaryTrie() { deleteAll(root); }

/* Helper function for destructor. Recursively deletes all the nodes.
    argument: a pointer pointing to the root of the subtree to be deleted.
 */
void DictionaryTrie::deleteAll(Node* ptr) {
    if (ptr == 0) {
        return;
    }
    deleteAll(ptr->left);
    deleteAll(ptr->mid);
    deleteAll(ptr->right);
    delete ptr;
}

/* inorder traverse through the subtree with given root, store all words
    in subtree into vector.
        arguments: the root of the subtree, a vector to store all of the words
 */
void DictionaryTrie::inorderTraversal(Node* ptr, vector<Node*>& vtr) {
    if (ptr == nullptr) {
        return;
    }
    inorderTraversal(ptr->left, vtr);
    if (ptr->is_word) {
        vtr.push_back(ptr);
    }
    inorderTraversal(ptr->mid, vtr);
    inorderTraversal(ptr->right, vtr);
}
/* the comparator used in sorting nodes based on their frequecy.
        arguments: two nodes to be compared
 */
bool DictionaryTrie::CompFreq::operator()(const Node* p1, const Node* p2) {
    return p1->freq > p2->freq;
}
/*  Create a node. Argument: a letter to be inserted  */
DictionaryTrie::Node::Node(char letter)
    : letter(letter), is_word(false), freq(0) {
    left = nullptr;
    mid = nullptr;
    right = nullptr;
    parent = nullptr;
}
/* get the word from the chosen word node by iteration to the root  */
string DictionaryTrie::Node::getWord() {
    if (!is_word) {
        return "";
    }
    string s = "";
    Node* ptr = this;
    s = ptr->letter + s;
    while (ptr->parent != nullptr) {
        if (ptr->parent->mid == ptr) {
            s = ptr->parent->letter + s;
        }
        ptr = ptr->parent;
    }
    return s;
}