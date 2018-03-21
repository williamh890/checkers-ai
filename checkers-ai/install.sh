mkdir -p $(BUILD_DIR)
curl https://github.com/nlohmann/json/releases/download/v3.1.0/json.hpp > headers/json.hpp -L
curl https://github.com/catchorg/Catch2/releases/download/v2.1.1/catch.hpp > tests/catch.hpp -L
