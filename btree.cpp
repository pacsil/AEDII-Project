#include <stdlib.h>
#include "btree.h"
#include <stdio.h>

#define TRUE 1
#define FALSE 0

btNo* btCriarNo (int ordem) {
    btNo * no = (btNo *) malloc (sizeof(btNo));
    
    no->ehFolha = TRUE;
    no->numChaves = 0;
    no->chaves = (int  *) malloc (ordem * sizeof (int *));
    no->filhos = (btNo **) malloc ((ordem+1) * sizeof (btNo *));
    
    for (int i=0; i<ordem; i++) 
    	no->chaves[i] = 0;
    for (int i=0; i<=ordem; i++) 
    	no->filhos[i] = NULL;

    return no;    
}


bTree btCriar(const unsigned char ordem) {
    bTree b;
    b.ordem = ordem;
    b.raiz = btCriarNo (b.ordem);
    return b;
}


void btDestruirNo (btNo *no) {
    if (no == NULL) 
    	return;
    for (int i=0; i <= no->numChaves; i++) 
        btDestruirNo (no->filhos[i]);
    
    free (no->filhos);
    free (no->chaves);
    free (no);
}

void btDestruir(bTree b) {
   btDestruirNo (b.raiz); 
}

int buscarChave (int n, const int *a, int chave) {
    int lo = -1;
    int hi = n;
    int mid;

    while (lo+1 < hi) {
        mid = (lo+hi)/2;
        if (a[mid] == chave)	
			return mid;
        else if (a[mid] < chave) 
			lo = mid;
        else 	
			hi = mid;
    }
    return hi;
}

int btBuscar (btNo *b, int chave) {
    if (b->numChaves == 0)
        return 0;
        
    int pos = buscarChave (b->numChaves, b->chaves, chave);
    
    if (pos < b->numChaves && b->chaves[pos] == chave) 
        return 1;
    else
        return (!b->ehFolha && btBuscar (b->filhos[pos], chave));
}

int btBuscar(bTree b, int chave) {
	return btBuscar (b.raiz, chave);
}


btNo * inserirInterno (btNo *b, int chave, int *mediana, int ordem) {
	int pos;
	int mid;
	btNo *b2;

	pos = buscarChave (b->numChaves, b->chaves, chave);

	if ((pos < b->numChaves) && (b->chaves[pos] == chave)) // ja existe
		return NULL;
	
	if (b->ehFolha) {
		for (int i=b->numChaves; i > pos; i--)
			b->chaves[i] = b->chaves[i-1];
		b->chaves [pos] = chave; //mid
		b->numChaves++;
	}
	else {
	    b2 = inserirInterno (b->filhos[pos], chave, &mid,ordem);
	    if (b2) {
			for (int i=b->numChaves; i > pos; i--) {
				b->chaves[i] = b->chaves[i-1];
				b->filhos[i+1] = b->filhos[i];
			}
	        b->chaves[pos] = mid; //mid;
	        b->filhos[pos+1] = b2;
	        b->numChaves++;
	    }
	}

	if (b->numChaves >= ordem) {
	    mid = b->numChaves / 2;	
	    *mediana = b->chaves[mid];
	    b2 = btCriarNo (ordem);
	    b2->numChaves = b->numChaves - mid - 1;
	    b2->ehFolha = b->ehFolha;
	    
	    for (int i=mid+1; i < b->numChaves; i++)
	    	b2->chaves[i-(mid+1)] = b->chaves[i];
	    if (!b->ehFolha) 
	    	for (int i=mid+1; i < b->numChaves+1; i++)
	    		b2->filhos[i-(mid+1)] = b->filhos[i];
	    b->numChaves = mid;
	    return b2;
	} else {
	    return NULL;
	}
}

void btInserir(bTree b, int chave) {
    btNo *b1;
    btNo *b2;
    int mediana;
    
    b2 = inserirInterno (b.raiz, chave, &mediana, b.ordem);
    
    if (b2) {
    	b1 = btCriarNo(b.ordem);
    	for (int i=0; i<b.ordem; i++) {
    		b1->chaves[i] = b.raiz->chaves[i];
    		b1->filhos[i] = b.raiz->filhos[i];
		} b1->filhos[b.ordem] = b.raiz->filhos[b.ordem];
    	b1->ehFolha = b.raiz->ehFolha;      b.raiz->ehFolha = FALSE;
    	b1->numChaves = b.raiz->numChaves; 	b.raiz->numChaves = 1;
    	
    	b.raiz->chaves[0] = mediana;
    	b.raiz->filhos[0] = b1;
    	b.raiz->filhos[1] = b2;
	}
}

int btAltura(bTree b) {
    btNo *no;
    int altura = 1;
    for (no = b.raiz; !no->ehFolha; no = no->filhos[0])
    	altura++;
    return altura;
}

int btContaNo (btNo *no) {
    if (no == NULL) 
        return 0;
    int n = 1;
    for (int i=0; i <= no->numChaves; i++) 
        n += btContaNo (no->filhos[i]);
    return n;
}

int btContaNos(bTree b) {
    return btContaNo (b.raiz);
}

int btContaChave (btNo *no) {
    if (no == NULL)
        return 0;
    int n = 0;
    for (int i=0; i <= no->numChaves; i++) 
        n += btContaChave (no->filhos[i]);
    n+=no->numChaves;

    return n;
}

int btContaChaves(bTree t) {
    return btContaChave (t.raiz);
}


void shownode (btNo *no, int level) {
	printf ("%x level %d [", no, level);
	
	for (int i=0; i<no->numChaves; i++) {
		if (i < no->numChaves-1)
			printf ("%d->%x,",no->chaves[i],no->filhos[i]);
		else
			printf ("%d->%x",no->chaves[i],no->filhos[i]);
	}
	printf ("]");
	
	printf ("\n");
	
	for (int i=0; i<no->numChaves+1; i++) {
		if (no->filhos[i] != NULL)
			shownode (no->filhos[i], level+1);
	}
}

void showtree (bTree b) {
	shownode (b.raiz, 0);
}

/* 
int main(int argc, char** argv) {
	
	printf ("iniciando\n");
	bTree b = btCriar(3);
	
	btInserir (b,1);
  	btInserir (b,2);
  	btInserir (b,3);
	btInserir (b,4);
	btInserir (b,5);
	btInserir (b,6);
	btInserir (b,7);
	btInserir (b,8);
	btInserir (b,9);
	btInserir (b,10);
	btInserir (b,11);
	btInserir (b,12);
	showtree (b);
	printf ("Altura=%d\n",btAltura(b));
	printf ("Nos=%d\n",btContaNos(b));

printf ("Chaves=%d\n",btContaChaves(b));
	

	return 0;
}*/	
