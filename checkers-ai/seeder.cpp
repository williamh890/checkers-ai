#include "headers/seeder.h"
using ai::Seeder;
using ai::SRandSeeder;
using ai::RandomDeviceSeeder;
using ai::getSeeder;

#include "headers/consts.h"
using ai::Settings::SEEDING_METHOD;

#include <memory>
using std::shared_ptr;
using std::make_shared;

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

shared_ptr<Seeder> ai::getSeeder() {
    shared_ptr<Seeder> seeder;

    if (SEEDING_METHOD == "random_device") {
        seeder = make_shared<RandomDeviceSeeder>();
    }
    if (SEEDING_METHOD == "time") {
        seeder = make_shared<SRandSeeder>();
    }

    return seeder;
}
