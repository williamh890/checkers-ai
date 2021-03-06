#include "seeder.h"
using ai::getSeeder;
using ai::RandomDeviceSeeder;
using ai::Seeder;
using ai::SRandSeeder;

#include "consts.h"
using ai::Settings::SEEDING_METHOD;

#include <memory>
using std::make_shared;
using std::shared_ptr;

Seeder::Seeder() = default;

SRandSeeder::SRandSeeder() : Seeder() { srand(time(nullptr)); }

RandomDeviceSeeder::RandomDeviceSeeder() : Seeder() {}

unsigned int SRandSeeder::get() { return rand(); }

unsigned int RandomDeviceSeeder::get() { return (unsigned int)device(); }

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
