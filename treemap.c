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
  if (tree == NULL || key == NULL)
    return;

  // Verificamos si la clave ya existe en el TreeMap
  if (searchTreeMap(tree, key) != NULL) {
    printf("La clave ya existe en el TreeMap.\n");
    return;
  }

  // Creamos un nuevo nodo
  TreeNode *new_node = createTreeNode(key, value);
  if (new_node == NULL)
    return;

  // Si el árbol está vacío, el nuevo nodo se convierte en la raíz
  if (tree->root == NULL) {
    tree->root = new_node;
    return;
  }

  // Comenzamos la búsqueda desde la raíz
  TreeNode *current = tree->root;
  TreeNode *parent = NULL;

  // Recorremos el árbol hasta encontrar el lugar adecuado para el nuevo nodo
  while (current != NULL) {
    parent = current;
    // Si la clave es menor que la clave del nodo actual, nos movemos a la
    // izquierda
    if (tree->lower_than(key, current->pair->key)) {
      current = current->left;
    }
    // Si la clave es mayor que la clave del nodo actual, nos movemos a la
    // derecha
    else {
      current = current->right;
    }
  }

  // Asignamos el padre del nuevo nodo
  new_node->parent = parent;

  // Si la clave es menor que la clave del padre, el nuevo nodo es el hijo
  // izquierdo
  if (tree->lower_than(key, parent->pair->key)) {
    parent->left = new_node;
  }
  // Si la clave es mayor que la clave del padre, el nuevo nodo es el hijo
  // derecho
  else {
    parent->right = new_node;
  }
}

TreeNode *minimum(TreeNode *x) { return NULL; }

void removeNode(TreeMap *tree, TreeNode *node) {}

void eraseTreeMap(TreeMap *tree, void *key) {
  if (tree == NULL || tree->root == NULL)
    return;

  if (searchTreeMap(tree, key) == NULL)
    return;
  TreeNode *node = tree->current;
  removeNode(tree, node);
}

Pair *searchTreeMap(TreeMap *tree, void *key) { return NULL; }

Pair *upperBound(TreeMap *tree, void *key) { return NULL; }

Pair *firstTreeMap(TreeMap *tree) { return NULL; }

Pair *nextTreeMap(TreeMap *tree) { return NULL; }
