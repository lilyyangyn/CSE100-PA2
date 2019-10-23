/**
 * This file declares the structure of DictionaryTrie class,
 * including its private variables and public methods.
 *
 * Author: Yuening YANG, Shenlang ZHOU
 * Email: y3yang@ucsd.edu shzhou@ucsd.edu
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
    /* It is the constructor*/

    DictionaryTrie();

    /* This is the function to insert the word into the trie.
        arguments: word to insert, frequency of that word
        return: true if insertion is successful, false otherwise
     */
    bool insert(string word, unsigned int freq);

    /* This is the function to find whether the word is in the trie.
        arguments: the target word
        return true if the word is found, false otherwise
     */
    bool find(string word) const;

    /* Use frequency to complete the predict completions.
        arguments: prefix, number of completions return.
        return: a list of completions, sorted by their frequency
     */
    vector<string> predictCompletions(string prefix,
                                      unsigned int numCompletions);

    /* TODO */
    vector<string> predictUnderscores(string pattern,
                                      unsigned int numCompletions);

    /* This is the destructor */
    ~DictionaryTrie();

  private:
    /* Helper function for destructor. Recursively deletes all the nodes.
        argument: a pointer pointing to the root of the subtree to be deleted.
     */
    void deleteAll(Node* ptr);

    /* inorder traverse through the subtree with given root, store all words
      in subtree into vector.
        arguments: the root of the subtree, a vector to store all of the words
     */
    void inorderTraversal(Node* ptr, vector<Node*>& vtr);

    /* the comparator used in sorting nodes based on their frequecy.
        arguments: two nodes to be compared
     */
    struct CompFreq {
        bool operator()(const Node* p1, const Node* p2);
    };
};
#endif  // DICTIONARY_TRIE_HPP
