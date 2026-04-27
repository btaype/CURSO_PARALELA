#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct nodo {
    int dato;
    struct nodo* sig;
} Nodo;

Nodo* head = NULL;
pthread_rwlock_t rwlock;

int Member(int x) {
    pthread_rwlock_rdlock(&rwlock);

    Nodo* act = head;
    while (act != NULL && act->dato < x)
        act = act->sig;

    int encontrado = (act != NULL && act->dato == x);

    pthread_rwlock_unlock(&rwlock);
    return encontrado;
}

int Insert(int x) {
    pthread_rwlock_wrlock(&rwlock);

    Nodo* act = head;
    Nodo* ant = NULL;

    while (act != NULL && act->dato < x) {
        ant = act;
        act = act->sig;
    }

    if (act != NULL && act->dato == x) {
        pthread_rwlock_unlock(&rwlock);
        return 0;
    }

    Nodo* nuevo = malloc(sizeof(Nodo));
    nuevo->dato = x;
    nuevo->sig = act;

    if (ant == NULL)
        head = nuevo;
    else
        ant->sig = nuevo;

    pthread_rwlock_unlock(&rwlock);
    return 1;
}

int Delete(int x) {
    pthread_rwlock_wrlock(&rwlock);

    Nodo* act = head;
    Nodo* ant = NULL;

    while (act != NULL && act->dato < x) {
        ant = act;
        act = act->sig;
    }

    if (act == NULL || act->dato != x) {
        pthread_rwlock_unlock(&rwlock);
        return 0;
    }

    if (ant == NULL)
        head = act->sig;
    else
        ant->sig = act->sig;

    free(act);

    pthread_rwlock_unlock(&rwlock);
    return 1;
}

void Print_list() {
    pthread_rwlock_rdlock(&rwlock);

    Nodo* act = head;
    printf("Lista: ");
    while (act != NULL) {
        printf("%d -> ", act->dato);
        act = act->sig;
    }
    printf("NULL\n");

    pthread_rwlock_unlock(&rwlock);
}

void* trabajo(void* arg) {
    Insert(10);
    Insert(5);
    Insert(20);

    Member(10);
    Delete(5);

    return NULL;
}

int main() {
    pthread_t h1, h2, h3;

    pthread_rwlock_init(&rwlock, NULL);

    pthread_create(&h1, NULL, trabajo, NULL);
    pthread_create(&h2, NULL, trabajo, NULL);
    pthread_create(&h3, NULL, trabajo, NULL);

    pthread_join(h1, NULL);
    pthread_join(h2, NULL);
    pthread_join(h3, NULL);

    Print_list();

    pthread_rwlock_destroy(&rwlock);

    return 0;
}
