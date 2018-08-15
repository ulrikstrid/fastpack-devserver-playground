import { test } from "./test";

const ws = new WebSocket("ws://localhost:3000/ws");

ws.addEventListener("open", event => {
  ws.send("hello");
});

ws.addEventListener("message", event => {
  console.log(event.data);
  if (event.data !== "hello") {
    const data = JSON.parse(event.data);
    console.log(data);
    if (data.error) {
      console.log(data.error);
      document.querySelector("#index").innerHTML = `<pre>${data.error}</pre>`;
      /*
        .replace(" ", "&nbsp;")
        .split("\n")
        .join("<br>");
        */
    } else {
      debugger;
      window.location.reload();
    }
  }
});

console.log(test);
