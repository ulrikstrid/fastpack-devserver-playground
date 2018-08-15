import { test } from "./test";

const ws = new WebSocket("ws://localhost:3000/ws");

ws.addEventListener("open", event => {
  ws.send("hello");
});

ws.addEventListener("message", event => {
  console.log(event.data, event.data.includes("Packed"));
  if (event.data.includes("Packed")) {
    location.reload();
  }
});

console.log(test);
