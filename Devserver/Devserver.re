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

let createCallback =
    (
      ~output,
      ~websocketHandler,
      ~proxyHandler,
      ~shouldProxy,
      ~proxyPath,
      ~proxyPathRewrite,
      conn,
      req: Cohttp.Request.t,
      body,
    ) => {
  let _ = Lwt_io.printf("Req: %s\n", req.resource);
  let req_path = Cohttp.Request.uri(req) |> Uri.path;
  let path_parts = Str.(split(regexp("/"), req_path));

  let proxyPathRewrite =
    switch (proxyPathRewrite) {
    | Some(p) => p
    | None => proxyPath
    };

  switch (req.meth, path_parts, shouldProxy) {
  | (_, [apiPath], true) when apiPath == proxyPath =>
    proxyHandler(proxyPathRewrite, req, body)
  | (_, [apiPath, ...path], true) when apiPath == proxyPath =>
    proxyHandler(String.concat("/", [proxyPathRewrite, ...path]), req, body)
  | (`GET, ["ws"], _) => websocketHandler(conn, req, body)
  | (`GET, _, _) => serveStatic(output, req_path)
  | _ => C.Server.respond_string(~status=`Not_found, ~body="", ())
  };
};

let start =
    (
      ~port=3000,
      ~entry,
      ~output,
      ~proxyTarget,
      ~proxyPath,
      ~proxyPathRewrite,
      (),
    ) => {
  Printf.sprintf("Listening on port %d...", port) |> print_endline;

  let (host, shouldProxy) =
    switch (proxyTarget) {
    | Some(target) => (target, true)
    | None => ("", false)
    };

  let proxyHandler = ProxyHandler.makeHandler(~host);

  let (sendMessage, websocketHandler) =
    WebsocketHandler.makeHandler(~debug=true, ());

  Lwt.join([
    Fastpack.start(~sendMessage, ~entry, ~output, ()),
    C.Server.create(
      ~mode=`TCP(`Port(port)),
      C.Server.make(
        ~callback=
          createCallback(
            ~output,
            ~websocketHandler,
            ~proxyHandler,
            ~shouldProxy,
            ~proxyPath,
            ~proxyPathRewrite,
          ),
        (),
      ),
    ),
  ]);
};
