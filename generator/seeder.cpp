#include "headers/seeder.h"
using ai::Seeder;
using ai::SRandSeeder;
using ai::RandomDeviceSeeder;

Seeder::Seeder() {
}

SRandSeeder::SRandSeeder(): Seeder() {
    srand(time(NULL));
}

RandomDeviceSeeder::RandomDeviceSeeder(): Seeder(){
}

unsigned int SRandSeeder::get() {
    return rand();
}

unsigned int RandomDeviceSeeder::get() {
    return (unsigned int)device();
}

