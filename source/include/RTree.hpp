// ++--++
// @file RTree.hpp
// @date Created: <9-05-16>
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

#ifndef MAP_RTREE_HPP
#define MAP_RTREE_HPP

#include <vector>
#include <memory>
#include <Box.hpp>

namespace r2d2 {

    template<int MIN, int MAX, typename T>
    class RTreeBranch;

    template<int MIN, int MAX, typename T>
    class RTree : public std::enable_shared_from_this<RTree<MIN, MAX, T>> {
    public:
        RTree(Box bounds) :
                parent{nullptr},
                bounds{bounds} {
        }

	    //! get the bounds of this node, see #bounds
        const Box &get_bounds() const {
            return bounds;
        }

        //! search for all the leaves within this node
        //!
        //! \parameter box the bounds to search the boxes of
        //! \parameter add_to the vector that the found leaves should be added to
        virtual void search(const Box &box, std::vector<std::shared_ptr<RTree<MIN, MAX, T>>> &add_to) = 0;

        //! find the leaf that is best suited for insertion of a given node
        //!
        //! \parameter node the node that is to be inserted into this tree
        //! \parameter max_depth the depth that the search should stop at,
        //!                      and instead take a branch as result
        virtual std::shared_ptr<RTree<MIN, MAX, T>> find_leaf(
                const std::shared_ptr<const RTree<MIN, MAX, T>> node,
                int max_depth = -1) = 0;

        virtual void insert(std::shared_ptr<RTree<MIN, MAX, T>> node) = 0;

	    //! get the data that is associated with this node
	    //!
	    //! for branches this function should return nullptr,
	    //! as only leaves can store any data.
	    //! this is also the reason for it being a raw pointer instead of
	    //! a reference, because it is impossible to return null data from branches otherwise.
	    //! it is also not a shared_ptr because of the fact that the data is stored raw in the leaf
        virtual T *get_data() const = 0;

	    //! remove this node from the tree, and rebalance the tree as a result
	    //!
        //! be sure that when you call this function the node is not
        //! disconnected from any tree, otherwise you will segfault.
        //! also note that you should not do anything within this node after
        //! calling this function without holding the shared_ptr to it.
        virtual void remove() {
            parent->remove(this);
        }

        //! print out the map to the ostream 'rhs', used for debugging purposes
        //!
        //! \parameter rhs the ostream to be printed into
        //! \parameter level the current depth of the tree that is being printed
        virtual std::ostream &print(std::ostream &rhs, int level) const {
            for (int i = 0; i < level; ++i) {
                rhs << ((i == level - 1) ? "+-" : "| ");
            }
            return rhs << bounds;
        }

        friend std::ostream &operator<<(std::ostream &rhs, const RTree<MIN, MAX, T> &lhs) {
            return lhs.print(rhs, 0);
        }

    protected:
        // A normal pointer is used here as the parent should not be kept alive by this child,
        // and we are unable to use weak_ptr because the inability to get a weak pointer to the set parent caller.
        // In theory this pointer should never have an deleted parent, otherwise the leaf would no longer
        // be in a existing tree, and any functions that call the parent would not be called in such a case
        RTreeBranch<MIN, MAX, T>* parent;

        //! the bounds that this node covers.
	    //!
        //! for leaves, this directly correlates to a single box in the map,
        //! and for branches it is the combination of the bounds of all it's children.
        Box bounds;

    public:
	    //! update the parent pointer within this node
	    //!
	    //! this method has to be public to allow for correct template
	    //! resolution, please don't use it outside of this class.
	    void set_parent(RTreeBranch<MIN, MAX, T> *parent) {
		    this->parent = parent;
	    }

    };

}

#endif //MAP_RTREE_HPP
