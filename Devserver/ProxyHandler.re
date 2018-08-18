open Lwt;
open Cohttp_lwt_unix;

let makeHandler = (~host, path, req: Cohttp.Request.t, body) => {
  let proxyPath = host ++ path;
  let headers = Cohttp.Header.replace(req.headers, "Host", host);

  print_endline(Cohttp.Header.to_string(headers));

  print_endline("Request sent to: " ++ proxyPath);

  (
    switch (req.meth) {
    | `GET => Client.get(~headers, Uri.of_string(proxyPath))
    | `POST => Client.post(~body, ~headers, Uri.of_string(proxyPath))
    | `PUT => Client.put(~body, ~headers, Uri.of_string(proxyPath))
    | `PATCH => Client.patch(~body, ~headers, Uri.of_string(proxyPath))
    | `DELETE => Client.delete(~body, ~headers, Uri.of_string(proxyPath))
    | _ => Client.get(Uri.of_string(proxyPath))
    }
  )
  >>= (
    ((resp, body)) => {
      let headers = resp |> Response.headers;
      let status = resp |> Response.status;

      print_endline("Response from: " ++ proxyPath);

      print_endline(Cohttp.Header.to_string(headers));
      Cohttp_lwt_unix.Server.respond(~headers, ~body, ~status, ());
    }
  );
};
