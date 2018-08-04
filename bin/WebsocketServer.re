open Websocket;
open Websocket_lwt;

let clients: ref(option(list(Connected_client.t))) = ref(None);

let broadcast = message =>
  switch (clients^) {
  | None => ()
  | Some(c) =>
    List.iter(
      client =>
        Connected_client.send(
          client,
          Frame.create(~opcode=Frame.Opcode.Text, ~content=message, ()),
        )
        |> ignore,
      c,
    )
  };
let react = (client: Connected_client.t, id: int) => {
  let _ = Lwt_io.printf("new connection from client %d\n", id);

  switch (clients^) {
  | None => clients := Some([client])
  | Some(c) => clients := Some([client, ...c])
  };
  open Lwt;
  let rec inner = () =>
    Connected_client.recv(client)
    >>= (
      fr =>
        switch (fr.opcode) {
        | Frame.Opcode.Pong => inner()
        | Frame.Opcode.Text
        | Frame.Opcode.Binary =>
          let () = print_endline(fr.content);
          Connected_client.send(client, fr) >>= inner;
        | _ => Connected_client.send(client, Frame.(close(1002)))
        }
    );
  inner();
};

let start = (uri: Uri.t) => {
  open Lwt;
  let id = ref(-1);
  let handle_conn = client => {
    incr(id);
    let id = id^;
    Lwt.catch(() => react(client, id), _ => Lwt_io.printf("error\n"));
  };

  Resolver_lwt.resolve_uri(~uri, Resolver_lwt_unix.system)
  >>= (
    endp => {
      open Conduit_lwt_unix;
      let endp_str =
        endp |> Conduit.sexp_of_endp |> Sexplib.Sexp.to_string_hum;

      Lwt_io.printf("%s", endp_str)
      >>= (
        () =>
          endp_to_server(~ctx=default_ctx, endp)
          >>= (
            server => {
              let server_str =
                server |> sexp_of_server |> Sexplib.Sexp.to_string_hum;

              Lwt_io.printf("%s", server_str)
              >>= (
                () =>
                  establish_standard_server(
                    ~ctx=default_ctx,
                    ~mode=server,
                    handle_conn,
                  )
              );
            }
          )
      );
    }
  );
};
