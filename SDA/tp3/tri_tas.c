#include "binary_tree.h"
#include "BT.h"
#include "item.h"
#include "queue_link.h"
#include "stack_link.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void fix_up_heap(item tab[], int k){
    while(k>1 && less(tab[k/2],tab[k])){
        exch(tab[k], tab[k/2]);
        k=k/2;
    }
}

void fix_down_heap(item tab[], int k, int n){
    int j;
    while(2*k <= n){
        j=2*k;
        if(j<n && less(tab[j],tab[j+1])) j++;
        if(!less(tab[k],tab[j])) break;
        exch(tab[k], tab[j]);
        k=j;
    }
}

void heap_sort(item tab[], int l, int r){
    int k,n=r-l+1;
    item *pq = tab+l-1;
    for(k=2; k<=n; ++k){
        fix_up_heap(pq,k);
    }
    k=n;
    while(k>1){
        exch(tab[l], tab[l-1+k]);
        fix_down_heap(pq,l,--k);
    }
}