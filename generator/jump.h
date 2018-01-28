#ifndef JUMP_H
#define JUMP_H

struct Jump {
    int to;
    int from;

    Jump(int to, int from): to(to), from(from)  {
    }
};

#endif
