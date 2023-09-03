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
    const char *imeCevi = "/tmp/chat_cev";
    sem_t *semaforPosiljaoca, *semaforPrimaoca;

    // Otvaranje imenovane cevi
    int cev_fd = open(imeCevi, O_RDONLY);
    if (cev_fd == -1) {
        perror("Greška pri otvaranju cevi za čitanje");
        exit(EXIT_FAILURE);
    }

    // Inicijalizacija semafora
    semaforPosiljaoca = sem_open("/chat_posiljaoc_semafor", O_CREAT, 0644, 1);
    semaforPrimaoca = sem_open("/chat_primaoc_semafor", O_CREAT, 0644, 1);
    if (semaforPosiljaoca == SEM_FAILED || semaforPrimaoca == SEM_FAILED) {
        perror("Greška pri inicijalizaciji semafora");
        exit(EXIT_FAILURE);
    }

    char primljenaPoruka[MAKS_VELICINA_PORUKE];

    while (1) {
        // Čekanje na semafor primaoca
        sem_wait(semaforPrimaoca);

        // Čitanje poruke iz cevi
        ssize_t procitanoBajtova = read(cev_fd, primljenaPoruka, MAKS_VELICINA_PORUKE);
        if (procitanoBajtova > 0) {
            printf("Primljena poruka: %s", primljenaPoruka);
        }

        // Oslobađanje semafora pošiljaoca
        sem_post(semaforPosiljaoca);
    }

    close(cev_fd);
    sem_close(semaforPosiljaoca);
    sem_close(semaforPrimaoca);
    return 0;
}
