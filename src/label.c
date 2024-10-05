#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>

#include "ast.h"
#include "label.h"

#define INITIAL_CAPACITY 10

typedef struct labelList_s
{
  label_t *labels; /* Dynamic array of labels */
  int size;      /* Current number of labels */
  int capacity;  /* Maximum number of labels before resizing */
} labelList_t;

static void freeLabelList(labelList_t *list);
static label_t *findLabel(labelList_t *list, const char *name);
static void addLabel(labelList_t *list, const char *name, int address);
static void resizeLabelList(labelList_t *list);
static labelList_t *createLabelList();

static labelList_t* root_list = NULL;

labelList_t *createLabelList()
{
  labelList_t *list = (labelList_t *)malloc(sizeof(labelList_t));
  list->labels = (label_t *)malloc(INITIAL_CAPACITY * sizeof(label_t));
  list->size = 0;
  list->capacity = INITIAL_CAPACITY;
  return list;
}

/* double the capacity */
void resizeLabelList(labelList_t *list)
{
  list->capacity *= 2;
  list->labels = (label_t *)realloc(list->labels, list->capacity * sizeof(label_t));
}

/* add name and address to the list */
void addLabel(labelList_t *list, const char *name, int address)
{
  /* Resize if necessary */
  if (list->size >= list->capacity)
  {
    resizeLabelList(list);
  }

  /* Create a new label and add it to the list */
  list->labels[list->size].name = strdup(name);
  list->labels[list->size].address = address;
  list->size++;
}

/* performs a linear search */
label_t *findLabel(labelList_t *list, const char *name)
{
  for (int i = 0; i < list->size; i++)
  {
    if (strcmp(list->labels[i].name, name) == 0)
    {
      return &list->labels[i]; /* Return pointer to the label */
    }
  }
  return NULL; /* label_t not found */
}

/* releases the allocated memory */
void freeLabelList(labelList_t *list)
{
  for (int i = 0; i < list->size; i++)
  {
    free(list->labels[i].name); /* Free each label's name */
  }
  free(list->labels); /* Free the label array */
  free(list);         /* Free the list structure itself */
}

/* add new label (no duplication check) */
int label_add(const char *lbl, unsigned int pc)
{
  addLabel(root_list, lbl, pc);
  return pc;
}

/* presence checker */
int label_exist(const char *lbl)
{
  label_t *obj = findLabel(root_list, lbl);
  return  (!!obj);
}

/* retrieve the address associated with the label */
_Bool label_getpc(unsigned int *addr, const char *lbl)
{
  label_t *obj = findLabel(root_list, lbl);
  if (addr)
    *addr = (obj) ? obj->address : 0;
  return !!obj;
}

static void destructor(void)
{
  extern int debug;
    int ret = label_deinit();
  if (debug)
    fprintf(stderr, "label_deinit() return %d\n", ret);
}

/* list's constructor */
int label_init(void)
{
  int ret = !!root_list; /*  Set to 1 if root_list is already allocated, 0 if it's unused (NULL) */
  root_list = createLabelList();
  atexit(destructor);
  return !root_list || ret;
}

/* list's destructor */
int label_deinit(void)
{
  int ret = !root_list;
  if (root_list) 
  {
    freeLabelList(root_list);
    root_list = NULL;
  }
  return ret;
}

#ifdef UNITTEST
struct unittest_s unittest =
{
  .freeLabelList = freeLabelList,
  .findLabel = findLabel,
  .addLabel = addLabel,
  .resizeLabelList = resizeLabelList,
  .createLabelList = createLabelList,

  .root_list = &root_list,
};
#endif
