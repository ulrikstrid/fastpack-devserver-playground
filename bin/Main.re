open Cmdliner;

let main =
    (source, output, port, entry, proxyTarget, proxyPath, proxyPathRewrite) => {
  open Lwt;

  print_endline("Copying index.html...");
  let fileServer =
    Fs.mkdir(~dir=output, ())
    >>= (
      _ =>
        Fs.copyFile(
          ~source=source ++ "/index.html",
          ~target=output ++ "/index.html",
          (),
        )
    )
    >>= (
      _ => {
        let%lwt sourceFile =
          Lwt_io.open_file(~mode=Lwt_io.Input, output ++ "/index.html");

        let%lwt indexHtml = Lwt_io.read(sourceFile);

        let%lwt () = Lwt_io.close(sourceFile);

        let indexHtml =
          Str.replace_first(
            Str.regexp("</body>"),
            Printf.sprintf(
              {|  <script>
    const ws = new WebSocket("ws://localhost:%d/ws");

    ws.addEventListener("open", event => {
      ws.send("hello");
    });

    ws.addEventListener("message", event => {
      console.log(event.data);
      if (event.data !== "hello") {
        const data = JSON.parse(event.data);
        console.log(data);
        if (data.error) {
          console.log(data.error);
          document.querySelector("#index").innerHTML = `<pre>${data.error}</pre>`;
        } else {
          debugger;
          window.location.reload();
        }
      }
    });
  </script>
</body>|},
              port,
            ),
            indexHtml,
          );

        let%lwt targetFile =
          Lwt_io.open_file(~mode=Lwt_io.Output, output ++ "/index.html");

        Lwt_io.write(targetFile, indexHtml)
        >>= (() => Lwt_io.close(targetFile));
      }
    )
    >>= (
      () => {
        print_endline("Starting server and fastpack...");
        Devserver.start(
          ~port,
          ~entry,
          ~output,
          ~proxyTarget,
          ~proxyPath,
          ~proxyPathRewrite,
          (),
        );
      }
    );

  fileServer |> Lwt_main.run;
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

let proxyTarget = {
  let doc = "The adress of the backend server you want to proxy.";
  Arg.(
    value
    & opt(some(string), None)
    & info(~docv="HOST", ~doc, ["pt", "proxytarget"])
  );
};

let proxyPath = {
  let doc = "The path you want to proxy to the backend.";
  Arg.(
    value
    & opt(string, "api")
    & info(~docv="PATH", ~doc, ["pp", "proxypath"])
  );
};

let proxyPathRewrite = {
  let doc = "Rewrite the proxy path to this.";

  Arg.(
    value
    & opt(some(string), None)
    & info(~docv="PATH", ~doc, ["pr", "proxyrewrite"])
  );
};

let entry = {
  let doc = "The main entry point to bundle.";
  Arg.(
    value & pos(0, string, "./src/index.js") & info(~docv="ENTRY", ~doc, [])
  );
};

let main_t =
  Term.(
    const(main)
    $ source
    $ output
    $ port
    $ entry
    $ proxyTarget
    $ proxyPath
    $ proxyPathRewrite
  );

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
