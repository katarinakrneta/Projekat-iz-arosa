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

    // Definicija i inicijalizacija semafora
    semaforPosiljaoca = sem_open("/chat_posiljaoc_semafor", O_CREAT, 0644, 1);
    semaforPrimaoca = sem_open("/chat_primaoc_semafor", O_CREAT, 0644, 1);
    if (semaforPosiljaoca == SEM_FAILED || semaforPrimaoca == SEM_FAILED) {
        perror("Greška pri inicijalizaciji semafora");
        exit(EXIT_FAILURE);
    }

    // Kreiranje imenovane cevi (FIFO)
    mkfifo(imeCevi, 0644);

    // Otvaranje cevi za pisanje
    int cev_fd = open(imeCevi, O_WRONLY);
    if (cev_fd == -1) {
        perror("Greška pri otvaranju cevi za pisanje");
        exit(EXIT_FAILURE);
    }

    char poruka[MAKS_VELICINA_PORUKE];

    while (1) {
        // Unos korisnika
        printf("Unesite poruku: ");
        fgets(poruka, MAKS_VELICINA_PORUKE, stdin);

        // Čekanje na semafor pošiljaoca
        sem_wait(semaforPosiljaoca);

        // Pisanje poruke u cijev
        write(cev_fd, poruka, strlen(poruka) + 1);

        // Oslobađanje semafora primaoca
        sem_post(semaforPrimaoca);
    }

    close(cev_fd);
    sem_close(semaforPosiljaoca);
    sem_close(semaforPrimaoca);
    return 0;
}
