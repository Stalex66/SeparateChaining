#ifndef ADS_SET_H
#define ADS_SET_H

#include <functional>
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <memory>
#include <stdexcept>


template <typename Key, size_t N = 13> // Startgröße der Tabelle
class ADS_set {
public:
    class Iterator;
    using value_type = Key;
    using key_type = Key;
    using reference = key_type&;
    using const_reference = const key_type&;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;
    using iterator = Iterator;
    using const_iterator = Iterator;
    using key_equal = std::equal_to<key_type>; // Hashing
    using hasher = std::hash<key_type>;        // Hashing
private:
    //values
    enum class Mode { free, used };

    static constexpr size_type table_size{ N };

    size_type curr_size{ 0 };

    struct element { // enthält die Werte
        key_type key;
        element* next;
        Mode mode{ Mode::free };

        element() {}
        element(const element& other) {
            *this = other;
        }
        element& operator=(const element& other) {
            key = other.key;
            if (other.mode != Mode::free) {
                next = new element(*other.next);
            }
            mode = other.mode;
            return *this;
        }
    };
    element* table{ new element[N] };

    //functions

    static size_t h(const key_type& key) { return hasher{}(key) % table_size; };
    element* insert_intern(const key_type& key);
    element* find_intern(const key_type& key) const {return find_intern(key, h(key));}
    element* find_intern(const key_type& key, size_type idx)const;

public:
    ADS_set() {}

    ADS_set(std::initializer_list<key_type> ilist) : ADS_set{} { insert(ilist); }

    template<typename InputIt> ADS_set(InputIt first, InputIt last) : ADS_set{} { insert(first, last); }

    ADS_set(const ADS_set& other) { // siehe Table problem
        curr_size = other.curr_size;
        for (size_type i{ 0 }; i < table_size; ++i) {
            table[i] = other.table[i];
        }
    }

    ~ADS_set() { // löscht erst alle Elemente von hinten nach vorne und danach die Tabelle
        clear_chains();
        delete[] table;
    }

    void clear_chains() { //löscht alle Elemente nach den ersten Elementen -> erste Elemente bleiben unverändert!
        /*
        for (size_t i{ 0 }; i < N; i++) {
            element* aktuell = &table[i];
            int level = 0; // variable welche speichert das wievielte Element das letzte ist
            bool stop = true;
            while (stop) {
                if (aktuell->mode == Mode::used) { aktuell = aktuell->next; level++; }
                if (aktuell->mode == Mode::free) { stop = false; break; }
            }
            for (; 0 < level; --level) {
                int level2 = level; // variable welche speichert das wievielte Element -1 es gibt
                element* aktuell2 = &table[i];
                for (; 1 < level2; --level2) { aktuell2 = aktuell2->next; }
                delete aktuell2->next;
            }
        }
        */
        for (size_t i{ 0 }; i < N; i++) {
            element* aktuell = &table[i];
            if (aktuell->mode == Mode::free) continue;
            aktuell = aktuell->next;
            while (aktuell->mode != Mode::free) {
                element* alt = aktuell;
                aktuell = aktuell->next;
                delete alt;
            }
            delete aktuell;
        }
    }

    ADS_set& operator=( ADS_set other) {
        if (this == &other) return *this;
        ADS_set copy = other;
        clear();
        std::swap(table, copy.table);
        std::swap(curr_size, copy.curr_size);
        return *this;
    }

    ADS_set& operator=(std::initializer_list<key_type> ilist) {
        clear();
        insert(ilist);
        return *this;
    }

    size_type size() const { return curr_size; }

    bool empty() const { return curr_size == 0; }

    size_type count(const key_type& key) const { return static_cast<bool>(find_intern(key)); } // wie oft das Element vorkommt 0/1 !! gibt 1 -> ein pointer 0 = nullptr

    iterator find(const key_type& key) const {
        size_type idx{ h(key) };
        element* found = find_intern(key, idx);
        if (found == nullptr)
            return end();
        return Iterator(found, idx, table);
    }

    void clear() { // löscht alle Elemente von hinten nach vorne
        clear_chains();
        for (size_t i{ 0 }; i < table_size; ++i) {
            table[i].mode = Mode::free;
        }
        curr_size = 0;
    }
    void swap(ADS_set& other) { // bitte testen ob das hier funktioniert
        if (this == &other) return;
        std::swap(table, other.table);
        std::swap(curr_size, other.curr_size);
    }

    void insert(std::initializer_list<key_type> ilist) {
        insert(ilist.begin(), ilist.end());
    }

    std::pair<iterator, bool> insert(const key_type& key) {
        std::pair <iterator, bool> ret;
        iterator var = find(key);
        if (var == end()) {
            auto element = insert_intern(key);
            var = Iterator(element,h(key),table); // glaube das sollte so passen mal schauen bitte
            ret = std::make_pair(var, true);
        }
        else { ret = std::make_pair(var, false); }

        return ret;
    }

    template<typename InputIt> void insert(InputIt first, InputIt last);



    size_type erase(const key_type& key) {
        size_t index = h(key);
        element* val = &table[index];
        if (val->mode == Mode::free)
            return 0;

        if (key_equal{}(val->key, key)) { // wenn das erste objekt ist ausnahme
            element* next = val->next;
            val->key = next->key;
            val->next = next->next;
            val->mode = next->mode;
            delete next;
            curr_size--;
            return 1;
        }

        element* previous = val;
        val = val->next;
        while (val->mode != Mode::free) {
            if (key_equal{}(val->key, key)) { // anderes als erstes objekt
                previous->next = val->next;
                delete val;
                curr_size--;
                return 1;
            }else{
                previous = val;
                val = val->next;
            }
        }
        return 0;
    }


    const_iterator begin() const {
        for (size_t index = 0; index < table_size; ++index) {
            element* a = &table[index];
            if (a->mode != Mode::free)
                return const_iterator(a, index, table);
        }
        return end();
    }
    const_iterator end() const {
        return const_iterator(nullptr, table_size, nullptr);
    }


    void dump(std::ostream& o = std::cerr) const;

    friend bool operator==(const ADS_set& lhs, const ADS_set& rhs) {
        if (lhs.curr_size != rhs.curr_size) // für anfang ungleich wenn das net gleich
            return false;

        for (auto& l : lhs)
            if (!rhs.count(l)) return false;
        return true;

        /* "schneller", weniger lesbar
        for (size_t index = 0; index < table_size; ++index) {
            element* comp1 = &lhs.table[index];
            element* comp2 = &rhs.table[index];
            while (comp1->mode != Mode::free) {
                auto const& key = comp1->key;
                element* comp2a = comp2;
                do {
                    if (key_equal{}(comp2a->key, key)) {
                        break;
                    }
                    comp2a = comp2a->next;
                } while (comp2a->mode != Mode::free);
                if (comp2a->mode == Mode::free)
                    return false;
                comp1 = comp1->next;
            }
        }
        return true;
        */
    }
    friend bool operator!=(const ADS_set& lhs, const ADS_set& rhs) {
        return !(operator==(lhs, rhs));
    }
};

template <typename Key, size_t N>
typename ADS_set<Key, N>::element* ADS_set<Key, N>::insert_intern(const key_type& key) {
    /*size_type idx{ h(key) };

    element* prufung = &table[idx];
    while (prufung->mode != Mode::free) {
        prufung = prufung->next;
    }
    prufung->key = key;
    prufung->mode = Mode::used;
    prufung->next = new element;
    ++curr_size;
    return prufung; retourniert Iterator auf position von insert*/
    size_type idx{ h(key) };
    element* help;
    help = new element;
    help->next = &table[idx];
    help->key = key;
    help->mode = Mode::used;
    help->next = new element;
    ++curr_size;
    table[idx] = *help;
    return help;



}

template <typename Key, size_t N>
typename ADS_set<Key, N>::element* ADS_set<Key, N>::find_intern(const key_type& key, size_type idx) const {
    element* prufung = &table[idx];
    while (true) {
        if (prufung->mode == Mode::used && key_equal{}(prufung->key, key)) return prufung;
        if (prufung->mode == Mode::free) return nullptr;
        prufung = prufung->next;
    }
}



template <typename Key, size_t N>
template<typename InputIt> void ADS_set<Key, N>::insert(InputIt first, InputIt last) {
    for (; first != last; ++first) {
        if (!count(*first)) {
            insert_intern(*first);
        }
    }
}

template <typename Key, size_t N>
void ADS_set<Key, N>::dump(std::ostream& o) const {
    o << "curr_size = " << curr_size << " table_size = " << table_size << "\n";
    for (size_type idx{ 0 }; idx < table_size; ++idx) {
        o << idx << ": ";
        switch (table[idx].mode) {
        case Mode::free:
            o << "--free";
            break;
        case Mode::used:
            o << table[idx].key;

            element* prufung = &table[idx];
            while (prufung->mode != Mode::free) {
                prufung = prufung->next; 
                if (prufung->mode == Mode::used){ o << "--" << prufung->key; }
            }
            o << "-free";
            break;
        }
        o << "\n";
    }
}




template <typename Key, size_t N>
class ADS_set<Key, N>::Iterator {
public:
    using Mode = ADS_set::Mode;
    using value_type = Key;
    using difference_type = std::ptrdiff_t;
    using reference = const value_type&;
    using pointer = const value_type*;
    using iterator_category = std::forward_iterator_tag;
    using element = ADS_set::element;

private:
    static constexpr size_t table_size = N;

    element* ptr;
    size_t curr_idx;
    element* table;
public:
    Iterator() : ptr{ nullptr }, curr_idx{ 0 }, table{ nullptr }{}
    Iterator(element* ptr2, size_t curr_idx2, element* table2) { ptr = ptr2; table = table2; curr_idx = curr_idx2; }
    /*~Iterator() { // braucht der Iterator einenen Dekonstruktor ????
        delete ptr;
        delete[] table;
    }*/
    reference operator*() const { return (ptr->key); }

    pointer operator->() const { return &(ptr->key); }

    Iterator& operator++() {
        // wenn mode == used
        if (ptr->next->mode != Mode::free) {
            // nächster pointer ist used
            ptr = ptr->next;
        } else {
            // nächster pointer ist leer
            for (curr_idx = curr_idx + 1; curr_idx < table_size; ++curr_idx) {
                ptr = &table[curr_idx];
                if (ptr->mode == Mode::used)
                    break;
            }
        }

        return *this;
    }


    Iterator operator++(int) {
        Iterator neu(*this);
        this->operator++();
        return neu;
    }

    friend bool operator==(const Iterator& lhs, const Iterator& rhs) {
        return lhs.curr_idx == table_size && rhs.curr_idx == table_size
            || lhs.ptr == rhs.ptr;
    }

    friend bool operator!=(const Iterator& lhs, const Iterator& rhs) {
        return !(lhs == rhs);
    }
};

template <typename Key, size_t N> void swap(ADS_set<Key, N>& lhs, ADS_set<Key, N>& rhs) { lhs.swap(rhs); }

#endif
