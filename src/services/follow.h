#pragma once
#include <common.h>
#include "../schema.h"
#include "../entities/Follow.h"
#include "../entities/User.h"

namespace FollowService {
  Follow create(User &user, string followee);
  Follow ingest(const string uri, const json follow);
}
