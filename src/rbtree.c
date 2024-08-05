#include "rbtree.h"

#include <stdlib.h>

void rbtree_insert_fixup(rbtree *t, node_t *curNode);

node_t SENTINEL = { // 전역 변수는 프로그램이 시작할 때 자동으로 할당되므로 malloc 노피료
    .color = RBTREE_BLACK,
    .key = -1,
    .left = NULL,
    .right = NULL,
    .parent = NULL};

rbtree *new_rbtree(void)
{
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree)); // rbtree를 가리키는 포인터
  p->nil = &SENTINEL;
  p->root = p->nil;
  return p;
}

// 회전, 재조정 함수 선언해주기
void rbtree_insert_fixup(rbtree *t, node_t *tempFixupNode)
{
  // z -> tempFixupNode
  node_t *tempFixupUncleNode = t->nil;
  while (tempFixupNode->parent->color == RBTREE_RED)
  {
    if (tempFixupNode->parent == tempFixupNode->parent->parent->left)
    {
      tempFixupUncleNode = tempFixupNode->parent->parent->right;
      if (tempFixupUncleNode->color == RBTREE_RED)
      {
        tempFixupNode->parent->color = RBTREE_BLACK;
        tempFixupUncleNode->color = RBTREE_BLACK;
        tempFixupNode->parent->parent->color = RBTREE_RED;
        tempFixupNode = tempFixupNode->parent->parent;
      }
      else
      {
        if (tempFixupNode == tempFixupNode->parent->right)
        {
          tempFixupNode = tempFixupNode->parent;
          rbtree_LeftRotate(t, tempFixupNode);
        }
        tempFixupNode->parent->color = RBTREE_BLACK;
        tempFixupNode->parent->parent = RBTREE_RED;
        rbtree_RightRotate(t, tempFixupNode);
      }
    }
    else
    {
      tempFixupUncleNode = tempFixupNode->parent->parent->left;
      if(tempFixupUncleNode->color == RBTREE_RED){
        tempFixupNode->parent->color = RBTREE_BLACK;
        tempFixupUncleNode->color = RBTREE_BLACK;
        tempFixupNode->parent->parent = RBTREE_RED;
        tempFixupNode = tempFixupNode->parent->parent;
      }

      else{
        if(tempFixupNode == tempFixupNode->parent->left){
          tempFixupNode = tempFixupNode->parent;
          rbtree_RightRotate(t, tempFixupNode);
        }
        tempFixupNode->parent->color = RBTREE_BLACK;
        tempFixupNode->parent->parent->color = RBTREE_RED;
        rbtree_LeftRotate(t, tempFixupNode);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

void rbtree_LeftRotate(rbtree *t, node_t *newNode);
{
  // x ->curLeftNode, y ->curLeftChildNode
  node_t *curLeftNode = t->nil;
  node_t *curLeftChildNode = t->nil;
  curLeftChildNode = curLeftNode->right;
  curLeftNode->right = curLeftChildNode->left;
  if (curLeftChildNode->left != t->nil)
  {
    curLeftChildNode->left->parent =
  }
}

void rbtree_RightRotate(rbtree *t, node_t *newNode);
{
  // x ->curRightNode
  node_t *curLeftNode = t->nil;
  node_t *curLeftChildNode = t->nil;
}

void delete_rbtree(rbtree *t)
{
  // TODO: reclaim the tree nodes's memory
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key)
{
  node_t *parentNode = t->nil;
  node_t *tempNode = t->root;
  node_t *newNode = (node_t *)calloc(1, sizeof(node_t));
  newNode->key = key;

  while (tempNode != t->nil)
  {
    parentNode = tempNode;
    if (newNode->key < tempNode->key)
    {
      tempNode = tempNode->left;
    }
    else
    {
      tempNode = tempNode->right;
    }
  }
  newNode->parent = parentNode;

  if (parentNode == t->nil)
  {
    t->root = newNode;
  }
  else if (newNode->key < parentNode->key)
  {
    parentNode->left = newNode;
  }
  else
  {
    parentNode->right = newNode;
  }

  newNode->left = t->nil;
  newNode->right = t->nil;
  newNode->color = RBTREE_RED;

  rbtree_insert_fixup(t, newNode);

  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key)
{
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t)
{
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t)
{
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p)
{
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  // TODO: implement to_array
  return 0;
}
