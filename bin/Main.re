let main = () => {
  print_endline("Starting server");
  let (sendMessage, fileServer) = FileServer.start();

  Lwt.join([fileServer, Fastpack.start(~sendMessage)]) |> Lwt_main.run;
};

  Lwt.join([fileServer, sendMessageAfter]) |> Lwt_main.run;
};

main();
