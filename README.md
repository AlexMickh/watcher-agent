# Watcher - a tool that helps you to monitor the server status

This part of app - agent that runs on server and send data other gRPC. Wather is an open source app that allows you to replace its parts. If you want to do it you just need to implement your service that matches the contract from <a href="https://github.com/AlexMickh/watcher-agent/tree/master/protos">protos folder.</a>
This app reads log file you specify in the config and also monitors the CPU load.

## To start using Watcher Agent
1. Download source code
```bash
git clone https://github.com/AlexMickh/watcher-agent.git
```
2. Compile it (only for linux). It requires this dependencies:
```
nlohmann-json grpc
```
3. After that you can run it 🥳
```bash
./agent path/to/config/file
```
Here is example of config file:
```json
{
    "service_name": "test",
    "server_addr": "localhost:8080",
    "log_path": "test.log"
}
```

After all you can use Watcher Agent with <a href="https://github.com/AlexMickh/watcher-server">Watcher Server</a> (in development) or develop your own server app.