#include <stdio.h> #include <stdlib.h> typedef struct Book {
int acc;	/* accession number (key) */ struct Book *left, *right;
} Book;
Book *insert(Book *root, int acc)
{
// TODO 1: If root is NULL create and return a new node. Otherwise insert into the
// left subtree if acc is smaller, the right subtree if larger; ignore duplicates.
// Return root
}
int height(const Book *r)	/* height of an empty tree is -1 */
{
// TODO 2: Return -1 for an empty tree, otherwise 1 + the larger of the two subtree
// heights
}
int search(const Book *r, int acc, int *cmp)
{
*cmp = 0; while (r) {
(*cmp)++;
if (acc == r->acc) return 1;
r = acc < r->acc ? r->left : r->right;
}
return 0;
}
void inorder(const Book *r)
{
if (!r) return; inorder(r->left); printf("%d ", r->acc); inorder(r->right);
}
void destroy(Book *r)
{
if (!r) return;
