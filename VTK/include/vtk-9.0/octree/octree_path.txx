// Included by octree

/**\typedef template<typename T_,typename R_,typename P_,typename O_,typename OP_,int d_> \
 *         typedef O_ octree_path<T_,R_,P_,O_,OP_,d_>::octree_type;
 * \brief Shorthand for an octree over which this class can iterate.
 */

/**\typedef template<typename T_,typename R_,typename P_,typename O_,typename OP_,int d_> \
 *         typedef OP_ octree_path<T_,R_,P_,O_,OP_,d_>::octree_pointer;
 * \brief Shorthand for a pointer to an octree over which this class can iterate.
 */

/**\typedef template<typename T_,typename R_,typename P_,typename O_,typename OP_,int d_> \
 *         typedef typename O_::allocator_type
 * octree_path<T_,R_,P_,O_,OP_,d_>::octree_allocator_type; \brief Shorthand for the allocator used
 * by the octrees over which this class iterates.
 */

/**\typedef template<typename T_,typename R_,typename P_,typename O_,typename OP_,int d_> \
 *         typedef typename O_::octree_node_reference
 * octree_path<T_,R_,P_,O_,OP_,d_>::octree_node_reference; \brief Shorthand for a reference to a
 * node in the octree.
 */

/**\typedef template<typename T_,typename R_,typename P_,typename O_,typename OP_,int d_> \
 *         typedef typename O_::octree_node_pointer
 * octree_path<T_,R_,P_,O_,OP_,d_>::octree_node_pointer; \brief Shorthand for a pointer to a node in
 * the octree.
 */

/**\typedef template<typename T_,typename R_,typename P_,typename O_,typename OP_,int d_> \
 *         typedef octree_path< T_, T_&, T_*, O_, O_*, d_ > octree_path<T_,R_,P_,O_,OP_,d_>::path;
 * \brief Shorthand for a non-const octree path (regardless of whether the current path is const or
 * not).
 */

/**\typedef template<typename T_,typename R_,typename P_,typename O_,typename OP_,int d_> \
 *         typedef octree_path< T_, const T_&, const T_*, O_, const O_*, d_ >
 * octree_path<T_,R_,P_,O_,OP_,d_>::const_path; \brief Shorthand for a const octree path (regardless
 * of whether the current path is const or not).
 */

/**\typedef template<typename T_,typename R_,typename P_,typename O_,typename OP_,int d_> \
 *         typedef octree_path< T_, R_, P_, O_, OP_, d_ >
 * octree_path<T_,R_,P_,O_,OP_,d_>::self_path; \brief Shorthand for an path of the same type as the
 * current path (be it const or not).
 */

/**\var template<typename T_,typename R_,typename P_,typename O_,typename OP_,int d_> \
 *     octree_node_pointer octree_path<T_,R_,P_,O_,OP_,d_>::_M_root
 *\brief The root of the octree we are iterating over.
 */

/**\var template<typename T_,typename R_,typename P_,typename O_,typename OP_,int d_> \
 *     std::vector<octree_node_pointer> octree_path<T_,R_,P_,O_,OP_,d_>::_M_parents
 *\brief List of parent nodes
 */

/**\var template<typename T_,typename R_,typename P_,typename O_,typename OP_,int d_> \
 *     std::vector<int> octree_path<T_,R_,P_,O_,OP_,d_>::_M_indices
 *\brief List of parent child indices
 */

/**\var template<typename T_,typename R_,typename P_,typename O_,typename OP_,int d_> \
 *     octree_node_pointer octree_path<T_,R_,P_,O_,OP_,d_>::_M_current_node
 *\brief Current node at the head of the path
 */

/**\brief Default constructor.
 */
template <typename T_, typename R_, typename P_, typename O_, typename OP_, int d_>
octree_path<T_, R_, P_, O_, OP_, d_>::octree_path()
{
  this->_M_root = nullptr;
  this->_M_current_node = nullptr;
}

/**\brief Simplest valid constructor.
 *
 * This creates a path that points to the root node of the specified \a tree.
 */
template <typename T_, typename R_, typename P_, typename O_, typename OP_, int d_>
octree_path<T_, R_, P_, O_, OP_, d_>::octree_path(octree_pointer tree)
{
  this->_M_root = tree->root();
  this->_M_current_node = tree->root();
}

/**\brief Simplest valid constructor.
 *
 * This creates a path that points to the \a root node of the specified tree.
 */
template <typename T_, typename R_, typename P_, typename O_, typename OP_, int d_>
octree_path<T_, R_, P_, O_, OP_, d_>::octree_path(octree_node_pointer root)
{
  this->_M_root = root;
  this->_M_current_node = root;
}

/**\brief Flexible constructor.
 *
 * This creates a path that points to a particular node of the specified \a tree,
 * given a \a path of nodes to descend from the root of the \a tree.
 */
template <typename T_, typename R_, typename P_, typename O_, typename OP_, int d_>
octree_path<T_, R_, P_, O_, OP_, d_>::octree_path(
  octree_node_pointer root, std::vector<int>& children)
{
  this->_M_root = root;
  this->_M_current_node = root;
  for (std::vector<int>::iterator cit = children.begin(); cit != children.end(); ++cit)
  {
    this->_M_parents.push_back(this->_M_current_node);
    this->_M_indices.push_back(*cit);
    this->_M_current_node = (*this->_M_current_node)[*cit];
  }
}

/**\fn template< typename T_, typename R_, typename P_, typename O_, typename OP_, int d_ > \
 *    octree_node_reference octree_path<T_,R_,P_,O_,OP_,d_>::operator * () const
 *\brief Provide access to the node at the current path head.
 */

/**\fn template< typename T_, typename R_, typename P_, typename O_, typename OP_, int d_ > \
 *    octree_node_pointer octree_path<T_,R_,P_,O_,OP_,d_>::operator -> () const
 *\brief Provide access to the node at the current path head.
 */

/**\fn template< typename T_, typename R_, typename P_, typename O_, typename OP_, int d_ > \
 *    int octree_path<T_,R_,P_,O_,OP_,d_>::level() const
 *\brief Return the depth of the current path.
 *
 * The root node of the octree is at level 0. Its children are all at level 1. Their children are at
 *level 2, and so forth.
 */

/**\fn template< typename T_, typename R_, typename P_, typename O_, typename OP_, int d_ > \
 *    bool octree_path<T_,R_,P_,O_,OP_,d_>::operator == ( const path& it )
 *\brief Compare two paths for equality.
 *
 * Iterators are considered equal iff they have the same octree and point to the same node
 *currently. The rest of the paths' state is ignored by the test.
 */

/**\fn template< typename T_, typename R_, typename P_, typename O_, typename OP_, int d_ > \
 *    bool octree_path<T_,R_,P_,O_,OP_,d_>::operator == ( const const_path& it )
 *\brief Compare two paths for equality.
 *
 * Iterators are considered equal iff they have the same octree and point to the same node
 *currently. The rest of the paths' state is ignored by the test.
 */

/**\fn template< typename T_, typename R_, typename P_, typename O_, typename OP_, int d_ > \
 *    bool octree_path<T_,R_,P_,O_,OP_,d_>::operator != ( const path& it )
 *\brief Compare two paths for inequality.
 *
 * Iterators are considered equal iff they have the same octree and point to the same node
 *currently. The rest of the paths' state is ignored by the test.
 */

/**\fn template< typename T_, typename R_, typename P_, typename O_, typename OP_, int d_ > \
 *    bool octree_path<T_,R_,P_,O_,OP_,d_>::operator != ( const const_path& it )
 *\brief Compare two paths for inequality.
 *
 * Iterators are considered equal iff they have the same octree and point to the same node
 *currently. The rest of the paths' state is ignored by the test.
 */
