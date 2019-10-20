# TODO: Define dictionary_trie using function library()
# define the ​library object ​(not an executable object => DictionaryTrie.cpp without main() method) 
dictionary_trie = library('dictionary_trie', sources: ['DictionaryTrie.cpp', 'DictionaryTrie.hpp'])
# the directories to add to the header search path
inc = include_directories('.')

dictionary_trie_dep = declare_dependency(include_directories: inc,
  link_with: dictionary_trie)