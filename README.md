# TinyBooter

TinyBooter is a small software for Docker that helps to launch and keep running services through ini files configuration. It is designed to have a small footprint as possible : memory use is less than 50KB (it depends on how many services you want to run but it doesn't change a lot !). It also contains a small healthcheck code for Docker.

This image alone doesn't do anything, it serves as a base : you can add a service as an ini file inside /etc/tinybooter.d/ directory like this :

```ini
name=myservicename
command=command to run
restart=true (optional, default to true)
max_retry=5 (optional, default to 5)
```

You can access the TinyBooter's official image [here](https://hub.docker.com/r/fangedhex/tinybooter/).

## Contributions

At the moment, I made this small software cause I was in need of a small software that can do that simple work. If you find any bugs or have a cool idea to improve it : don't hesitate to create an issue here.

Want to add something yourself ? Fork this project, add your code and do a pull request :)
