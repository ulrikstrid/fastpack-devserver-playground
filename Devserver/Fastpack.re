let logProcess = (~sendMessage, p: Lwt_process.process_full) =>
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

let start = (~sendMessage: string => Lwt.t(unit), ~entry, ~output, ()) =>
  Lwt_process.with_process_full(
    (
      "fpack",
      [|
        "fpack",
        "--report=json",
        "--watch",
        "--development",
        "--output=" ++ output,
        entry,
      |],
    ),
    logProcess(~sendMessage),
  );
