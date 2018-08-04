console.log("bundle");

const ws = new WebSocket('ws://localhost:8080');

ws.addEventListener("open", (event) => {
  console.log(event);
  ws.send(JSON.stringify({
    'msg_type': 'chat_message',
    'message': "Here's some text that the server is urgently awaiting!"
  }));
});

ws.addEventListener('message', event => {
  console.log(event.data);
  // debugger;
  // location.reload();
});
