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
#include <chrono>
#include <queue>
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
        unsigned int maxFreq;

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

    /* function for wildcard prediction
          arguments: pattern with (or without) underscore(s)
                      number of completions desired
          return: a list of completions.
          hints: if the patter is empty, the returned list is empty
                if the patter is without underscore, return that patter
                  if it is a word in the trie
    */
    vector<string> predictUnderscores(string pattern,
                                      unsigned int numCompletions);

    /* This is the destructor */
    ~DictionaryTrie();

  private:
    /* the comparator used in sorting nodes based on their frequecy.
        arguments: two nodes to be compared
     */
    struct CompFreq {
        bool operator()(const pair<int, string>& p1,
                        const pair<int, string>& p2);
    };

    struct CompNodeByMaxFrequent {
        bool operator()(const Node* ptr1, const Node* ptr2);
    };
    /* Helper function for destructor. Recursively deletes all the nodes.
        argument: a pointer pointing to the root of the subtree to be deleted.
     */
    void deleteAll(Node* ptr);

    /* traverse through the subtree with given root, prune the branch if the
      root of that branch fail to meet the requirement of being pushed to PQ
        arguments: the root of the subtree,
        a path showing how to reach the parent of the root, or empty if no
      parent,
        a piority queue to store all of the words,
        the max size of PQ, which equals to numCompletions
     */
    void dfs(Node* ptr, string path,
             priority_queue<pair<int, string>, vector<pair<int, string>>,
                            CompFreq>& q,
             int k);

    /* helper method for predictUnderscore, so that we could fill in the
       underscore and recursive down. arguments: subpattern (ignore the part
       before the undersocre we just filled in)， a pointer pointing to node the
       lettter of which is used to fill in the underscore a priority_queue used
       to store the completions k (numOfCompletions) determines the max-size of
       PQ we want, in order to improve efficiency
     */
    void underscoreHelper(
        string pattern, Node* ptr,
        priority_queue<pair<int, string>, vector<pair<int, string>>, CompFreq>&
            q,
        int k);
};

#endif  // DICTIONARY_TRIE_HPP
