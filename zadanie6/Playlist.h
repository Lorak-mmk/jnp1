#ifndef PLAYER_PLAYLIST_H
#define PLAYER_PLAYLIST_H

#include <cstddef>
#include <vector>
#include "IPlayMode.h"
#include "Playable.h"

// TODO: sprawdzanie cykli w add albo play (wsm w play bo cykl może się pojawić już po add, np jeśli zrobimy add w zagnieżdżonej playliście)
class Playlist : public Playable {
public:
    explicit Playlist(std::string name);
    void add(const std::shared_ptr<Playable>& element);
    void add(const std::shared_ptr<Playable>&, size_t position);
    void remove();
    void remove(size_t position);
    void setMode(std::shared_ptr<IPlayMode> new_mode);
    void play() override;

private:
    std::string name;
    std::vector<std::shared_ptr<Playable>> elements;
    std::shared_ptr<IPlayMode> mode;
};

#endif  // PLAYER_PLAYLIST_H
