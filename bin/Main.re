open Cmdliner;

let main = () => {
  open Lwt;
  print_endline("Copying index.html");

  let copyIndexHtml =
    Fs.mkdir(~dir="./bundle", ())
    >>= (
      _ =>
        Fs.copyFile(
          ~source="./public/index.html",
          ~target="./bundle/index.html",
          (),
        )
    );

  print_endline("Starting server");
  let fileServer = Devserver.start();

  Lwt.join([fileServer, copyIndexHtml]) |> Lwt_main.run;
};

let main_t = Term.(const(main) $ const());

let () = Term.exit(Term.eval((main_t, Term.info("Devserver"))));
