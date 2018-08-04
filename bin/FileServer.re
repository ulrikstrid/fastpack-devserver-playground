module C = Cohttp_lwt_unix;
module CServer = Cohttp_lwt_unix.Server;

let serveStatic = (base, path) => {
  let fname = Filename.concat(base, path);
  if (Sys.file_exists(fname)) {
    if (Sys.is_directory(fname)) {
      if (Sys.file_exists(Filename.concat(fname, "index.html"))) {
        CServer.respond_file(
          ~fname=Filename.concat(fname, "index.html"),
          (),
        );
      } else {
        C.Server.respond_string(~status=`Not_found, ~body="", ());
      };
    } else {
      CServer.respond_file(~fname, ());
    };
  } else if (Sys.file_exists(fname ++ ".html")) {
    CServer.respond_file(~fname=fname ++ ".html", ());
  } else {
    C.Server.respond_string(~status=`Not_found, ~body="", ());
  };
};

let callback = (_conn, req: Cohttp.Request.t, _body) => {
  let _ = Lwt_io.printf("Req: %s\n", req.resource);
  let req_path = Cohttp.Request.uri(req) |> Uri.path;
  let path_parts = Str.(split(regexp("/"), req_path));
  switch (req.meth, path_parts) {
  | (`GET, _) => serveStatic("./public", req_path)
  | _ => C.Server.respond_string(~status=`Not_found, ~body="", ())
  };
};

let start = (~port=3000, ()) => {
  Printf.sprintf("Listening on port %d...", port) |> print_endline;
  C.Server.create(~mode=`TCP(`Port(port)), C.Server.make(~callback, ()));
};
