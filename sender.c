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

    // Definicija i inicijalizacija semafora
    semforPosiljaoca = sem_open("/chat_posiljaoc_semfor", O_CREAT, 0644, 1);
    semforPrimaoca = sem_open("/chat_primaoc_semfor", O_CREAT, 0644, 1);
    if (semforPosiljaoca == SEM_FAILED || semforPrimaoca == SEM_FAILED) {
        perror("Greška pri inicijalizaciji semafora");
        exit(EXIT_FAILURE);
    }

    // Kreiranje imenovane cijevi (FIFO)
    mkfifo(imeCijevi, 0644);

    // Otvaranje cijevi za pisanje
    int cijev_fd = open(imeCijevi, O_WRONLY);
    if (cijev_fd == -1) {
        perror("Greška pri otvaranju cijevi za pisanje");
        exit(EXIT_FAILURE);
    }

    char poruka[MAKS_VELICINA_PORUKE];

    while (1) {
        // Unos korisnika
        printf("Unesite poruku: ");
        fgets(poruka, MAKS_VELICINA_PORUKE, stdin);

        // Čekanje na semafor pošiljaoca
        sem_wait(semforPosiljaoca);

        // Pisanje poruke u cijev
        write(cijev_fd, poruka, strlen(poruka) + 1);

        // Oslobađanje semafora primaoca
        sem_post(semforPrimaoca);
    }

    close(cijev_fd);
    sem_close(semforPosiljaoca);
    sem_close(semforPrimaoca);
    return 0;
}