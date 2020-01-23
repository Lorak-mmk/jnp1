#ifndef PLAYER_PLAYLIST_H
#define PLAYER_PLAYLIST_H

#include <set>
#include <vector>
#include "IPlayMode.h"
#include "IPlayable.h"

class Playlist : public IPlayable {
public:
    explicit Playlist(std::string name);
    bool contains(IPlayable *other) override;
    void add(const std::shared_ptr<IPlayable> &element);
    void add(const std::shared_ptr<IPlayable> &element, size_t position);
    void remove();
    void remove(size_t position);
    void setMode(std::shared_ptr<IPlayMode> new_mode) noexcept;
    void play() override;

private:
    std::string name;
    std::vector<std::shared_ptr<IPlayable>> elements;
    std::shared_ptr<IPlayMode> mode;
};

#endif  // PLAYER_PLAYLIST_H