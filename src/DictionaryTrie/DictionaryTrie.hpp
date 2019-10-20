/**
 * This file declares the structure of DictionaryTrie class,
 * including its private variables and public methods.
 *
 * Author: Yuening YANG, Shenlang ZHOU
 * Email: y3yang@ucsd.edu
 */
#ifndef DICTIONARY_TRIE_HPP
#define DICTIONARY_TRIE_HPP

#include <string>
#include <utility>
#include <vector>

using namespace std;

/**
 * The class for a dictionary ADT, implemented as either
 * a mulit-way trie or a ternary search tree.
 */
class DictionaryTrie {
  private:
    /* inner class which defines a node of ternary trie */
    class Node {
      public:
        Node* left;
        Node* mid;
        Node* right;
        char letter;
        bool is_word;
        unsigned int freq;

        Node(char letter) : letter(letter), is_word(false), freq(0) {
            left = nullptr;
            mid = nullptr;
            right = nullptr;
        }
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
    /* helper function for destructor */
    void deleteAll(Node* ptr);
};
#endif  // DICTIONARY_TRIE_HPP
