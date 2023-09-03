# Aros-projekat
Katarina Krneta

Iz predmeta Arhitektura Računara i Operativni Sistemi uradila sam projekat koji se odnosi se na izradu aplikacije za međusobnu razmenu tekstualnih poruka između različitih procesa koristeći named pipe i semafore.

Kako da pokrenete program:

    Otvorite dva terminala.

    U terminalima se premestite u direktorijum gde se nalaze programski kodovi korišćenjem komande cd.

    Kompajlirajte izvorne kodove koristeći sledeće komande:
        gcc sender.c -o sender -lpthread
        gcc receiver.c -o receiver -lpthread

    Pokrenite programe u terminalima pomoću sledećih komandi:
        ./sender
        ./receiver
