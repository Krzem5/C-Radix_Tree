#include <radix_tree.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>



void _radix_tree_print_node(radix_tree_node_t* n,uint32_t i){
	for (uint32_t j=0;j<i;j++){
		putchar(' ');
	}
	char* nm=n->v;
	while (*nm){
		putchar(*nm);
		nm++;
	}
	if (n->nll){
		putchar(':');
	}
	if (n->id!=UINT32_MAX){
		printf(" (%u)\n",n->id);
	}
	else{
		putchar('\n');
	}
	for (uint32_t j=0;j<n->nll;j++){
		_radix_tree_print_node(n->nl+j,i+n->vl);
	}
}



void radix_tree_init(radix_tree_t* t){
	t->v=malloc(sizeof(char));
	*(t->v)=0;
	t->vl=0;
	t->id=UINT32_MAX;
	t->nl=NULL;
	t->nll=0;
	t->_nid=0;
}



void radix_tree_insert(radix_tree_t* t,char* v){
	radix_tree_node_t* r=(radix_tree_node_t*)t;
_nxt:
	uint32_t ml=0;
	radix_tree_node_t* nn=NULL;
	for (uint32_t i=0;i<r->nll;i++){
		radix_tree_node_t* e=r->nl+i;
		uint32_t cml=0;
		for (uint32_t j=0;j<e->vl;j++){
			if (!(*(v+j))||*(v+j)!=*(e->v+j)){
				break;
			}
			cml++;
		}
		if (cml==e->vl){
			v+=e->vl;
			if (!(*v)){
				e->id=t->_nid;
				t->_nid++;
				return;
			}
			r=e;
			goto _nxt;
		}
		if (cml>ml){
			ml=cml;
			nn=e;
		}
	}
	if (!ml){
		r->nll++;
		r->nl=realloc(r->nl,r->nll*sizeof(radix_tree_node_t));
		uint32_t sz=0;
		while (*(v+sz)){
			sz++;
		}
		(r->nl+r->nll-1)->v=malloc((sz+1)*sizeof(char));
		(r->nl+r->nll-1)->vl=sz;
		*((r->nl+r->nll-1)->v+sz)=0;
		while (sz){
			sz--;
			*((r->nl+r->nll-1)->v+sz)=*(v+sz);
		}
		(r->nl+r->nll-1)->id=t->_nid;
		(r->nl+r->nll-1)->nl=NULL;
		(r->nl+r->nll-1)->nll=0;
	}
	else{
		v+=ml;
		if (!(nn->nll)){
			nn->nll+=2;
			nn->nl=realloc(nn->nl,nn->nll*sizeof(radix_tree_node_t));
			(nn->nl+nn->nll-2)->v=malloc((nn->vl-ml+1)*sizeof(char));
			(nn->nl+nn->nll-2)->vl=nn->vl-ml;
			*((nn->nl+nn->nll-2)->v+nn->vl-ml)=0;
			for (uint32_t i=0;i<nn->vl-ml;i++){
				*((nn->nl+nn->nll-2)->v+i)=*(nn->v+ml+i);
			}
			(nn->nl+nn->nll-2)->id=nn->id;
			(nn->nl+nn->nll-2)->nl=NULL;
			(nn->nl+nn->nll-2)->nll=0;
			uint32_t sz=0;
			while (*(v+sz)){
				sz++;
			}
			(nn->nl+nn->nll-1)->v=malloc((sz+1)*sizeof(char));
			(nn->nl+nn->nll-1)->vl=sz;
			*((nn->nl+nn->nll-1)->v+sz)=0;
			for (uint32_t i=0;i<sz;i++){
				*((nn->nl+nn->nll-1)->v+i)=*(v+i);
			}
			(nn->nl+nn->nll-1)->id=t->_nid;
			(nn->nl+nn->nll-1)->nl=NULL;
			(nn->nl+nn->nll-1)->nll=0;
		}
		else{
			radix_tree_node_t* n_nl=malloc(2*sizeof(radix_tree_node_t));
			n_nl->v=malloc((nn->vl-ml+1)*sizeof(char));
			n_nl->vl=nn->vl-ml;
			*(n_nl->v+nn->vl-ml)=0;
			for (uint32_t i=0;i<nn->vl-ml;i++){
				*(n_nl->v+i)=*(nn->v+ml+i);
			}
			n_nl->id=nn->id;
			n_nl->nl=nn->nl;
			n_nl->nll=nn->nll;
			uint32_t sz=0;
			while (*(v+sz)){
				sz++;
			}
			(n_nl+1)->v=malloc((sz+1)*sizeof(char));
			(n_nl+1)->vl=sz;
			*((n_nl+1)->v+sz)=0;
			for (uint32_t i=0;i<sz;i++){
				*((n_nl+1)->v+i)=*(v+i);
			}
			(n_nl+1)->id=t->_nid;
			(n_nl+1)->nl=NULL;
			(n_nl+1)->nll=0;
			nn->nl=n_nl;
			nn->nll=2;
		}
		nn->v=realloc(nn->v,(ml+1)*sizeof(char));
		nn->vl=ml;
		*(nn->v+ml)=0;
		nn->id=UINT32_MAX;
	}
	t->_nid++;
}



uint8_t radix_tree_delete(radix_tree_t* t,char* v){
	radix_tree_node_t* r=(radix_tree_node_t*)t;
	radix_tree_node_t** l=malloc(sizeof(radix_tree_node_t*));
	*l=r;
	uint32_t ll=1;
_nxt:
	uint32_t ml=0;
	radix_tree_node_t* nn=NULL;
	for (uint32_t i=0;i<r->nll;i++){
		radix_tree_node_t* e=r->nl+i;
		uint32_t cml=0;
		for (uint32_t j=0;j<e->vl;j++){
			if (!(*(v+j))||*(v+j)!=*(e->v+j)){
				break;
			}
			cml++;
		}
		if (cml==e->vl){
			v+=e->vl;
			ll++;
			l=realloc(l,ll*sizeof(radix_tree_node_t*));
			*(l+ll-1)=e;
			if (!(*v)){
				while (ll){
					ll--;
					radix_tree_node_t* k=*(l+ll);
					if (k->nll){
						for (uint32_t j=0;j<k->nll;j++){
							radix_tree_node_t* e=k->nl+j;
							if (e==*(l+ll+1)){
								while (j<k->nll){
									*(k->nl+j)=*(k->nl+j+1);
									j++;
								}
								k->nll--;
								k->nl=realloc(k->nl,k->nll*sizeof(radix_tree_node_t));
								break;
							}
						}
						if (!k->nll){
							continue;
						}
						if (k->nll==1&&k->id==UINT32_MAX){
							radix_tree_node_t* e=k->nl;
							k->v=realloc(k->v,(k->vl+e->vl+1)*sizeof(char));
							for (uint32_t j=0;j<e->vl;j++){
								*(k->v+k->vl+j)=*(e->v+j);
							}
							free(e->v);
							k->vl+=e->vl;
							*(k->v+k->vl)=0;
							k->id=e->id;
							k->nl=e->nl;
							k->nll=e->nll;
							free(k->nl);
							break;
						}
						k->id=UINT32_MAX;
						break;
					}
				}
				free(l);
				return 1;
			}
			r=e;
			goto _nxt;
		}
	}
	free(l);
	return 0;
}



void radix_tree_print(radix_tree_t* t){
	for (uint32_t i=0;i<t->nll;i++){
		_radix_tree_print_node(t->nl+i,0);
	}
}
