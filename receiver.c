#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <semaphore.h>

#define MAKS_VELICINA_PORUKE 2000

int main() {
    const char *imeCijevi = "/tmp/chat_cijev";
    sem_t *semforPosiljaoca, *semforPrimaoca;

    // Otvaranje imenovane cijevi
    int cijev_fd = open(imeCijevi, O_RDONLY);
    if (cijev_fd == -1) {
        perror("Greška pri otvaranju cijevi za čitanje");
        exit(EXIT_FAILURE);
    }

    // Inicijalizacija semafora
    semforPosiljaoca = sem_open("/chat_posiljaoc_semfor", O_CREAT, 0644, 1);
    semforPrimaoca = sem_open("/chat_primaoc_semfor", O_CREAT, 0644, 1);
    if (semforPosiljaoca == SEM_FAILED || semforPrimaoca == SEM_FAILED) {
        perror("Greška pri inicijalizaciji semafora");
        exit(EXIT_FAILURE);
    }

    char primljenaPoruka[MAKS_VELICINA_PORUKE];

    while (1) {
        // Čekanje na semafor primaoca
        sem_wait(semforPrimaoca);

        // Čitanje poruke iz cijevi
        ssize_t procitanoBajtova = read(cijev_fd, primljenaPoruka, MAKS_VELICINA_PORUKE);
        if (procitanoBajtova > 0) {
            printf("Primljena poruka: %s", primljenaPoruka);
        }

        // Oslobađanje semafora pošiljaoca
        sem_post(semforPosiljaoca);
    }

    close(cijev_fd);
    sem_close(semforPosiljaoca);
    sem_close(semforPrimaoca);
    return 0;
}