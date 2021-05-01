#include <radix_tree.h>
#include <stdio.h>



int main(int argc,const char** argv){
	radix_tree_t t;
	radix_tree_init(&t);
	radix_tree_insert(&t,"romane");
	radix_tree_insert(&t,"romanus");
	radix_tree_insert(&t,"romulus");
	radix_tree_insert(&t,"rubens");
	radix_tree_insert(&t,"ruber");
	radix_tree_insert(&t,"rubicon");
	radix_tree_insert(&t,"rubicundus");
	radix_tree_print(&t);
	printf("%u\n",radix_tree_delete(&t,"ruber"));
	printf("%u\n",radix_tree_delete(&t,"invalid_key"));
	radix_tree_print(&t);
	return 0;
}
