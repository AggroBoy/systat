#!/usr/bin/zsh

sudo systemctl stop systat-fcgi
sudo cp systat-fcgi.service /usr/lib/systemd/system
sudo cp systat.cgi /usr/share/nginx/fcgi
sudo mkdir -p /var/run/fcgi
sudo chown http:http /var/run/fcgi
sudo systemctl start systat-fcgi

