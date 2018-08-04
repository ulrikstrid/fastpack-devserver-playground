let main = () => {
  let fileServer = FileServer.start();

  let uri = Uri.make(~scheme="http", ~host="127.0.0.1", ~port=8080, ());
  let websocketServer = WebsocketServer.start(uri);

  Lwt.join([fileServer, websocketServer]) |> Lwt_main.run;
};

main();
