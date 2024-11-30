#define USE_DB
#include <router.h>
#include <common.h>
#include "../../schema.h"
#include "../../utils.h"
#include "local.h"
#include "../../services/note.h"
#include "../../services/user.h"
#include "../../http.h"

POST(post_status, "/api/v1/statuses") {
  Note note = NoteService::create(ct->userid, body["status"]);
  json j = MSrenderNote(note);

  OK(j, MIMEJSON);
}

GET(status, "/api/v1/statuses/:id") {
  string id (req->getParameter("id"));

  auto n = NoteService::lookup(id);
  if (!n.has_value()) {
    ERROR(404, "no note");
  }

  json j = MSrenderNote(n.value());
  OK(j, MIMEJSON);
}

POST(status_like, "/api/v1/statuses/:id/favourite") {
  string id (req->getParameter("id"));

  auto user = UserService::lookup(ct->userid);
  auto note = NoteService::lookup(id);
  if (!note.has_value()) {
    ERROR(404, "no note");
  }

  string likeid = utils::genid();
  Like like = {
    .uri = LIKE(likeid),
    .id = likeid,
    .local = true,

    .owner = user->uri,
    .object = note->uri
  };
  like.insert();

  URL url(note->uri);
  APClient cli(user.value(), url.host);

  json activity = {
    {"actor", user->uri},
    {"id", like.uri},
    {"object", note->uri},
    {"content", "❤"},
    {"_misskey_reaction", "❤"},
    {"type", "Like"}
  };

  auto resp = cli.Post("/inbox", activity);
  dbg(resp->body);
  dbg(resp->status);
}

GET(status_context, "/api/v1/statuses/:id/context") {
  string id (req->getParameter("id"));
  auto n = NoteService::lookup(id);
  if (!n.has_value()) {
    ERROR(404, "no note");
  }


  json ancestors = json::array();
  json descendants = json::array();


  json j = {
    {"ancestors", ancestors},
    {"descendants", descendants},
  };

  OK(j, MIMEJSON);
}


GET(timelines, "/api/v1/timelines/:id") {
  auto q = STATEMENT("SELECT * FROM note");


  json response = json::array();
  while (q.executeStep()) {
    Note n;
    n.load(q);

    response.push_back(MSrenderNote(n));
  }
  std::reverse(response.begin(), response.end());

  OK(response, MIMEJSON);
}
