console.log("bundle");

const ws = new WebSocket("ws://localhost:3000/ws");

ws.addEventListener("open", event => {
  console.log(event);
  ws.send("hello");
});

ws.addEventListener("message", event => {
  console.log(event.data);
  // debugger;
  // location.reload();
});
