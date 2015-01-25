#ifndef __UBA_H_
#define __UBA_H_ /* __UBA_H_ */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief A structure representing an unbounded array.
 */
struct uba
{
  /** The data. */
  void** data;
  /** The length. */
  int length;
};

/**
 * create a new empty uba with inital size 15
 * @return a new empty uba
 */
struct uba*
uba_new ();

/**
 * free the memory used for an uba
 * @param u the uba to delete
 */
void
uba_delete (struct uba* u);

/**
 * add an element to the end of the list
 * @param u the uba to add to
 * @param el the element to add
 */
void
uba_add (struct uba* u, void* el);

/**
 * add an element to the specified index
 * @param u the uba to add to
 * @param index the index to add at
 * @param el the element to add
 */
void
uba_add_index (struct uba* u, int index, void* el);

/**
 * add all the contents of one uba to the end of another
 * @param usrc the uba that's contents will be added
 * @param udst the uba that will be appended onto
*/
void
uba_add_all (struct uba* usrc, struct uba* udst);

/**
 * add all the contents of one uba to another starting at an index
 * @param usrc the uba that's contents will be added
 * @param udst the uba that will be appended onto
 * @param index the index to start adding at in udst
 */
void
uba_add_all_index (struct uba* usrc, struct uba* udst, int index);

/**
 * clear the contents of an uba
 * @param u the uba to clear the contents of
 */
void
uba_clear (struct uba* u);

/**
 * check if an uba contains an element
 * @param u the uba to check in
 * @param el the element to check for in u
 * @return nonzero if u contains el
 */
int
uba_contains (struct uba* u, void* el);

/**
 * check if an uba contains another uba
 * @param uout the uba to check in
 * @param uin the (sub)uba to check for in uout
 * @return nonzero if uin is in uout
 */
int
uba_contains_all (struct uba* uout, struct uba* uin);

/**
 * check if two ubas are ==
 * @pararm u1 the first uba
 * @param u2 the second uba
 * @return nonzero if the ubas are ==
 */
int
uba_equals (struct uba* u1, struct uba* u2);

/**
 * get an item at an index in the uba
 * @param u the uba to get from
 * @param index the index of the item to get
 * @return u[index]
 */
void*
uba_get (struct uba* u, int index);

/**
 * get the index of an element
 * @param u the uba to get the index from
 * @param el the element to get the index of in u
 * @return the index of el in u, or -1 if el is not found in u
 */
int
uba_index_of (struct uba* u, void* el);

/**
 * get the last index of an element
 * @param u the uba to get the index from
 * @param el the element to get the last index of in u
 * @return the last index of el in u, or -1 if el is not found in u
 */
int
uba_last_index_of (struct uba* u, void* el);

/**
 * remore an index form the uba
 * @param u the uba to remove from
 * @param index the index to remove
 * @return the element that was removed, or (void**)0 if the element was not
 * found
 */
void*
uba_remove (struct uba* u, int index);

/**
 * remove the first occurance of an element from the uba
 * @param u the uba to remove from
 * @param el the element to remove
 * @return nonzero if the remove was successful
 */
int
uba_remove_element (struct uba* u, void* el);

/**
 * set an index to an element
 * @param u the uba to set an index
 * @param index the index to set
 * @param el the element to set u[index] to
 * @return whatever was at u[index] before executing u[index] = el
 */
void*
uba_set (struct uba* u, int index, void* el);

/**
 * get the length of an uba
 * @param u the uba to get the length of
 * @return the length of u
 */
int
uba_length (struct uba* u);

#ifdef __cplusplus
}
#endif

#endif /* __UBA_H_ */
