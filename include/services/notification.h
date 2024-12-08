#pragma once
#include "entities/User.h"
#include "entities/Note.h"

namespace NotificationService {
  void createFollow(User &followee, User &follower);
  void createBite(optional<Note> note, User &biteee, User &biter);
  void createRenote(Note &note, Note &renote, User &renotee, User &renoter);
  void createFavorite(Note &note, User &favoritee, User &favoriter);
}