var http = require("http"),
  fs = require("fs"),
  path = require("path"),
  os = require("os");

let armazenamento = fs.readFileSync(
  __dirname + "/4.0/armazenamento.html",
  "utf-8"
);
let index = fs.readFileSync(__dirname + "/4.0/index.html", "utf-8");
let tempo = fs.readFileSync(__dirname + "/4.0/tempo.html", "utf-8");
let visao = fs.readFileSync(__dirname + "/4.0/visao.html", "utf-8");

let style = fs.readFileSync(__dirname + "/4.0/css/style.css", "utf-8");
let it = fs.readFileSync(__dirname + "/4.0/css/it.css", "utf-8");
let vc = fs.readFileSync(__dirname + "/4.0/css/vc.css", "utf-8");

let script = fs.readFileSync(__dirname + "/4.0/js/script.js", "utf-8");

const port = 45565;

var app = http.createServer(function (req, res) {
  let url = req.url;

  if (url === "/") {
    res.writeHead(200, { "Content-Type": "text/html" });
    res.end(index);
  } else if (url === "/armazenamento") {
    res.writeHead(200, { "Content-Type": "text/html" });
    res.end(armazenamento);
  } else if (url === "/tempo") {
    res.writeHead(200, { "Content-Type": "text/html" });
    res.end(tempo);
  } else if (url === "/visao") {
    res.writeHead(200, { "Content-Type": "text/html" });
    res.end(visao);
  } else if (url === "/css/style.css") {
    res.writeHead(200, { "Content-Type": "text/css" });
    res.end(style);
  } else if (url === "/css/it.css") {
    res.writeHead(200, { "Content-Type": "text/css" });
    res.end(it);
  } else if (url === "/css/vc.css") {
    res.writeHead(200, { "Content-Type": "text/css" });
    res.end(vc);
  } else if (url === "/js/script.js") {
    res.writeHead(200, { "Content-Type": "application/javascript" });
    res.end(script);
  }
});

app.listen(port, () => {
  console.log(`Server connected to ${port}`);
});
