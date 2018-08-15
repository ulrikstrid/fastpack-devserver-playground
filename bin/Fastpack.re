let logProcess = (p: Lwt_process.process_full, sendMessage) =>
  Lwt.join([
    Lwt_io.read_lines(p#stdout)
    |> Lwt_stream.iter_s(s => {
         print_endline(s);
         sendMessage(s);
       }),
    Lwt_io.read_lines(p#stderr)
    |> Lwt_stream.iter_s(s => {
         print_endline(s);
         sendMessage(s);
       }),
  ]);

let start = (~sendMessage: string => Lwt.t(unit)) =>
  Lwt.(
    Lwt_process.with_process_full(
      (
        "fpack",
        [|
          "fpack",
          "--report=json",
          "--watch",
          "--development",
          "./public/index.js",
        |],
      ),
      p => {
        let%lwt status = p#status
        and () = logProcess(p, sendMessage);
        Lwt.return(status);
      },
    )
    >>= (_ => Lwt.return())
  );
