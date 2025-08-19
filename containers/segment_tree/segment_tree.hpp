#pragma once

namespace mystd {
    template<class T>
    class segment_tree {
    private:
        using merge_type = T&(const T&, const T&);
        struct query {
            int left_query, right_query;
            T& value;
            std::function<merge_type>& update_op;

            query(int left_query, int right_query, T value, std::function<merge_type>&& update_op) :
                left_query(left_query), right_query(right_query), value(value), update_op(std::move(update_op)) {}

            query(int left_query, int right_query, T value, const std::function<merge_type>& update_op) :
                left_query(left_query), right_query(right_query), value(value), update_op(update_op) {}
            
            query(int left_query, int right_query, T value) :
                left_query(left_query), right_query(right_query), value(value) {}
        };
        
        int nelem;
        std::function<merge_type> merge;
        std::vector<T> nodes;

        void _update(int id, int l, int r, const query& this_query) {
            if (this_query.left_query > r || l > this_query.right_query) {
                return;
            }
            if (this_query.left_query <= l && r <= this_query.right_query) {
                this->nodes[id] = this_query.update_op(this->nodes[id], this_query.value);
                return;
            }

            int mid = l + (r - l) / 2;
            _update(id * 2, l, mid);
            _update(id * 2 + 1, mid + 1, r);

            this->nodes[id] = this->merge(this->nodes[id * 2], this->nodes[id * 2 + 1]);
        }

        T _get(int id, int l, int r, const query& this_query) {
            if (this_query.left_query > r || l > this_query.right_query) {
                return this_query.value;
            }
            if (this_query.left_query <= l && r <= this_query.right_query) {
                return this->nodes[id];
            }

            int mid = l + (r - l) / 2;
            return this->merge(
                _get(id * 2, l, mid, this_query.value),
                _get(id * 2 + 1, mid + 1, r, this_query.value)
            );
        }
    public:
        segment_tree(int n, std::function<merge_type> f = std::max<T>): nelem(n), nodes(4 * n + 1) {}
        segment_tree() : nelem(0), merge(std::max<T>) {}
        
        void update(int left_query, int right_query, const T& value, std::function<merge_type>&& update_op)  {
            query this_query = query(left_query, right_query, value, std::move(update_op));

            _update(1, 1, this->nelem, this_query);
        }

        void update(int left_query, int right_query, const T& value, const std::function<merge_type>& update_op)  {
            query this_query = query(left_query, right_query, value, update_op);

            _update(1, 1, this->nelem, this_query);
        }
        
        T get(int left_query, int right_query, const T& default_value) {
            query this_query = query(left_query, right_query, default_value);

            return _get(1, 1, this->nelem, this_query);
        }
    };
}