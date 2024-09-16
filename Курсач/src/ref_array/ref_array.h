#ifndef REF_ARRAY_H
#define REF_ARRAY_H

struct RefArray;

typedef struct RefArray RefArray;
typedef RefArray* RefArrayPtr;

struct RefArray
{
	void** data;
	int size;
	int capacity;
};

RefArray init_ref_array(int reserve);
void add_ref(RefArrayPtr array, void* ref);
void clear_array(RefArrayPtr array);

void* get_ref(RefArrayPtr array, int pos);

#endif // REF_ARRAY_H