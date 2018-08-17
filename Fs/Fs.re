let mkdir = (~dir, ()): Lwt.t(string) =>
  Lwt.(
    (
      try%lwt (Lwt_unix.mkdir(dir, 511)) {
      | _ =>
        Printf.sprintf("could n ot create the directory %s", dir)
        |> print_endline;
        Lwt.return();
      }
    )
    >|= (() => dir)
  );

let copyFile = (~source, ~target, ()): Lwt.t(unit) => {
  let%lwt sourceFile = Lwt_io.open_file(~mode=Lwt_io.Input, source);
  let%lwt targetFile = Lwt_io.open_file(~mode=Lwt_io.Output, target);

  let%lwt file = Lwt_io.read(sourceFile);
  Lwt_io.write(targetFile, file);
};
