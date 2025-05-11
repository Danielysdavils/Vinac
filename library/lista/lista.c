#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

struct lista_t *lista_cria ()
{
	struct lista_t *lst = malloc(sizeof(struct lista_t));
	if (!lst)
		return NULL;

	lst->prim = NULL;
	lst->ult = NULL;
	lst->tamanho = 0;

	return lst;
}

struct lista_t *lista_destroi (struct lista_t *lst)
{
	if(!lst)
		return NULL;

	int valor;
	while(lst->tamanho > 0)
	{
		int status = lista_retira (lst, &valor, 0);
		if(status < 0)
			return NULL;
	}

	free(lst);
	return NULL;
}

int lista_insere (struct lista_t *lst, int item, int pos)
{
	if(!lst)
		return -1;

	struct item_t *itemN = malloc(sizeof(struct item_t));
	
    if(!itemN)
		return -1;

	//item a inserir
	itemN->valor = item;
	itemN->ant = NULL;
	itemN->prox = NULL;

	//insere numa posição maior do tam da lista ou numa pos invalida
	if(pos == -1 || pos > lst->tamanho - 1)
	{
		//primeiro elemento da lst
		if(lst->tamanho == 0)
		{
			lst->prim = itemN;
			lst->ult = itemN;
		}else
		{
			itemN->ant = lst->ult;
			lst->ult->prox = itemN;
			lst->ult = itemN;
		}

	//insere na pos 0
	}else if (pos == 0)
	{
		itemN->prox = lst->prim;
		if(lst->prim != NULL)
			lst->prim->ant = itemN;
		lst->prim = itemN;
	}
	else
	//insere no meio
	{
		struct item_t *aux = lst->prim;
		for(int i = 0; i < pos - 1 && aux != NULL; i++)
			aux = aux->prox;

		itemN->prox = aux->prox;
		itemN->ant = aux;
		if(aux->prox != NULL)
			aux->prox->ant = itemN;

		aux->prox = itemN;
	}

	lst->tamanho++;
	return lst->tamanho;
}

int lista_retira (struct lista_t *lst, int *item, int pos)
{
	if(!lst || lst->tamanho == 0)
		return -1;

	struct item_t *aux;

	//insere no final caso pos invalida
	if (pos == -1)
	{
		aux = lst->ult;
		*item = aux->valor;

		if(lst->ult->ant != NULL)
		{
			lst->ult = lst->ult->ant;
			lst->ult->prox = NULL;
		}else
			lst->prim = lst->ult = NULL; //tinha apenas 1 elemento

	//remove no inicio da lst
	} else if (pos == 0)
	{
		aux = lst->prim;
		*item = aux->valor;

		if(lst->prim->prox != NULL)
		{
			lst->prim = lst->prim->prox;
			lst->prim->ant = NULL;

		}else
			lst->prim = lst->ult = NULL;

	}else
	//remove no meio
	{
		aux = lst->prim;
		for (int i = 0; i < pos && aux != NULL; i++)
			aux = aux->prox;

		*item = aux->valor;

		if(aux == NULL)
			return -1;

		if(aux->ant != NULL)
			aux->ant->prox = aux->prox;

		if(aux->prox != NULL)
			aux->prox->ant = aux->ant;
	}

	free(aux);
	lst->tamanho--;

	return lst->tamanho;
}

int lista_consulta (struct lista_t *lst, int *item, int pos)
{
	if(!lst || lst->tamanho == 0)
		return -1;

	//consulta do valor final
	if(pos == -1)
	{
		if(lst->ult != NULL)
		{
			*item = lst->ult->valor;
			return lst->tamanho;
		}else
			return -1;
	}

	struct item_t *aux = lst->prim;
	for (int i = 0; i <= pos - 1 && aux != NULL; i++)
		aux = aux->prox;

	if(aux != NULL)
	{
		*item = aux->valor;
		return lst->tamanho;
	}

	return -1;
}

int lista_procura (struct lista_t *lst, int valor)
{
	if(!lst || lst->tamanho <= 0)
		return -1;

	struct item_t *aux = lst->prim;
	for (int i = 0; i <= lst->tamanho - 1; i++)
	{
		if (aux->valor == valor)
			return i;

		aux = aux->prox;
	}

	return -1;
}

int lista_tamanho (struct lista_t *lst)
{
	return lst == NULL ? -1 : lst->tamanho;
}

void lista_imprime (struct lista_t *lst)
{
	struct item_t *aux = lst->prim;
	for(int i = 0; i <= lst->tamanho - 1; i++)
	{
		printf("%d", aux->valor);

		if(i != lst->tamanho - 1)
			printf(" ");

		aux = aux->prox;
	}
}



