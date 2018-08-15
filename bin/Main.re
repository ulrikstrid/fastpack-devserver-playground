open Cmdliner;

let main = () => {
  print_endline("Starting server");
  let (sendMessage, fileServer) = FileServer.start();

  Lwt.join([fileServer, Fastpack.start(~sendMessage)]) |> Lwt_main.run;
};

let main_t = Term.(const(main) $ const());

let () = Term.exit(Term.eval((main_t, Term.info("Devserver"))));
