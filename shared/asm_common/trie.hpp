#pragma once
#include <string_view>

// Trie on the 'A'-'Z' alphabet
template<typename Value>
class Trie
{
public:
    void emplace(const std::string_view& key, const Value& value);
    bool find(const std::string_view& key, Value& value);
    void clear();

    Trie() = default;
    ~Trie() { clear(); }
    Trie(const Trie&) = delete;
    Trie& operator=(const Trie&) = delete;
private:
    static constexpr size_t ALPHABET_SIZE = 26;

    struct Node
    {
        Node* children[ALPHABET_SIZE]{};
        Value value;
        bool hasValue = false;
    };

    void clear(Node* node);

    Node m_root;
};

template<typename Value>
void Trie<Value>::emplace(const std::string_view& key, const Value& value)
{
    Node* current = &m_root;
    for (char c : key)
    {
        int index = c - 'A';
        if (index < 0 || index >= ALPHABET_SIZE)
            return;

        if (!current->children[index])
            current->children[index] = new Node{};

        current = current->children[index];
    }

    current->hasValue = true;
    current->value = value;
}

template<typename Value>
inline bool Trie<Value>::find(const std::string_view& key, Value& value)
{
    Node* current = &m_root;
    for (char c : key)
    {
        int index = c - 'A';
        if (index < 0 || index >= ALPHABET_SIZE)
            return false;

        if (!current->children[index])
            return false;

        current = current->children[index];
    }

    if (!current->hasValue)
        return false;

    value = current->value;
    return true;
}

template<typename Value>
void Trie<Value>::clear()
{
    for (unsigned int i = 0; i < ALPHABET_SIZE; i++)
        if (m_root.children[i])
            clear(m_root.children[i]);
}

template<typename Value>
void Trie<Value>::clear(Node* node)
{
    for (unsigned int i = 0; i < ALPHABET_SIZE; i++)
        if (node->children[i])
            clear(node->children[i]);

    delete node;
}
