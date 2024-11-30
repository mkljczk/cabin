#include "SQLiteCpp/Statement.h"
#include <type_traits>
#define USE_DB
#include "router.h"
#include "common.h"

#include "../../utils.h"
#include "../../services/note.h"
GET(notes, "/notes/:id") {
  std::string id(req->getParameter("id"));
  std::string idurl = API("notes/"+id);



  auto note = NoteService::lookup(id);

  if (!note.has_value()) {
    ERROR(404, "no such note");
  }
  

  OK(note->renderAP(), MIMEAP);
}
