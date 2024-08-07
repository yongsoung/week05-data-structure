#include "rbtree.h"

#include <stdlib.h>

void rbtree_insert_fixup(rbtree *t, node_t *tempFixupNode);
void erase_rbtree_fixup(rbtree *t, node_t *deleteFixupNode);
void rbtree_LeftRotate(rbtree *t, node_t *curLNode);
void rbtree_RightRotate(rbtree *t, node_t *newNode);
void rbtree_transplant(rbtree *t, node_t *originNode, node_t *transNode);

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

void recursive_delete_rbtree(rbtree *t, node_t *curNode)
{
  if (curNode != t->nil)
  {
    // 재귀
    recursive_delete_rbtree(t, curNode->right);
    recursive_delete_rbtree(t, curNode->left);
    free(curNode);
  }
}

void delete_rbtree(rbtree *t)
{
  recursive_delete_rbtree(t, t->root);
  // while (t->root != t->nil)
  // {
  //   rbtree_erase(t, t->root);
  // }

  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key)
{
  node_t *parentNode = t->nil;
  node_t *tempNode = t->root;
  node_t *newNode = malloc(sizeof(node_t));
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
  node_t *tempNode = t->root;
  while (tempNode != t->nil)
  {
    if (key < tempNode->key)
    {
      tempNode = tempNode->left;
    }
    else if (key > tempNode->key)
    {
      tempNode = tempNode->right;
    }
    else
      return tempNode;
  }
  return NULL;
}

node_t *rbtree_min_by_node(const rbtree *t, const node_t *node)
{
  node_t *minNode = node;
  while (minNode->left != t->nil)
  {
    minNode = minNode->left;
  }
  return minNode;
}

node_t *rbtree_min(const rbtree *t)
{
  // TODO: implement find
  node_t *minNode = t->root;
  while (minNode->left != t->nil)
  {
    minNode = minNode->left;
  }
  return minNode;
}

node_t *rbtree_max(const rbtree *t)
{
  // TODO: implement find
  node_t *maxNode = t->root;
  while (maxNode->right != t->nil)
  {
    maxNode = maxNode->right;
  }
  return maxNode;
}

node_t *rbtree_find_successor(const rbtree *t, node_t *node)
{
  node_t *successor = node;
  while (successor->left != t->nil)
  {
    successor = successor->left;
  }
  return successor;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  node_t *curNode = t->root;
  node_t *prevNode = t->nil;
  node_t *tempNode = t->nil;
  int index = 0;

  while (curNode != t->nil)
  {
    if (curNode->left != t->nil)
    {
      prevNode = curNode->left;

      while (prevNode->right != t->nil)
      {
        prevNode = prevNode->right;
      }
      prevNode->right = curNode;
      tempNode = curNode;
      curNode = curNode->left;
      tempNode->left = t->nil;
    }
    else
    {
      arr[index++] = curNode->key;
      if (index >= n)
      {
        break;
      }
      curNode = curNode->right;
    }
  }

  return 0;
}

int rbtree_erase(rbtree *t, node_t *deleteNode)
{
  // TODO: implement erase
  node_t *y = deleteNode;
  node_t *deleteChildNode;
  color_t yOriginalColor = y->color;

  if (deleteNode->left == t->nil)
  {
    deleteChildNode = deleteNode->right;
    rbtree_transplant(t, deleteNode, deleteNode->right);
  }
  else if (deleteNode->right == t->nil)
  {
    deleteChildNode = deleteNode->left;
    rbtree_transplant(t, deleteNode, deleteNode->left);
  }
  else
  {
    y = rbtree_min_by_node(t, deleteNode->right);
    yOriginalColor = y->color;
    deleteChildNode = y->right;
    if (y->parent == deleteNode)
    {
      deleteChildNode->parent = y;
    }
    else
    {
      rbtree_transplant(t, y, y->right);
      y->right = deleteNode->right;
      y->right->parent = y;
    }

    rbtree_transplant(t, deleteNode, y);
    y->left = deleteNode->left;
    y->left->parent = y;
    y->color = deleteNode->color;
  }
  if (yOriginalColor == RBTREE_BLACK)
  {
    erase_rbtree_fixup(t, deleteChildNode);
  }
  free(deleteNode);
  return 0;
}

void erase_rbtree_fixup(rbtree *t, node_t *deleteFixupNode)
{
  node_t *siblNode;
  while (deleteFixupNode != t->root && deleteFixupNode->color == RBTREE_BLACK)
  {
    if (deleteFixupNode == deleteFixupNode->parent->left)
    {
      siblNode = deleteFixupNode->parent->right;
      if (siblNode->color == RBTREE_RED)
      {
        siblNode->color = RBTREE_BLACK;
        deleteFixupNode->parent->color = RBTREE_RED;
        rbtree_LeftRotate(t, deleteFixupNode->parent);
        siblNode = deleteFixupNode->parent->right;
      }
      if (siblNode->left->color == RBTREE_BLACK && siblNode->right->color == RBTREE_BLACK)
      {
        siblNode->color = RBTREE_RED;
        deleteFixupNode = deleteFixupNode->parent;
      }
      else
      {
        if (siblNode->right->color == RBTREE_BLACK)
        {
          siblNode->left->color = RBTREE_BLACK;
          siblNode->color = RBTREE_RED;
          rbtree_RightRotate(t, siblNode);
          siblNode = deleteFixupNode->parent->right;
        }
        siblNode->color = deleteFixupNode->parent->color;
        deleteFixupNode->parent->color = RBTREE_BLACK;
        siblNode->right->color = RBTREE_BLACK;
        rbtree_LeftRotate(t, deleteFixupNode->parent);
        deleteFixupNode = t->root;
      }
    }
    else
    {
      siblNode = deleteFixupNode->parent->left;
      if (siblNode->color == RBTREE_RED)
      {
        siblNode->color = RBTREE_BLACK;
        deleteFixupNode->parent->color = RBTREE_RED;
        rbtree_RightRotate(t, deleteFixupNode->parent);
        siblNode = deleteFixupNode->parent->left;
      }
      if (siblNode->right->color == RBTREE_BLACK && siblNode->left->color == RBTREE_BLACK)
      {
        siblNode->color = RBTREE_RED;
        deleteFixupNode = deleteFixupNode->parent;
      }
      else
      {
        if (siblNode->left->color == RBTREE_BLACK)
        {
          siblNode->right->color = RBTREE_BLACK;
          siblNode->color = RBTREE_RED;
          rbtree_LeftRotate(t, siblNode);
          siblNode = deleteFixupNode->parent->left;
        }
        siblNode->color = deleteFixupNode->parent->color;
        deleteFixupNode->parent->color = RBTREE_BLACK;
        siblNode->left->color = RBTREE_BLACK;
        rbtree_RightRotate(t, deleteFixupNode->parent);
        deleteFixupNode = t->root;
      }
    }
  }
  deleteFixupNode->color = RBTREE_BLACK;
}

void rbtree_LeftRotate(rbtree *t, node_t *curLNode)
{
  // x ->curLNode, y ->curLChildNode
  node_t *curLChildNode = curLNode->right;
  curLNode->right = curLChildNode->left;
  if (curLChildNode->left != t->nil)
  {
    curLChildNode->left->parent = curLNode;
  }
  curLChildNode->parent = curLNode->parent;
  if (curLNode->parent == t->nil)
  {
    t->root = curLChildNode;
  }
  else if (curLNode == curLNode->parent->left)
  {
    curLNode->parent->left = curLChildNode;
  }
  else
  {
    curLNode->parent->right = curLChildNode;
  }
  curLChildNode->left = curLNode;
  curLNode->parent = curLChildNode;
}

void rbtree_RightRotate(rbtree *t, node_t *curRNode)
{
  // x ->curRNode
  node_t *curRChildNode = curRNode->left;
  curRNode->left = curRChildNode->right;
  if (curRChildNode->right != t->nil)
  {
    curRChildNode->right->parent = curRNode;
  }
  curRChildNode->parent = curRNode->parent;
  if (curRNode->parent == t->nil)
  {
    t->root = curRChildNode;
  }
  else if (curRNode == curRNode->parent->right)
  {
    curRNode->parent->right = curRChildNode;
  }
  else
  {
    curRNode->parent->left = curRChildNode;
  }
  curRChildNode->right = curRNode;
  curRNode->parent = curRChildNode;
}

// transplant 과정(삭제)
void rbtree_transplant(rbtree *t, node_t *originNode, node_t *transNode)
{
  if (originNode->parent == t->nil)
  {
    t->root = transNode;
  }
  else if (originNode == originNode->parent->left)
  {
    originNode->parent->left = transNode;
  }
  else
  {
    originNode->parent->right = transNode;
  }
  transNode->parent = originNode->parent;
}

// 회전, 재조정 함수 선언해주기
void rbtree_insert_fixup(rbtree *t, node_t *tempFixupNode)
{
  // z -> tempFixupNode
  node_t *tempFixupUncleNode;
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
        tempFixupNode->parent->parent->color = RBTREE_RED;
        rbtree_RightRotate(t, tempFixupNode->parent->parent);
      }
    }
    else
    {
      tempFixupUncleNode = tempFixupNode->parent->parent->left;
      if (tempFixupUncleNode->color == RBTREE_RED)
      {
        tempFixupNode->parent->color = RBTREE_BLACK;
        tempFixupUncleNode->color = RBTREE_BLACK;
        tempFixupNode->parent->parent->color = RBTREE_RED;
        tempFixupNode = tempFixupNode->parent->parent;
      }

      else
      {
        if (tempFixupNode == tempFixupNode->parent->left)
        {
          tempFixupNode = tempFixupNode->parent;
          rbtree_RightRotate(t, tempFixupNode);
        }
        tempFixupNode->parent->color = RBTREE_BLACK;
        tempFixupNode->parent->parent->color = RBTREE_RED;
        rbtree_LeftRotate(t, tempFixupNode->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}