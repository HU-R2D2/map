//
// Created by chiel on 10/05/16.
//

#ifndef MAP_RTREELEAF_HPP
#define MAP_RTREELEAF_HPP

#include "RTree.hpp"

namespace r2d2 {

    template<int MIN, int MAX, typename T>
    class RTreeLeaf : public RTree<MIN, MAX, T> {
        using RTree<MIN, MAX, T>::shared_from_this;
        using RTree<MIN, MAX, T>::bounds;
        using RTree<MIN, MAX, T>::parent;
    public:
        RTreeLeaf(Box bounds, T &data) :
                RTree<MIN, MAX, T>{bounds},
                data(data) {
        }

        virtual void search(const Box &box, std::vector<std::shared_ptr<r2d2::RTree<MIN, MAX, T>>> &add_to) override {
            if (bounds.intersects(box)) {
                add_to.push_back(this->shared_from_this());
            }
        }

        virtual std::shared_ptr<r2d2::RTree<MIN, MAX, T>> find_leaf(
                const std::shared_ptr<const r2d2::RTree<MIN, MAX, T>> node,
                int max_depth = -1) override {
            return shared_from_this();
        }

        virtual void insert(std::shared_ptr<r2d2::RTree<MIN, MAX, T>> node) override {
            parent->insert(node); // a leaf cannot insert into itself
        }

        virtual T *get_data() const override {
            return &data;
        }

        virtual std::ostream &print(std::ostream &rhs, int level) const override {
            for (int i = 0; i < level; ++i) {
                rhs << ((i == level - 1) ? "+#" : "| ");
            }
            return rhs << this->bounds << std::endl;
        }

    private:
        T data;

    };

}

#endif //MAP_RTREELEAF_HPP
