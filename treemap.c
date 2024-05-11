#include "treemap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct TreeNode TreeNode;

struct TreeNode {
  Pair *pair;
  TreeNode *left;
  TreeNode *right;
  TreeNode *parent;
};

struct TreeMap {
  TreeNode *root;
  TreeNode *current;
  int (*lower_than)(void *key1, void *key2);
};

int is_equal(TreeMap *tree, void *key1, void *key2) {
  if (tree->lower_than(key1, key2) == 0 && tree->lower_than(key2, key1) == 0)
    return 1;
  else
    return 0;
}

TreeNode *createTreeNode(void *key, void *value) {
  TreeNode *new = (TreeNode *)malloc(sizeof(TreeNode));
  if (new == NULL)
    return NULL;
  new->pair = (Pair *)malloc(sizeof(Pair));
  new->pair->key = key;
  new->pair->value = value;
  new->parent = new->left = new->right = NULL;
  return new;
}

TreeMap *createTreeMap(int (*lower_than)(void *key1, void *key2)) {
  TreeMap *new_tree = (TreeMap *)malloc(sizeof(TreeMap));
  if (new_tree == NULL)
    return NULL;
  new_tree->root = NULL;
  new_tree->current = NULL;
  new_tree->lower_than = lower_than;
  return new_tree;
}

void insertTreeMap(TreeMap *tree, void *key, void *value) {
  if (tree == NULL || key == NULL || value == NULL)
    return;

  TreeNode *new_node = createTreeNode(key, value);
  if (new_node == NULL)
    return;

  if (tree->root == NULL) {
    tree->root = new_node;
  } else {
    TreeNode *current = tree->root;
    TreeNode *parent = NULL;

    while (current != NULL) {
      parent = current;
      if (tree->lower_than(key, current->pair->key)) {
        current = current->left;
      } else if (tree->lower_than(current->pair->key, key)) {
        current = current->right;
      } else {
        // Clave duplicada, liberar memoria y salir
        free(new_node->pair);
        free(new_node);
        return;
      }
    }

    new_node->parent = parent;
    if (tree->lower_than(key, parent->pair->key)) {
      parent->left = new_node;
    } else {
      parent->right = new_node;
    }
  }

  tree->current = new_node; // Actualizamos tree->current
}

TreeNode *minimum(TreeNode *x) {
  if (x == NULL)
    return NULL;

  while (x->left != NULL) {
    x = x->left;
  }

  return x;
}

void removeNode(TreeMap *tree, TreeNode *node) {
  if (node->left == NULL && node->right == NULL) {
    if (node->parent == NULL) {
      tree->root = NULL;
    } else if (node == node->parent->left) {
      node->parent->left = NULL;
    } else {
      node->parent->right = NULL;
    }
  } else if (node->left == NULL) {
    if (node->parent == NULL) {
      tree->root = node->right;
    } else if (node == node->parent->left) {
      node->parent->left = node->right;
    } else {
      node->parent->right = node->right;
    }
    node->right->parent = node->parent;
  } else if (node->right == NULL) {
    if (node->parent == NULL) {
      tree->root = node->left;
    } else if (node == node->parent->left) {
      node->parent->left = node->left;
    } else {
      node->parent->right = node->left;
    }
    node->left->parent = node->parent;
  } else {
    TreeNode *successor = minimum(node->right);
    node->pair = successor->pair;
    removeNode(tree, successor);
    return; // Se sale de la función después de eliminar el sucesor
  }
  free(node); // Liberación de memoria solo para el nodo
}

void eraseTreeMap(TreeMap *tree, void *key) {
  if (tree == NULL || tree->root == NULL)
    return;

  if (searchTreeMap(tree, key) == NULL)
    return;
  TreeNode *node = tree->current;
  removeNode(tree, node);
}

Pair *searchTreeMap(TreeMap *tree, void *key) {
  if (tree == NULL || key == NULL)
    return NULL;

  TreeNode *current = tree->root;
  while (current != NULL) {
    int compare_result = tree->lower_than(key, current->pair->key);
    if (compare_result) {
      current = current->left;
    } else if (tree->lower_than(current->pair->key, key)) {
      current = current->right;
    } else {
      // Se encontró el nodo con la clave especificada
      tree->current = current; // Actualizamos tree->current
      return current->pair;
    }
  }

  // No se encontró ningún nodo con la clave especificada
  return NULL;
}

Pair *upperBound(TreeMap *tree, void *key) { return NULL; }

Pair *firstTreeMap(TreeMap *tree) {
  if (tree == NULL || tree->root == NULL) {
    return NULL;
  }

  TreeNode *current = tree->root;
  while (current->left != NULL) {
    current = current->left;
  }

  tree->current = current;
  return current->pair;
}

Pair *nextTreeMap(TreeMap *tree) {
  if (tree == NULL || tree->root == NULL || tree->current == NULL) {
    return NULL;
  }

  TreeNode *current = tree->current;

  // Si el nodo tiene un hijo derecho, el siguiente nodo es el nodo más a la
  // izquierda del subárbol derecho.
  if (current->right != NULL) {
    current = current->right;
    while (current->left != NULL) {
      current = current->left;
    }
    tree->current = current;
    return current->pair;
  }

  // Si el nodo no tiene un hijo derecho, el siguiente nodo es el primer
  // ancestro cuyo hijo izquierdo es también un ancestro del nodo.
  TreeNode *parent = current->parent;
  while (parent != NULL && current == parent->right) {
    current = parent;
    parent = parent->parent;
  }
  tree->current = parent;
  return (parent != NULL) ? parent->pair : NULL;
}
