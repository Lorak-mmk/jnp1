#ifndef PLAYER_IPLAYABLE_H
#define PLAYER_IPLAYABLE_H

class IPlayable {
public:
    virtual void play() = 0;
    virtual bool contains(IPlayable* other) {
        return this == other;
    }
    virtual ~IPlayable() = default;
};

#endif