#include "headers/seeder.h"
using ai::Seeder;
using ai::SRandSeeder;
using ai::RandomDeviceSeeder;


SRandSeeder::SRandSeeder() {
    srand(time(NULL));
}

unsigned int SRandSeeder::get() {
    return rand();
}

unsigned int RandomDeviceSeeder::get() {
    return (unsigned int)device();
}

