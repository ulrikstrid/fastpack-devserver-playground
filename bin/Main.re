open Cmdliner;

let main = (source, output, port, entry) => {
  open Lwt;

  print_endline("Copying index.html...");
  let copyIndexHtml =
    Fs.mkdir(~dir=output, ())
    >>= (
      _ =>
        Fs.copyFile(
          ~source=source ++ "/index.html",
          ~target=output ++ "/index.html",
          (),
        )
    );

  print_endline("Starting server and fastpack...");
  let fileServer = Devserver.start(~port, ~entry, ~output, ());

  Lwt.join([fileServer, copyIndexHtml]) |> Lwt_main.run;
};

let source = {
  let doc = "The source folder to copy files from.";
  Arg.(
    value
    & opt(string, "./source")
    & info(~docv="DIR", ~doc, ["s", "source"])
  );
};

let output = {
  let doc = "The output directory to put files.";
  Arg.(
    value
    & opt(string, "./bundle")
    & info(~docv="DIR", ~doc, ["o", "output"])
  );
};

let port = {
  let doc = "The port to serve files on.";
  Arg.(value & opt(int, 3000) & info(~docv="PORT", ~doc, ["p", "port"]));
};

let entry = {
  let doc = "The main entry point to bundle.";
  Arg.(
    value & pos(0, string, "./src/index.js") & info(~docv="ENTRY", ~doc, [])
  );
};

let main_t = Term.(const(main) $ source $ output $ port $ entry);

let info = {
  let doc = "Run a devserver with fastpack.";
  let man = [`S(Manpage.s_bugs), `P("Report issues on the github repo.")];

  Term.info(
    ~version="%%VERSION%%",
    ~doc,
    ~exits=Term.default_exits,
    ~man,
    "fpack-devserver",
  );
};

let () = Term.exit(Term.eval((main_t, info)));
