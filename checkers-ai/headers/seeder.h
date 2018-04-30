#ifndef SEEDER_H
#define SEEDER_H

#include <random>
// std::random_device
#include <memory>
// std::shared_ptr
#include <string>
// std::string

namespace ai {
class Seeder {
 public:
  Seeder();
  virtual unsigned int get() = 0;
};

class SRandSeeder : public Seeder {
 public:
  SRandSeeder();
  unsigned int get() override;
};

class RandomDeviceSeeder : public Seeder {
  std::random_device device;

 public:
  RandomDeviceSeeder();
  unsigned int get() override;
  ~RandomDeviceSeeder() = default;
};

std::shared_ptr<Seeder> getSeeder();
}  // namespace ai

#endif  // SEEDER_H
