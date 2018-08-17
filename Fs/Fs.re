let mkdir = (~dir, ()): Lwt.t(string) =>
  (
    try%lwt (Lwt_unix.mkdir(dir, 511)) {
    | _ => Lwt.return() /* It probably failed because it exists */
    }
  )
  |> Lwt.map(() => dir);

let copyFile = (~source, ~target, ()): Lwt.t(unit) => {
  let%lwt sourceFile = Lwt_io.open_file(~mode=Lwt_io.Input, source);
  let%lwt targetFile = Lwt_io.open_file(~mode=Lwt_io.Output, target);

  let%lwt file = Lwt_io.read(sourceFile);
  let%lwt () = Lwt_io.write(targetFile, file)
  Lwt.join([Lwt_io.close(targetFile), Lwt_io.close(targetFile)]);
};
