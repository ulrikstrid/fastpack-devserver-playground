let copyFile = (~source, ~target, ()): Lwt.t(unit) => {
  let%lwt sourceFile =
    Lwt_io.open_file(~flags=[Unix.O_RDONLY], ~mode=Lwt_io.Input, source);
  let%lwt targetFile =
    Lwt_io.open_file(
      ~flags=[Unix.O_WRONLY, Unix.O_CREAT],
      ~mode=Lwt_io.Output,
      target,
    );
  Lwt_io.read_lines(sourceFile) |> Lwt_io.write_lines(targetFile);
};
