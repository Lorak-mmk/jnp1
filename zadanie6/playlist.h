#ifndef PLAYER_MAIN_H
#define PLAYER_MAIN_H

#include "Exceptions.h"
#include "File.h"
#include "OddEvenMode.h"
#include "SequenceMode.h"
#include "ShuffleMode.h"
#include "Player.h"

/* To add new module (like Audio or Video):
 * create class deriving from IPlayable
 * To make it constructible from file create class deriving from IMediaExtractor
 * Register your filetype by calling MediaFactory::registerFiletype(type, extractor)
 *
 * Library not safe to use in init - filetypes may not be loaded (static initalization order fiasco)
*/

#endif  // PLAYER_MAIN_H
