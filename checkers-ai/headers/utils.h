#ifndef UTILS_H
#define UTILS_H

#include "models.h"
// ai::Position

#include <string>
// std::string


namespace ai {
    Position spaceToPosition(int space);

    int positionToSpace(const Position & pos);

    std::string getUsername ();

    std::string idToFilename(int ID);

    double getTime();

// class NetworkTestingNamechange
// Simple object to handle changing username for testing.
// Changes name back when it goes out of scope.
    class NetworkTestingNamechange {
        public:
        NetworkTestingNamechange ();
        ~NetworkTestingNamechange ();
        private: 
        std::string _previousName;

        void changeUsername(std::string);
    };
}

#endif
