/**
 * This file declares the structure of DictionaryTrie class,
 * including its private variables and public methods.
 *
 * Author: Yuening YANG, Shenlang ZHOU
 * Email: y3yang@ucsd.edu
 */
#ifndef DICTIONARY_TRIE_HPP
#define DICTIONARY_TRIE_HPP

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

using namespace std;

/**
 * The class for a dictionary ADT, implemented as either
 * a mulit-way trie or a ternary search tree.
 * Here we use a ternary search tree
 */
class DictionaryTrie {
  private:
    /** inner class which defines a node of TST */
    class Node {
      public:
        Node* left;
        Node* mid;
        Node* right;
        Node* parent;
        char letter;
        bool is_word;
        unsigned int freq;

        Node(char letter);

        string getWord();
    };

    // ptr to the root of the trie, or 0 if empty trie
    Node* root;

  public:
    /* TODO: add function header */
    DictionaryTrie();

    /* TODO: add function header */
    bool insert(string word, unsigned int freq);

    /* TODO: add function header */
    bool find(string word) const;

    /* TODO: add function header */
    vector<string> predictCompletions(string prefix,
                                      unsigned int numCompletions);

    /* TODO: add function header */
    vector<string> predictUnderscores(string pattern,
                                      unsigned int numCompletions);

    /* TODO: add function header */
    ~DictionaryTrie();

  private:
    /** helper function for destructor */
    void deleteAll(Node* ptr);

    /** inorder traverse through the subtree with given root,
          store all words in subtree into vector */
    void inorderTraversal(Node* ptr, vector<Node*>& vtr);

    /** the comparator used in sorting nodes based on their frequecy */
    struct CompFreq {
        bool operator()(const Node* p1, const Node* p2);
    };
};
#endif  // DICTIONARY_TRIE_HPP
