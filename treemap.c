#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"


typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap* new = (TreeMap *)malloc(sizeof(TreeMap));
    new->root = NULL;
    new->current = NULL;
    new->lower_than = lower_than;
    return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
  tree->current= tree->root;
  TreeNode* new = createTreeNode(key,value);
  while(1){
    if (tree->current){
      if (is_equal(tree,tree->current->pair->key,key)) break;
      if (tree->lower_than(tree->current->pair->key,key)) {
        if(tree->current->right){
          tree->current=tree->current->right;
        }
        else{
          tree->current->right = new;
          new->parent = tree->current;
        }
      }
      else{
        if(tree->current->left){
          tree->current=tree->current->left;
        }
        else{
          tree->current->left = new;
          new->parent = tree->current;
        }
        
      }
      
    }
  }
}


TreeNode * minimum(TreeNode * x){
    while(x){
      if (x->left == NULL) return x;
      else x= x->left;
    }
    return NULL;
}


void removeNode(TreeMap * tree, TreeNode* node) {
  if (node->right && node->left){//dos hijos
    TreeNode* replacement = minimum(node->right);
    node->pair = replacement->pair;
    removeNode(tree,replacement);
  }
  else if (node->right || node->left){//1 hijo
    if (node->right){
      if (node->parent){
        if(tree->lower_than(node->pair->key,node->parent->pair->key)) {
          node->parent->left=node->right;
          }
        else {
          node->parent->right=node->right;
          }

      }
      else tree->root = node->right;
      
    node->right->parent=node->parent;
      }
  else{
      if (node->parent){
        if(tree->lower_than(node->pair->key,node->parent->pair->key)) {
          node->parent->left=node->left;
          }
        else {
          node->parent->right=node->left;
          }

      }
      else tree->root = node->left;
      
    node->left->parent=node->parent;
    }
  
  }
  else{
    if (node->parent){
      if(tree->lower_than(node->pair->key,node->parent->pair->key)) node->parent->left=NULL;
      else node->parent->right=NULL;
    }
    else tree->root = NULL;
  }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
  tree->current= tree->root;
  while(tree->current){
      if (is_equal(tree,tree->current->pair->key,key)) return tree->current->pair;
      if (tree->lower_than(tree->current->pair->key,key)) {
          tree->current=tree->current->right;}
      else{
          tree->current=tree->current->left;}
      }
    return NULL;
  }


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    tree->current = minimum(tree->root);
    return tree->current->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
  if (tree->current){
    TreeNode* prev = tree->current;
    if(tree->current->right){
      tree->current= minimum(tree->current->right);
      return tree->current->pair;
    }
    else{
      while(tree->current){
        tree->current=tree->current->parent;
        if (tree->lower_than(prev->pair->key,tree->current->pair->key)){
          return tree->current->pair;
        }
      }
    }
  return NULL;
}
