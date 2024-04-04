#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

enum Vehicle_type {truck, car, v_ndef};

typedef struct {
    float load;
    short axes;
    int trailer_mass;
} Truck_data;

typedef struct {
    char vehicle_ID[15];
    int places;
    int driving_wheels;
    int towbar;
} Car_data;

typedef struct {int d; int m; int y;} Date;

typedef struct {
    char owner[30];
    Date valid_date;
    enum Vehicle_type v_type;
    union {
        Car_data cs;
        Truck_data ts;
    } veh_union;
} Vehicle;

int cmp_date(Date d1, Date d2) {
	if (d1.y != d2.y)
        return d1.y - d2.y;
    if (d1.m != d2.m)
        return d1.m - d2.m;
    return d1.d - d2.d;
}

int delayed(const Vehicle *tv, int size, const Date *base_date, char ***pdelayed_owners) {
    int sum = 0;
    Date base = *(Date *)base_date;
    for (int i=0; i<size; i++) {
        if (cmp_date(tv[i]->valid_date, base)<0)
            sum++;
    }
    if (sum==0) {
        pdelayed_owners = NULL;
        return 0;
    }
    pdelayed_owners = (char ***)calloc(sum, sizeof(char*));
    int own = 0;
    for (int i=0; i<size; i++) {
        if (cmp_date(tv[i]->valid_date, base)<0) {
            char *owner = tv[i]->owner;
            *pdelayed_owners[own] = owner;
            own++;
        }
    }
    return sum;
}

void print_owners(const char **t_owners, size_t size) {
    for (size_t i=0; i<size; i++)
        printf("%s ", t_owners[i]);
}

Vehicle *create_vehicle(void) {
    Vehicle *pojazd = (Vehicle *)malloc(sizeof(Vehicle));
    pojazd->owner = NULL;
    Date data = {01, 01, 1970};
    pojazd->valid_date = data;
    pojazd->v_type = v_ndef;
}

void print_v_data(const Vehicle *v) {
    if (v->v_type==v_ndef)
        return;
    if (v->v_type==car) {
        printf("Car: ID: %s, miejsc: %d, naped na: %d kola, hak: ", v->veh_union.cs.vehicle_ID, v->veh_union.cs.places, v->veh_union.cs.driving_wheels, v->veh_union.cs.towbar);  
    }
    else if (v->v_type==truck) {
        return;
    }
}

int compar(Vehicle v1, Vehicle v2) {
    return cmp_date(v1.valid_date, v2.valid_date);
}

int main (void) {

}