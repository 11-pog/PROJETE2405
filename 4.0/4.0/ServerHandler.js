var http = require("http"),
  fs = require("fs"),
  path = require("path");

const thisDir = __dirname;
index = fs.readFileSync(path.dirname(thisDir) + "/4.0/index.html");

const port = 45565;

var app = http.createServer(function (req, res) {
  res.writeHead(200, { "Content-Type": "text/html" });
  res.end(index);
});

app.listen(port, () => {
  console.log("Server connected to ${port}");
});
