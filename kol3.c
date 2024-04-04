#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#define BUFFER_SIZE 50

typedef struct {
    char wlasciciel[BUFFER_SIZE];
    int data[3];
    enum Typ {car, bus, truck} typ;
    enum Naped {electric, hybrid, combustion} naped;
    union Spec {
        struct Truck {
            float pojemnosc;
            float nacisk;
        } truck;

        struct Car {
            int maxi;
            float moc;
        } car;

        struct Bus {
            int siedzace;
            float stojace;
        } bus;
    } spec;
} Vehicle;

void new_owner(Vehicle *pvehicle) {
    char buf[BUFFER_SIZE];
    fgets(buf, BUFFER_SIZE, stdin)
    memset(pvehicle->wlasciciel, 0, BUFFER_SIZE);
    strcpy(pvehicle->wlasciciel, buf);
}

int delayed(Vehicle *pvehicle, int size, int *base_date, char ***pdelayed_owners) {
    int sum = 0;
    for (int i=0; i<size; i++) {
        if (cmp(pvehicle[i]->data, base_date)<0)
            sum++;
    }
    pdelayed_owners = (char ***)calloc(sum, sizeof(char*));
    int own = 0;
    for (int i=0; i<size; i++) {
        if (cmp(pvehicle[i]->data, base_date)<0) {
            pdelayed_owners[own] = pvehicle->wlasciciel;
            own++;
        }
    }
    return sum;
}

int cmp(int d1, int d2) {
    return (d1[0]-d2[0])*365+(d1[1]-d2[1])*31+(d1[2]-d2[2]);
}

void print_pojazd(Vehicle *pvehicle) {
    printf("%s/n%d %d %d/n%d/n%d/n%d", pvehicle->wlasciciel, pvehicle->data[0], pvehicle->data[1], pvehicle->data[2],
           pvehicle->typ, pvehicle->naped, pvehicle->spec)
}

int main (void) {

}