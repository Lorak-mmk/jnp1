#ifndef INSERTION_ORDERED_MAP_H
#define INSERTION_ORDERED_MAP_H

#include <list>
#include <memory>
#include <unordered_map>
#include <utility>

namespace iom_impl {
template <class K, class Hash>
struct HashPtr {
    Hash hash;
    std::size_t operator()(K const &arg) const { return hash(*arg); }
};

template <class K>
struct EqualPtr {
    constexpr bool operator()(const K &l, const K &r) const { return *l == *r; }
};

template <class K, class V, class Hash>
class Container {
   public:
    using elem_t = std::pair<K, V>;
    using order_t = std::list<elem_t>;
    using iterator_t = typename order_t::iterator;
    using const_iterator_t = typename order_t::const_iterator;
    using key_t = const K *;
    using map_t = std::unordered_map<key_t, iterator_t, iom_impl::HashPtr<key_t, Hash>, iom_impl::EqualPtr<key_t>>;

    map_t table;
    order_t order;
    bool blockCoW;

    Container() = default;
    Container(const Container &other) : table(), order(other.order), blockCoW(false) {
        for (iterator_t it = order.begin(); it != order.end(); it++) {
            auto to_insert = iterator_t(it);
            table.emplace(&to_insert->first, to_insert);
        }
    }
    Container(Container &&other) noexcept = default;
    ~Container() = default;
};

template <class K, class V, class Hash>
std::shared_ptr<Container<K, V, Hash>> empty_state = std::make_shared<Container<K, V, Hash>>();

}  // namespace iom_impl

class lookup_error : public std::exception {};

template <class K, class V, class Hash = std::hash<K>>
class insertion_ordered_map {
   private:
    using Container_t = iom_impl::Container<K, V, Hash>;
    using v_iterator_t = typename Container_t::iterator_t;

    class CopyManager;

    std::shared_ptr<Container_t> data;

   public:
    using iterator = typename Container_t::const_iterator_t;

    insertion_ordered_map() : data(std::make_shared<Container_t>()) {}

    insertion_ordered_map(insertion_ordered_map const &other) {
        this->data = other.data;
        if (data->blockCoW) {
            CopyManager manager(this);
            manager.success();
        }
    }

    insertion_ordered_map(insertion_ordered_map &&other) noexcept {
        if (other.data == iom_impl::empty_state<K, V, Hash>) {
            data = iom_impl::empty_state<K, V, Hash>;
            return;
        } else {
            data = std::move(other.data);
            other.data = iom_impl::empty_state<K, V, Hash>;
        }
    }

    ~insertion_ordered_map() = default;

    insertion_ordered_map &operator=(insertion_ordered_map other) {
        this->data = other.data;
        return *this;
    }

    bool insert(K const &k, V const &v) {
        CopyManager manager(this);

        if (contains(k)) {
            data->order.splice(data->order.end(), data->order, data->table.find(&k)->second);
            return false;
        }

        v_iterator_t it = data->order.emplace(data->order.end(), k, v);
        try {
            data->table.emplace(&it->first, it);
        } catch (...) {
            data->order.erase(it);
            throw;
        }
        data->blockCoW = false;
        manager.success();
        return true;
    }

    void erase(K const &k) {
        CopyManager manager(this);
        auto it = data->table.find(&k);
        if (it == data->table.end()) throw lookup_error();
        data->order.erase(it->second);
        data->table.erase(it);
        data->blockCoW = false;
        manager.success();
    }

    void merge(insertion_ordered_map const &other) {
        if (this == &other) return;
        CopyManager manager(this, true);
        for (auto v : other) {
            insert(v.first, V(v.second));
        }
        manager.success();
    }

    V &at(K const &k) {
        CopyManager manager(this);
        auto it = data->table.find(&k);
        if (it == data->table.end()) throw lookup_error();
        manager.success();
        data->blockCoW = true;
        return it->second->second;
    }

    V const &at(K const &k) const {
        auto it = data->table.find(&k);
        if (it == data->table.end()) throw lookup_error();
        return it->second->second;
    }

    V &operator[](K const &k) {
        if (!contains(k)) {
            insert(k, V());
        }
        data->blockCoW = true;
        V &ret = at(k);
        return ret;
    }

    size_t size() const noexcept { return data->table.size(); }

    bool empty() const noexcept { return data->table.size() == 0; }

    void clear() {
        CopyManager manager(this);
        data->table.clear();
        data->order.clear();
        data->blockCoW = false;
        manager.success();
    }

    bool contains(K const &k) const { return data->table.count(&k) != 0; }

    iterator begin() const noexcept { return data->order.begin(); }

    iterator end() const noexcept { return data->order.end(); }
};

template <class K, class V, class Hash>
class insertion_ordered_map<K, V, Hash>::CopyManager {
   private:
    using Container_t = iom_impl::Container<K, V, Hash>;
    insertion_ordered_map<K, V, Hash> *object;
    std::shared_ptr<Container_t> tmp_data;
    bool ok;

   public:
    CopyManager() = delete;

    CopyManager(insertion_ordered_map<K, V, Hash> *obj, bool force_copy = false)
        : object(obj), ok(false), tmp_data(object->data) {
        if (object->data.use_count() > 2 || force_copy) {
            object->data = std::make_shared<Container_t>(*(object->data));
        } else {
            ok = true;
        }
    }

    void success() { ok = true; }

    ~CopyManager() {
        if (!ok) {
            object->data = tmp_data;
        }
    }
};

#endif
