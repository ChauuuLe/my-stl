#pragma once

namespace mystd {
    template<class T>
    class segment_tree {
    private:
        using merge_type = T(const T&, const T&);
        struct query {
            int left_query, right_query;
            T value;
            std::function<merge_type> update_op;

            template<class... Args>
            query(int left_query, int right_query, T value, Args&&... args) :
                left_query(left_query), right_query(right_query), value(value), update_op(std::forward<Args>(args)...) {}

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
            _update(id * 2, l, mid, this_query);
            _update(id * 2 + 1, mid + 1, r, this_query);

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
            T left = _get(id * 2, l, mid, this_query);
            T right = _get(id * 2 + 1, mid + 1, r, this_query);

            return this->merge(
                left,
                right
            );
        }
    public:
        segment_tree(int n, const T& val, const std::function<merge_type>& f): 
                nelem(n), nodes(4 * n + 7, val), merge(f) {}

        template<class... Args>
        segment_tree(int n, const T& val, Args&&... args):
                nelem(n), nodes(4 * n + 7, val), merge(std::forward<Args>(args)...) {}

        segment_tree() : nelem(0),
            merge([](T& x, T& y) {
                return std::max(x, y);
            }) {}
        
        template<class... Args>    
        void update(int left_query, int right_query, const T& value, Args&&... args)  {
            query this_query = query(left_query, right_query, value, std::forward<Args>(args)...);

            _update(1, 1, this->nelem, this_query);
        }

        void update(int left_query, int right_query, const T& value, const std::function<merge_type>& update_op)  {
            query this_query = query(left_query, right_query, value, update_op);
            
            _update(1, 1, this->nelem, this_query);
        }
        
        T get(int left_query, int right_query, const T& default_value) {
            if (left_query > right_query) {
                return default_value;
            }
            
            query this_query = query(left_query, right_query, default_value);

            return _get(1, 1, this->nelem, this_query);
        }
    };
}