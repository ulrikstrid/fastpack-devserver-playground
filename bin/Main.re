let main = () => {
  open Lwt;
  print_endline("Starting server");
  let (sendMessage, fileServer) = FileServer.start();

  let sendMessageAfter = Lwt_unix.sleep(5.) >>= (() => sendMessage("test"));

  Lwt.join([fileServer, sendMessageAfter]) |> Lwt_main.run;
};

main();
