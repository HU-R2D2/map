// ++--++
// @file RStarMap.hpp
// @date Created: <10-05-16>
// @version <1.0.0>
//
// @author Chiel Douwes
//
// @section LICENSE
// License: newBSD
//
// Copyright � 2016, HU University of Applied Sciences Utrecht.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
// - Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
// - Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
// - Neither the name of the HU University of Applied Sciences Utrecht nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE HU UNIVERSITY OF APPLIED SCIENCES UTRECHT
// BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
// GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
// OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// ++--++

#ifndef MAP_RTREEBRANCH_HPP
#define MAP_RTREEBRANCH_HPP

#include <limits>
#include <algorithm>
#include <cassert>

#include "RTree.hpp"

namespace r2d2 {

    template<int MIN, int MAX, typename T>
    class RTreeBranch : public RTree<MIN, MAX, T> {
    protected:
        using RTree<MIN, MAX, T>::shared_from_this;
        using RTree<MIN, MAX, T>::set_parent;
        using RTree<MIN, MAX, T>::remove;
        using RTree<MIN, MAX, T>::parent;
        using RTree<MIN, MAX, T>::bounds;

    public:
        RTreeBranch(std::vector<std::shared_ptr<RTree<MIN, MAX, T>>> children) :
                RTreeBranch::RTree({}) {
	        // we need at least two children per branch to be a tree structure
            static_assert(MIN >= 2, "MIN must be >= 2");
            static_assert(MAX >= MIN * 2, "MAX must be >= MIN*2");
            assert(children.size() <= MAX);
            for (unsigned int i = 0; i < children.size(); ++i) {
                this->children[i] = children[i];
                children[i]->set_parent(this);
                bounds = i == 0 ? children[0]->get_bounds() : bounds.get_union_box(children[i]->get_bounds());
            }
            num_children = int(children.size());
        }

        template<long unsigned int S>
        RTreeBranch(std::array<std::shared_ptr<RTree<MIN, MAX, T>>, S> children) :
                RTreeBranch::RTree({}) {
            static_assert(MIN >= 2, "MIN must be >= 2");
            static_assert(MAX >= MIN * 2, "MAX must be >= MIN*2");
            static_assert(0 < S <= MAX, "children cannot have more elements than MAX");
            bounds = children[0]->get_bounds();
            for (int i = 0; i < S; ++i) {
                this->children[i] = children[i];
                children[i]->set_parent(this);
                bounds = bounds.get_union_box(children[i]->get_bounds());
            }
            num_children = int(S);
        }

        virtual void search(const Box &box, std::vector<std::shared_ptr<RTree<MIN, MAX, T>>> &add_to) override {
            if (bounds.intersects(box)) {
                for (int i = 0; i < num_children; ++i) {
                    children[i]->search(box, add_to);
                }
            }
        }

        // search for the leaf that is best suited for insertion of 'node'
        virtual std::shared_ptr<RTree<MIN, MAX, T>> find_leaf(
                const std::shared_ptr<const RTree<MIN, MAX, T>> node,
                int max_depth = -1) override {
            // search for the best match to insert
            if (max_depth == 0) {
                return shared_from_this();
            } else {
                unsigned long bestIndex = 0;
                Length bestBox{
                        get_union_score(children[0]->get_bounds(), node->get_bounds())
                };
                for (int i = 1; i < num_children; ++i) {
                    // find the node with the least area plus enlargement
                    Length newBox{get_union_score(
                            children[i]->get_bounds(), node->get_bounds()
                    )};
                    if (newBox < bestBox) {
                        // this box takes less area and enlargement, therefore it is more efficient
                        bestIndex = i;
                        bestBox = newBox;
                    }
                }
                return children[bestIndex]->find_leaf(node, max_depth - 1);
            }
        };

        // worst case O(log(n) * (MAX + MAX^2))
        // amortized O(log(n))
        // best case O(log(n))
        virtual void insert(std::shared_ptr<RTree<MIN, MAX, T>> node) override {
            children[num_children++] = node;
            node->set_parent(this);
            if (num_children >= MAX) {
                // split the node into equal parts
                auto splitNode = split();

                // propagate split recursively upward
                parent->insert(splitNode);
            } else {
                // spit stopped propagating
                // add bounds as the inner children could not have been removed
                add_bound(node->get_bounds());
            }

            // TODO if this is not the root level and this is the first call of OverflowTreatment
            // in the given level during the insertion of one given rectangle then invoke ReInsert
            // - the r* paper
            // reinsertion might be somewhat good for the performance, but adds too much complexity to the current program
        }

        virtual T *get_data() const override {
            return nullptr; // return null as the branch does not actually store any data
        }

        // O(log(n)*MAX) worst case
        virtual void remove(const RTree<MIN, MAX, T> *node, const std::vector<std::shared_ptr<RTree<MIN, MAX, T>>> raise_nodes = {}) {
            std::shared_ptr<RTree<MIN, MAX, T>> found{};
            for (auto child : children) {
                if (child.get() == node) {
                    found = child;
                }
            }
            if (found != nullptr) {
                (*std::remove(children.begin(), children.begin() + num_children, found)) = nullptr;
                --num_children;

                // do the node insertion after the node removal to prevent splits,
                // but do it before node underflow te prevent removal
                for (auto raise_node : raise_nodes) {
                    // try inserting the raised node into this node
                    find_leaf(raise_node, 1)->insert(raise_node);
                }

                if (num_children < MIN) {
                    // underfull node, add all the children to the parent
                    underflow_treatment();
                } else {
                    remove_bound(node->get_bounds());
                }
            } else {
                // the node was not found, insert the raised ones anyways
                for (auto raise_node : raise_nodes) {
                    insert(raise_node);
                }
            }
        }

        virtual std::ostream &print(std::ostream &rhs, int level) const override {
            RTree<MIN, MAX, T>::print(rhs, level) << std::endl;
            for (int i = 0; i < num_children; ++i) {
                children[i]->print(rhs, level + 1);
            }
            return rhs;
        }

    private:
        // O(1)
        static const Length &get_axis(const Coordinate &coord, int axis) {
            switch (axis) {
                default:
                case 0:
                    return coord.get_x();
                case 1:
                    return coord.get_y();
                case 2:
                    return coord.get_z();
            }
        }

        // O(n log(n))
        void sort(int axis) {
            std::sort(children.begin(), children.begin() + num_children,
                      [axis](const std::shared_ptr<RTree<MIN, MAX, T>> &e1,
                             const std::shared_ptr<RTree<MIN, MAX, T>> &e2) {
                          const Box &e1_bounds{e1->get_bounds()}, &e2_bounds{e2->get_bounds()};
                          const Length &min_axis1 = get_axis(e1_bounds.get_bottom_left(), axis),
                                  &min_axis2 = get_axis(e2_bounds.get_bottom_left(), axis);
                          if (min_axis1 < min_axis2) {
                              return -1;
                          } else if (min_axis2 < min_axis1) {
                              return 1;
                          } else {
                              const Length &max_axis1 = get_axis(e1_bounds.get_top_right(), axis),
                                      &max_axis2 = get_axis(e2_bounds.get_top_right(), axis);
                              if (max_axis1 < max_axis2) {
                                  return -1;
                              } else if (max_axis2 < max_axis1) {
                                  return 1;
                              } else {
                                  return 0;
                              }
                          }
                      });
        }

        virtual void underflow_treatment() {
            // double removal/insertion to avoid object delete problems
            parent->remove(this, {children.begin(), children.begin() + num_children});
        }

        // O(1)
        static const Length get_union_score(const Box &b1, const Box &b2) {
            Box union_box{b1.get_union_box(b2)};
            return get_box_margin(union_box) * 2.0 - get_box_margin(b2);
                    // box margin is used here instead of the usual box size as it is not feasible in 3d
        }

        // O(1)
        static const Length get_box_margin(const Box &box) {
            const Translation &axis_size{box.get_axis_size()};
            return axis_size.get_x() + axis_size.get_y() + axis_size.get_z();
        }

        // O(1)
        static double get_box_size(const Box &box) {
            Translation &axis_size{box.get_axis_size()};
            return axis_size.get_x() / Length::METER
                   * axis_size.get_y() / Length::METER
                   * axis_size.get_z() / Length::METER;
        }

    protected:
        #define NUM_AXES 3
        // O((n log(n) + (n^2)) * NUM_AXES+1) = O(n^2)
        // best case O(n^2)
        std::shared_ptr<RTree<MIN, MAX, T>> split() {
            // find the most efficient split between the nodes currently in the set
            int best_axis = 0, best_spilt = 0;
            Box best_split_bounds{};
            // set the score to infinity here so the first split will always be better
            double best_split_score = std::numeric_limits<double>::infinity();
            for (int axis = 0; axis < NUM_AXES; ++axis) {
                // do a sort as it the most efficient way to find minimal overlap
                sort(axis);
                Box front_margin{};
                // update the front margin to reflect the minimum amount of children
                for (int i = 0; i < MIN; ++i) {
                    front_margin = front_margin.get_union_box(children[i]->get_bounds());
                }
                for (int i = MIN - 1; i < num_children - MIN; ++i) {
                    // the back margin has to be recomputed every box, as it cannot be dynamically updated.
                    // this takes the complexity of this function up to O(n^2), but it is worth it for small numbers of MAX
                    Box back_margin{};
                    for (int j = i + 1; j < num_children; ++j) {
                        back_margin = back_margin.get_union_box(children[j]->get_bounds());
                    }

                    double first_score = get_box_margin(front_margin) / Length::METER,
                            second_score = get_box_margin(back_margin) / Length::METER,
                            overlap =
                            get_axis(children[i + 1]->get_bounds().get_bottom_left(), axis) / Length::METER
                            - get_axis(children[i]->get_bounds().get_top_right(), axis) / Length::METER,
                    // overlap also includes separation from the two boxes
                            total_score = first_score * first_score + second_score * second_score + overlap;
                    // taking the square of the score does two things:
                    // 1. it tries to minimise the difference between the box sizes
                    // 2. it tries to maximise the squareness of the boxes
                    if (total_score < best_split_score) {
                        best_split_score = total_score;
                        best_spilt = i;
                        best_axis = axis;
                        best_split_bounds = front_margin;
                    }
                }
            }

            // the best spilt has been found, now extract if from the array
            if (best_axis != NUM_AXES - 1) {
                // the last axis sorted can be skipped
                sort(best_axis);
            }
            std::vector<std::shared_ptr<RTree<MIN, MAX, T>>> new_tree;
            for (int i = best_spilt + 1; i < num_children; ++i) {
                new_tree.push_back(children[i]);
                children[i] = nullptr;
            }
            num_children = best_spilt + 1;
            // update the bounds of the parent(s)
            recompute_bounds();
            // bounds = bestSplitBounds;
            // we already know the bounds here, but it's not very elegant to use them

            return std::make_shared<RTreeBranch<MIN, MAX, T>>(new_tree);
        }

        // checks whether a certain box has one of it's edges on the edge of this rtree node
        // this can be used for things like checking whether to propagate bounds growing/shrinking
        bool has_edge(const Box box) {
            return !bounds.contains(box);
        }

        //! add a bound to this branch's bounding box, possibly expanding it
        virtual void add_bound(const Box box) {
            if (has_edge(box)) {
                bounds = bounds.get_union_box(box);

                parent->add_bound(box);
            }
        }

        //! remove a bound from this branch's bounding box, possibly shrinking it
        virtual void remove_bound(const Box box) {
            // if the removed node made up part of this node's edges
            if (has_edge(box)) {
                // shrink the bounds to fit the leftover nodes
                bounds = children[0]->get_bounds();
                for (int i = 1; i < num_children; ++i) {
                    bounds = bounds.get_union_box(children[i]->get_bounds());
                }

                parent->remove_bound(box);
            }
        }

        //! completely recalculate the bounds of this branch
        virtual void recompute_bounds() {
            bounds = children[0]->get_bounds();
            for (int i = 1; i < num_children; ++i) {
                bounds = bounds.get_union_box(children[i]->get_bounds());
            }

            parent->recompute_bounds();
        }

        std::array<std::shared_ptr<RTree<MIN, MAX, T>>, MAX> children;
        int num_children;

    };

}

#endif //MAP_RTREEBRANCH_HPP
