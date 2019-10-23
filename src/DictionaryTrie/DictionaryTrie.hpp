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

    /* This is the function to insert the word into the trie. Argument is 
    frequency and the word itself and return wherther the word is well inserted*/
    bool insert(string word, unsigned int freq);

    /* This is the function to find whether the word is in the trie. The argument is the word and return
    whether it is found.*/
    bool find(string word) const;

   /* Use frequency to complete the predict completions. Arguments are prefix and number of 
results we need. Return the list with the predictions amounting to the number of numCompletions */
    vector<string> predictCompletions(string prefix,
                                      unsigned int numCompletions);

    /*  */
    vector<string> predictUnderscores(string pattern,
                                      unsigned int numCompletions);

    /* This is the destructor */
    ~DictionaryTrie();

  private:
    /** helper function for destructor */
    void deleteAll(Node* ptr);

    /** inorder traverse through the subtree with given root,
          store all words in subtree into vector The argument is a node and a vector to store all of the words*/
    void inorderTraversal(Node* ptr, vector<Node*>& vtr);

    /** the comparator used in sorting nodes based on their frequecy Get the frequency and compare them. The argument is the two nodes*/
    struct CompFreq {
        bool operator()(const Node* p1, const Node* p2);
    };
};
#endif  // DICTIONARY_TRIE_HPP
