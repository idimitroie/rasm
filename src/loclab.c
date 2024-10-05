extern int debug;
#include <stdio.h>
#include <stdlib.h>

#include "loclab.h"
/* Node structure for the inner tree (address BST) */
typedef struct AddressNode
{
  unsigned int address;
  struct AddressNode *left;
  struct AddressNode *right;
} AddressNode;

/* Node structure for the outer tree (name BST) */
typedef struct NameNode
{
  unsigned int name;
  AddressNode *addresses; /* Root of the address BST for this name */
  struct NameNode *left;
  struct NameNode *right;
} NameNode;

/* The root of the outer tree (name BST) */
static AddressNode *find_b(unsigned int name, unsigned int adr);
static AddressNode *find_f(unsigned int name, unsigned int adr);
static void freeAddressTree(AddressNode *root);
static void freeNameTree(NameNode *root);
static AddressNode *insertAddress(AddressNode *root, unsigned int adr);
static NameNode *insertName(NameNode *root, unsigned int name, unsigned int adr);
/* Find Closest Address (Helper Functions) Find Closest Address Smaller or Equal */
static AddressNode *findClosestSmallerOrEqual(AddressNode *root, unsigned int adr, AddressNode *bestMatch);
/* Find Closest Address Greater or Equal */
static AddressNode *findClosestGreater(AddressNode *root, unsigned int adr, AddressNode *bestMatch);

/* Callback function signature */
typedef void (*callback_t)(unsigned int name, unsigned int address);

#if defined(TRAVERSE)
static void traverseAddressTree(AddressNode *root, unsigned int name, callback_t callback);
static void traverseNameTree(NameNode *root, callback_t callback);
static void traverse(callback_t callback);
#endif
/* The root of the outer tree (name BST) */
static NameNode *nameTreeRoot = NULL;

/* BST Helper Functions */

/* Inserting into the AddressNode (Inner BST) */
AddressNode *insertAddress(AddressNode *root, unsigned int adr)
{
  if (root == NULL)
  {
    AddressNode *newNode = (AddressNode *)malloc(sizeof(AddressNode));
    newNode->address = adr;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
  }
  if (adr < root->address)
    root->left = insertAddress(root->left, adr);
  else
    root->right = insertAddress(root->right, adr);
  return root;
}

NameNode *insertName(NameNode *root, unsigned int name, unsigned int adr)
{
  if (root == NULL)
  {
    NameNode *newNode = (NameNode *)malloc(sizeof(NameNode));
    newNode->name = name;
    newNode->addresses = insertAddress(NULL, adr); /* Create new address tree for this name */
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
  }
  if (name < root->name)
    root->left = insertName(root->left, name, adr);
  else if (name > root->name)
    root->right = insertName(root->right, name, adr);
  else
    root->addresses = insertAddress(root->addresses, adr); /* Insert address if name already exists */
  return root;
}

/* Find Closest Address (Helper Functions) */

/* Find Closest Address Smaller or Equal */
AddressNode *findClosestSmallerOrEqual(AddressNode *root, unsigned int adr, AddressNode *bestMatch)
{
  if (root == NULL)
    return bestMatch;

  if (root->address <= adr)
  {
    bestMatch = root;
    return findClosestSmallerOrEqual(root->right, adr, bestMatch); /* Go right to find closer match */
  }
  else
  {
    return findClosestSmallerOrEqual(root->left, adr, bestMatch); /* Go left to find smaller match */
  }
}

/* Find Closest Address Greater than */
AddressNode *findClosestGreater(AddressNode *root, unsigned int adr, AddressNode *bestMatch)
{
  if (root == NULL)
    return bestMatch;

  if (root->address > adr)
  {
    bestMatch = root;
    return findClosestGreater(root->left, adr, bestMatch); /* Go left to find closer match */
  }
  else
  {
    return findClosestGreater(root->right, adr, bestMatch); /* Go right to find larger match */
  }
}

void loclab_init()
{
  if (debug)
  {
    fprintf(stderr,"init: %p\n", nameTreeRoot);
    fflush(stderr);
  }
  nameTreeRoot = NULL;
  atexit(loclab_deinit);
}

void loclab_add(unsigned int name, unsigned int adr)
{
  if (debug)
  {
    fprintf(stderr,"add: %u, %u\n", name, adr);
    fflush(stdout);
  }
  nameTreeRoot = insertName(nameTreeRoot, name, adr);
  if (debug)
  {
    fprintf(stderr,"nameTreeRoot: %p\n", nameTreeRoot);
    fflush(stderr);
  }
}

AddressNode *find_b(unsigned int name, unsigned int adr)
{
  NameNode *current = nameTreeRoot;

  /* Search for the name node */
  while (current != NULL)
  {
    if (name < current->name)
      current = current->left;
    else if (name > current->name)
      current = current->right;
    else
      break;
  }

  if (current == NULL)
  {
    return NULL; /* Name not found */
  }

  /* Find closest address in the address tree */
  return findClosestSmallerOrEqual(current->addresses, adr, NULL);
}

AddressNode *find_f(unsigned int name, unsigned int adr)
{
  NameNode *current = nameTreeRoot;

  /* Search for the name node */
  while (current != NULL)
  {
    if (name < current->name)
      current = current->left;
    else if (name > current->name)
      current = current->right;
    else
      break;
  }

  if (current == NULL)
  {
    return NULL; /* Name not found */
  }

  /* Find closest address in the address tree */
  return findClosestGreater(current->addresses, adr, NULL);
}

_Bool loclab_getpc(unsigned *addr, int name, unsigned pc)
{
  AddressNode *n = NULL;
  if (debug)
  {
    fprintf(stderr,"getpc: %d, %u\n", name, pc);
    fflush(stdout);
  }
  if (name < 0)
    n = find_b(-name, pc);
  else
    n = find_f(name, pc);
  if (addr)
  {
    *addr = (n) ? n->address : 0;
  }
  return !!n;
}

void freeAddressTree(AddressNode *root)
{
  if (root == NULL)
  {
    return;
  }

  /* Recursively free the left and right subtrees */
  freeAddressTree(root->left);
  freeAddressTree(root->right);

  /* Free the current node */
  free(root);
}

void freeNameTree(NameNode *root)
{
  if (root == NULL)
  {
    return;
  }

  /* Recursively free the left and right subtrees */
  freeNameTree(root->left);
  freeNameTree(root->right);

  /* Free the associated address tree */
  freeAddressTree(root->addresses);

  /* Free the current name node */
  free(root);
}

void loclab_deinit()
{
  if (debug)
    fprintf(stderr, "loclab_deinit(): free %p\n", nameTreeRoot);
  /* Free the name tree, which will also free all associated address trees */
  if (nameTreeRoot)
  {
    freeNameTree(nameTreeRoot);
    /* Set the root pointer to NULL after freeing everything */
    nameTreeRoot = NULL;
  }
}

#if defined(TRAVERSE)
void traverseAddressTree(AddressNode *root, unsigned int name, callback_t callback)
{
  if (root == NULL)
  {
    return;
  }

  /* In-order traversal: left -> node -> right */
  traverseAddressTree(root->left, name, callback);
  callback(name, root->address); /* Call the callback for each address */
  traverseAddressTree(root->right, name, callback);
}

void traverseNameTree(NameNode *root, callback_t callback)
{
  if (root == NULL)
  {
    return;
  }

  /* In-order traversal: left -> node -> right */
  traverseNameTree(root->left, callback);
  traverseAddressTree(root->addresses, root->name, callback); /* Traverse the associated address tree */
  traverseNameTree(root->right, callback);
}

void traverse(callback_t callback)
{
  /* Traverse the name tree, passing the callback function */
  traverseNameTree(nameTreeRoot, callback);
}
#endif
